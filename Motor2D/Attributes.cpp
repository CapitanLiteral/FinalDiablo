#include "Attributes.h"
#include <time.h>  

Attributes::Attributes(AttributeBuilder builder) :
base_life(builder.base_life),
base_strength(builder.base_strength),
base_intelligence(builder.base_intelligence),
base_dexterity(builder.base_dexterity),
base_attackSpeed(builder.base_attackSpeed),
base_castSpeed(builder.base_castSpeed),
base_damage(builder.base_damage),
base_accuracy(builder.base_accuracy),
base_critChance(builder.base_critChance),
base_critMultiplier(builder.base_critMultiplier),
base_armor(builder.base_armor),
base_evasionRating(builder.base_evasionRating),
base_lifeRegen(builder.base_lifeRegen),
base_blockChance(builder.base_blockChance),
base_movementSpeed(builder.base_movementSpeed),
base_maxEnduranceCharges(builder.base_maxEnduranceCharges),
base_maxFrenzyCharges(builder.base_maxFrenzyCharges),
base_maxPowerCharges(builder.base_maxPowerCharges),
base_endranceChargeDamageReduction(builder.base_endranceChargeDamageReduction),
base_frenzyChargeAttackSpeedIncrease(builder.base_frenzyChargeAttackSpeedIncrease),
base_frenzyChargeCastSpeedIncrease(builder.base_frenzyChargeCastSpeedIncrease),
base_frenzyChargeDamageIncrease(builder.base_frenzyChargeDamageIncrease),
base_powerChargeCritChanceIncrease(builder.base_powerChargeCritChanceIncrease),
current_level(builder.current_level),
current_life(builder.current_life),
experience(builder.experience)
{
	player = false;

	for (int i = 0; i < modifiers.max_size(); i++)
	{
		modifiers[i].clear();
	}

	lifeRegenTimer.start();
}

Attributes::~Attributes()
{
	for (int i = 0; i < modifiers.max_size(); i++)
	{
		modifiers[i].clear();
	}
}

bool Attributes::setReferences(int* _x, int* _y)
{
	bool ret;

	if (ret = (_x != NULL && _y != NULL))
	{
		x = _x;
		y = _y;
	}

	return ret;
}

bool Attributes::setHud(Hud* _hud)
{
	bool ret;

	if (ret = (_hud != NULL))
	{
		hud = _hud;
	}

	return ret;
}

bool Attributes::addMod(Modifier* mod)
{
	bool ret;

	if (ret = (mod != NULL))
	{
		switch (mod->type) // check for availability if it's got a max value
		{
		case ENDURANCE_CHARGE:
		{
			if (!(ret = (getEnduranceCharges() >= getMaxEnduranceCharges()))) return ret;
			break;
		}
		case FRENZY_CHARGE:
		{
			if (!(ret = (getFrenzyCharges() >= getMaxFrenzyCharges()))) return ret;
			break;
			break;
		}
		case POWER_CHARGE:
		{
			if (!(ret = (getPowerCharges() >= getMaxPowerCharges()))) return ret;
			break;
		}
		default: break;
		}

		modifiers[int(mod->type)].push_back(mod);
	}

	return ret;
}


bool Attributes::update()
{
	bool ret;

	if (ret = (current_life > 0))
	{
		std::vector<Modifier*>::const_iterator it;
		for (int i = 0; i <= int(POWER_CHARGE_CRIT_CHANCE_INCREASE); i++)
		{
			for (it = modifiers[i].begin(); it != modifiers[i].end(); it++)
			{
				if (!((*it)->update())) // on false: clear & remove mod
				{
					(*it)->clear();
					modifiers[i].erase(it);
					it--;
				}
			}
		}

		// life regen
		addLife(lifeRegenTimer.ReadSec() * getLifeRegen());
		lifeRegenTimer.start();
	}

	return ret;
}

void Attributes::reset(bool clearMods)
{
	// reset life
	current_life = getMaxLife();

	if (clearMods)
	{
		// clear mods
		for (int i = 0; i < modifiers.max_size(); i++)
		{
			modifiers[i].clear();
		}
	}
}

// Adders / setters

bool Attributes::addLife(float val)
{
	bool ret;

	if (ret = (current_life + val > 0.0f))
	{
		float maxLife = getMaxLife();

		if (current_life + val <= maxLife)
		{
			current_life += val;
		}
		else
		{
			current_life = maxLife;
		}
	}
	else
	{
		current_life = 0.0f;
	}

	return ret;
}

void Attributes::addExp(int exp)
{
	experience += exp;
}

bool Attributes::setLevel(int val)
{
	bool ret;

	if (ret = (val > 0))
	{
		current_level = val;
	}

	return ret;
}

void Attributes::setLife(int newLife)
{
	current_life = newLife;
}

// Getters

float Attributes::getMaxLife()const // strength adds +1 life for each 2 strength
{
	return ((base_life + getMod(FLAT_LIFE) + ((getStrength() / 2.0f)) * getMod(NONFLAT_LIFE)));
}

float Attributes::getStrength()const
{
	return ((base_strength + getMod(FLAT_STRENGTH)) * getMod(NONFLAT_STRENGTH));
}

float Attributes::getIntelligence()const
{
	return ((base_intelligence + getMod(FLAT_INTELLIGENCE)) * getMod(NONFLAT_INTELLIGENCE));
}

float Attributes::getDexterity()const
{
	return ((base_dexterity + getMod(FLAT_DEXTERITY)) * getMod(NONFLAT_DEXTERITY));
}

float Attributes::getAttackSpeed()const // depends on frenzy charges and dexterity
{
	return ((base_attackSpeed + getMod(FLAT_ATTACK_SPEED)) * (getMod(NONFLAT_ATTACK_SPEED) + (getFrenzyCharges() * getFrenzyChargeAttackSpeedIncrease()) + (getDexterity() / 2.0f)));
}

float Attributes::getCastSpeed()const // depends on frenzy charges and intelligence
{
	return ((base_castSpeed + getMod(FLAT_CAST_SPEED)) * (getMod(NONFLAT_CAST_SPEED) + (getFrenzyCharges() * getFrenzyChargeCastSpeedIncrease()) + (getIntelligence() / 2.0f)));
}

float Attributes::getDamage()const // depends on frenzy charges and strength
{
	return ((base_damage + getMod(FLAT_DAMAGE)) * (getMod(NONFLAT_DAMAGE) + (getFrenzyCharges() * getFrenzyChargeDamageIncrease()) + (getStrength() / 250.0f)));
}

float Attributes::getAccuracy()const // depends on dexterity
{
	return ((base_accuracy + getMod(FLAT_ACCURACY) + (getDexterity() * 2.0f)) * getMod(NONFLAT_ACCURACY));
}

float Attributes::getCritChance()const // depends on power charges and intelligence
{
	return ((base_critChance + getMod(FLAT_CRIT_CHANCE)) * (getMod(NONFLAT_CRIT_CHANCE) + (getPowerCharges() * getPowerChargeCritChanceIncrease()) + (getIntelligence() / 500.0f)));
}

float Attributes::getCritMult()const // depends on intelligence
{
	return ((base_critMultiplier + getMod(FLAT_CRIT_MULT) + (getIntelligence() / 200.0f)) * getMod(NONFLAT_CRIT_MULT));
}

float Attributes::getArmor()const
{
	return ((base_armor + getMod(FLAT_ARMOR)) * getMod(NONFLAT_ARMOR));
}

float Attributes::getEvasionRating()const // depends on dexterity
{
	return ((base_evasionRating + getMod(FLAT_EVASION) + (getDexterity() / 500.0f)) * getMod(NONFLAT_EVASION));
}

float Attributes::getLifeRegen()const // depends on strength
{
	return ((base_lifeRegen + getMod(FLAT_LIFE_REGEN)) * (getMod(NONFLAT_LIFE_REGEN) + (getStrength() / 100)));
}

float Attributes::getBlockChance()const
{
	return ((base_blockChance + getMod(FLAT_BLOCK_CHANCE)) * getMod(NONFLAT_BLOCK_CHANCE));
}

float Attributes::getMovementSpeed()const
{
	return ((base_movementSpeed + getMod(FLAT_MOVEMENT_SPEED)) * getMod(NONFLAT_MOVEMENT_SPEED));
}

float Attributes::getLifeLeach()const
{
	return ((getMod(FLAT_LIFE_LEACH)) * getMod(NONFLAT_LIFE_LEACH));
}

float Attributes::getEnduranceCharges()const
{
	return getMod(ENDURANCE_CHARGE);
}

float Attributes::getFrenzyCharges()const
{
	return getMod(FRENZY_CHARGE);
}

float Attributes::getPowerCharges()const
{
	return getMod(POWER_CHARGE);
}

float Attributes::getMaxEnduranceCharges()const
{
	return (base_maxEnduranceCharges + getMod(MAX_ENDURANCE_CHARGE));
}

float Attributes::getMaxFrenzyCharges()const
{
	return (base_maxFrenzyCharges + getMod(MAX_FRENZY_CHARGE));
}

float Attributes::getMaxPowerCharges()const
{
	return (base_maxPowerCharges + getMod(MAX_POWER_CHARGE));
}

float Attributes::getEnduranceChargeDamageReduction()const
{
	return (base_endranceChargeDamageReduction + getMod(ENDURANCE_CHARGE_DAMAGE_REDUCTION_INCREASE));
}

float Attributes::getFrenzyChargeAttackSpeedIncrease()const
{
	return (base_frenzyChargeAttackSpeedIncrease + getMod(FRENZY_CHARGE_ATTACK_SPEED_INCREASE));
}

float Attributes::getFrenzyChargeCastSpeedIncrease()const
{
	return (base_frenzyChargeCastSpeedIncrease + getMod(FRENZY_CHARGE_CAST_SPEED_INCREASE));
}

float Attributes::getFrenzyChargeDamageIncrease()const
{
	return (base_frenzyChargeDamageIncrease + getMod(FRENZY_CHARGE_DAMAGE_INCREASE));
}

float Attributes::getPowerChargeCritChanceIncrease()const
{
	return (base_powerChargeCritChanceIncrease + getMod(POWER_CHARGE_CRIT_CHANCE_INCREASE));
}







bool Attributes::damage(Attributes* attacker, int attackType)
{
	bool ret = true;

	if (attacker == NULL)
		return ret;

	// Steps:
	// 1 - check if evade or block
	// 2 - check if crit
	// 3 - calculate damage mitigation
	// 4 - check damage

	float attackerAcuracy = attacker->getAccuracy();
	int chanceToEvade = int((1.0f - (attackerAcuracy / (attackerAcuracy + (0.12 * getEvasionRating())))) * 100);
	int randValue = rand() % 100 + 1;     // range 1 to 100

	if (!(chanceToEvade > randValue && ((getBlockChance() * 100) > randValue))) //hit
	{
		float damage = attacker->getDamage();
		bool crit;
		if (crit = ((attacker->getCritChance() * 100) > randValue)) // apply crit multiplier if crit
		{
			damage *= attacker->getCritMult();
		}

		float armor = getArmor();
		float damageMitigation = (armor / (armor + (10 * damage))); // calculate armor damage reduction: 0.0 - 1.0

		float enduranceCharges;
		if ((enduranceCharges = getEnduranceCharges()) > 0.0f) // apply endurance charge damage reduction if there are endurance charges
		{
			damageMitigation += (enduranceCharges * getEnduranceChargeDamageReduction());
		}

		// reduce damage to take
		if (damageMitigation > 0.0f)
		{
			damage -= damage * damageMitigation;
		}

		if (ret = (damage < current_life))
		{
			current_life -= damage;
		}
		else
		{
			current_life = 0.0f;
		}

		float leach = attacker->getLifeLeach(); // apply life leach
		if (leach > 0.0)
		{
			attacker->addLife(leach * damage);
		}

		// trigger hit with/out bool crit
		if (hud && x && y) hud->displayDamage((*x), (*y), damage, crit);

		if (attacker->player) // on player
		{
			if (!ret) // on kill
			{
				attacker->addExp(experience);
			}

			attacker->addRage(25.0f);
		}
	}

	if (hud && x && y) hud->displayDamage((*x), (*y), 0.0f);

	return ret;
}

float Attributes::getMod(modifierType type)const
{
	float ret = 0.0f;

	if (type < ENDURANCE_CHARGE)
	{
		if ((int(type) % 2) == 1)
		{
			ret += 1.0f; // add base planar value for nonflats
		}
	}
	else if (type > MAX_POWER_CHARGE)
	{
		if (type < FLAT_RAGE)
		{
			ret += 1.0f; // add base planar value for charge effects
		}
		else
		{
			if ((int(type) % 2) == 0)
			{
				ret += 1.0f; // add base planar value for nonflats
			}
		}
	}
	
	if (!modifiers[int(type)].empty())
	{
		std::vector<Modifier*>::const_iterator it;
		for (it = modifiers[int(type)].begin(); it != modifiers[int(type)].end(); it++)
		{
			ret += (*it)->value;
		}
	}

	return ret;
}

PlayerAttributes::PlayerAttributes(AttributeBuilder builder) : Attributes(builder),
base_item_rarity(builder.base_item_rarity),
current_rage(builder.current_rage),
current_stamina(builder.current_stamina),
base_rageRegen(builder.base_rageRegen),
base_staminaRegen(builder.base_staminaRegen),
base_maxRage(builder.base_maxRage),
base_maxStamina(builder.base_maxStamina)
{
	player = true;

	base_exp = 1000 * current_level;

	rageDegenTimer.start();
	staminaRegenTimer.start();
}

bool PlayerAttributes::update()
{
	bool ret;

	if (ret = (current_life > 0))
	{
		std::vector<Modifier*>::const_iterator it;
		for (int i = 0; i <= int(NONFLAT_ITEM_RARITY); i++)
		{
			for (it = modifiers[i].begin(); it != modifiers[i].end(); it++)
			{
				if (!((*it)->update())) // on false: clear & remove mod
				{
					(*it)->clear();
					modifiers[i].erase(it);
					it--;
				}
			}
		}

		// life regen
		addLife(lifeRegenTimer.ReadSec() * getLifeRegen());
		lifeRegenTimer.start();

		// degenerate rage : -2 rage / s
		addRage(rageDegenTimer.ReadSec() * getRageRegen());
		rageDegenTimer.start();

		// regen stamina: +1 stamina/s
		addStamina(staminaRegenTimer.ReadSec() * getStaminaRegen());
		staminaRegenTimer.start();
	}

	return ret;
}

void PlayerAttributes::reset(bool clearMods)
{
	// reset life
	current_life = getMaxLife();
	current_rage = getMaxRage();
	current_stamina = getMaxStamina();

	if (clearMods)
	{
		// clear mods
		for (int i = 0; i < modifiers.max_size(); i++)
		{
			modifiers[i].clear();
		}
	}
}

bool PlayerAttributes::addStamina(float val)
{
	bool ret;

	if (ret = (current_stamina + val > 0.0f))
	{
		float maxStamina = getMaxStamina();

		if (current_stamina + val < maxStamina)
		{
			current_stamina += val;
		}
		else
		{
			current_stamina = maxStamina;
		}
	}
	else
	{
		current_stamina = 0.0f;
	}

	return ret;
}

bool PlayerAttributes::addRage(float val)
{
	bool ret;

	if (ret = (current_rage + val > 0.0f))
	{
		float maxRage = getMaxRage();

		if (current_rage + val < maxRage)
		{
			current_rage += val;
		}
		else
		{
			current_rage = maxRage;
		}
	}
	else
	{
		current_rage = 0.0f;
	}

	return ret;
}

void PlayerAttributes::addExp(int exp)
{
	if (exp > 0.0f && current_level < 10)
	{
		if (exp + experience >= base_exp)
		{
			experience += exp - base_exp;
			setLevel(current_level + 1);
		}
		else
		{
			experience += exp;
		}
	}
}

bool PlayerAttributes::setLevel(int val)
{
	bool ret;

	if (ret = (val > 0 && val <= 10  && val != current_level))
	{
		if (hud != NULL) hud->levelChanged(val, current_level);

		if (current_level < val) availablePoints += 3;
		if (val == 5 || val == 10) availablePoints++;


		current_level = val;

		base_life += base_life + (0.7f * float(current_level));
		base_damage += base_damage + (0.3f * float(current_level));
		base_strength += base_strength + (0.3f * float(current_level));
		base_intelligence += base_intelligence + (0.3f * float(current_level));
		base_dexterity += base_dexterity + (0.3f * float(current_level));

		base_exp = 1000 * current_level;
		reset(false);
	}

	return ret;
}

float PlayerAttributes::getMaxRage()const
{
	return ((base_maxRage + getMod(FLAT_RAGE)) * getMod(NONFLAT_RAGE));
}

float PlayerAttributes::getMaxStamina()const
{
	return ((base_maxStamina + getMod(FLAT_STAMINA)) * getMod(NONFLAT_STAMINA));
}

float PlayerAttributes::getRageRegen()const
{
	return ((base_rageRegen + getMod(FLAT_RAGE_REGEN)) * getMod(NONFLAT_RAGE_REGEN));;
}

float PlayerAttributes::getStaminaRegen()const
{
	return ((base_staminaRegen + getMod(FLAT_STAMINA_REGEN)) * getMod(NONFLAT_STAMINA_REGEN));;
}

float PlayerAttributes::getItemRarity()const
{
	return ((base_item_rarity + getMod(FLAT_ITEM_RARITY)) * getMod(NONFLAT_ITEM_RARITY));
}

int PlayerAttributes::getMaxExp()const
{
	return int(base_exp);
}

float PlayerAttributes::getMod(modifierType type)const
{
	float ret = 0.0f;

	if (type < ENDURANCE_CHARGE)
	{
		if ((int(type) % 2) == 1)
		{
			ret += 1.0f; // add base planar value for nonflats
		}
	}
	else if (type > MAX_POWER_CHARGE)
	{
		if (type < FLAT_RAGE)
		{
			ret += 1.0f; // add base planar value for charge effects
		}
		else
		{
			if ((int(type) % 2) == 0)
			{
				ret += 1.0f; // add base planar value for nonflats
			}
		}
	}

	if (!modifiers[int(type)].empty())
	{
		std::vector<Modifier*>::const_iterator it;
		for (it = modifiers[int(type)].begin(); it != modifiers[int(type)].end(); it++)
		{
			ret += (*it)->value;
		}
	}

	
	/*------------------------------------------------------------------
	--------------------------------------------------------------------
	-----------------ADD INVENTORY DEPENDANCIES-------------------------
	---------------------for player only--------------------------------
	------------------------------------------------------------------*/

	return ret;
}