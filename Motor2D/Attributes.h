#ifndef __ATTRIBUTES_H__
#define __ATTRIBUTES_H__

#include <string>
#include <list>
#include <vector>
#include <array>
#include "Timer.h"
#include "Hud.h"

enum modifierType
{
	// Base
	FLAT_LIFE = 0,
	NONFLAT_LIFE,
	FLAT_STRENGTH,
	NONFLAT_STRENGTH,
	FLAT_INTELLIGENCE,
	NONFLAT_INTELLIGENCE,
	FLAT_DEXTERITY,
	NONFLAT_DEXTERITY,

	// Offensive
	FLAT_ATTACK_SPEED,
	NONFLAT_ATTACK_SPEED,
	FLAT_CAST_SPEED,
	NONFLAT_CAST_SPEED,
	FLAT_DAMAGE,
	NONFLAT_DAMAGE,
	FLAT_ACCURACY,
	NONFLAT_ACCURACY,
	FLAT_CRIT_CHANCE,
	NONFLAT_CRIT_CHANCE,
	FLAT_CRIT_MULT,
	NONFLAT_CRIT_MULT,
	FLAT_LIFE_LEACH,
	NONFLAT_LIFE_LEACH,

	// Defensive
	FLAT_ARMOR,
	NONFLAT_ARMOR,
	FLAT_EVASION,
	NONFLAT_EVASION,
	FLAT_BLOCK_CHANCE,
	NONFLAT_BLOCK_CHANCE,

	// Utility
	FLAT_LIFE_REGEN,
	NONFLAT_LIFE_REGEN,
	FLAT_MOVEMENT_SPEED,
	NONFLAT_MOVEMENT_SPEED,

	// Charges
	ENDURANCE_CHARGE,
	FRENZY_CHARGE,
	POWER_CHARGE,

	MAX_ENDURANCE_CHARGE,
	MAX_FRENZY_CHARGE,
	MAX_POWER_CHARGE,

	ENDURANCE_CHARGE_DAMAGE_REDUCTION_INCREASE,
	FRENZY_CHARGE_ATTACK_SPEED_INCREASE,
	FRENZY_CHARGE_CAST_SPEED_INCREASE,
	FRENZY_CHARGE_DAMAGE_INCREASE,
	POWER_CHARGE_CRIT_CHANCE_INCREASE,

	// Player-only
	FLAT_RAGE,
	NONFLAT_RAGE,
	FLAT_STAMINA,
	NONFLAT_STAMINA,
	FLAT_ITEM_RARITY,
	NONFLAT_ITEM_RARITY
};


// Modificador - se a�ade a los atributos y son responsables de
// cambiar de forma temporal o constante los valores de los atributos
class Modifier
{
public:
	Modifier(float value, modifierType type) : value(value), type(type), remove(false){}
	~Modifier(){}
	virtual bool update(){ return remove; }
	virtual void clear(){}
	modifierType type;
	float value;
	bool remove;
};


class TempMod : public Modifier
{
public:

	TempMod(float time, float value, modifierType type) : time(time), Modifier(value, type){ timer.start(); }
	~TempMod(){}
	bool update(){ return (remove ? remove : (timer.ReadSec() < time)); }
	Timer timer;
	float time;
};


struct AttributeBuilder
{
	// Base General Attributes
	float	base_life;
	float	base_rage;
	float	base_stamina;
	float	base_strength;
	float	base_intelligence;
	float	base_dexterity;

	// Base Offensive Attributes
	float	base_attackSpeed;
	float	base_castSpeed;
	float	base_damage;
	float	base_accuracy;
	float	base_critChance = 5.0f;
	float	base_critMultiplier = 200.0f;

	// Base Deffensive Attributes
	float	base_armor;
	float	base_evasionRating;
	float	base_lifeRegen;
	float	base_blockChance;
	float	base_movementSpeed;

	// Base Utility Attributes
	float	base_item_rarity = 0.1f;
	float	base_staminaRegen;

	// Base Charges Attributes
	float	base_maxEnduranceCharges = 3;
	float	base_maxFrenzyCharges = 3;
	float	base_maxPowerCharges = 3;
	float	base_endranceChargeDamageReduction = 0.04f;
	float	base_frenzyChargeAttackSpeedIncrease = 0.04f;
	float	base_frenzyChargeCastSpeedIncrease = 0.04f;
	float	base_frenzyChargeDamageIncrease = 0.04f;
	float	base_powerChargeCritChanceIncrease = 0.5f;

	// Starting values
	int		current_level;
	float	current_life;
	float	current_rage;
	float	current_stamina;

	float	rageRegen = -2.0f;
	float	staminaRegen = 1.0f;
	float	maxRage = 100.0f;
	float	maxStamina = 30.0f;
};

class Attributes
{
public:
	Attributes(AttributeBuilder builder);
	~Attributes();

	// Add a modifier to the attributes
	bool addMod(Modifier* mod);

	// Updates the attributes that change over time
	// and checks for any changes in its mods
	// Returns true if still alive
	virtual bool update();

	virtual void reset();

	// When damaging, call this methos with the attacker's attributs and attack
	// Returns true if still alive
	bool damage(Attributes* attacker, int attackType);

	// Immediate Getters
	float getLevel()const	{ return current_level; }
	float getLife()const	{ return current_life; }

	// Adders / setters
	bool addLife(float val);
	virtual bool setLevel(int val);

	// Getters
	float getMaxLife()const;
	float getStrength()const;
	float getIntelligence()const;
	float getDexterity()const;

	float getAttackSpeed()const;
	float getCastSpeed()const;
	float getDamage()const;
	float getAccuracy()const;
	float getCritChance()const;
	float getCritMult()const;

	float getArmor()const;
	float getEvasionRating()const;
	float getLifeRegen()const;
	float getBlockChance()const;
	float getMovementSpeed()const;
	float getLifeLeach()const;

	float getEnduranceCharges()const;
	float getFrenzyCharges()const;
	float getPowerCharges()const;

	float getMaxEnduranceCharges()const;
	float getMaxFrenzyCharges()const;
	float getMaxPowerCharges()const;

	float getEnduranceChargeDamageReduction()const;
	float getFrenzyChargeAttackSpeedIncrease()const;
	float getFrenzyChargeCastSpeedIncrease()const;
	float getFrenzyChargeDamageIncrease()const;
	float getPowerChargeCritChanceIncrease()const;

protected:

	// Mod Getter - gets flat and nonflat modifier values
	virtual float getMod(modifierType type)const;

public:

	// Nombre: para usar en la interfaz
	std::string characterName;

	// array with modifiers
	std::array<std::vector<Modifier*>, int(NONFLAT_ITEM_RARITY) + 1> modifiers;

protected:

	int* x = NULL;
	int* y = NULL;
	Hud* hud = NULL;

	// Base General Attributes
	float	base_life;
	float	base_strength;
	float	base_intelligence;
	float	base_dexterity;

	// Base Offensive Attributes
	float	base_attackSpeed;
	float	base_castSpeed;
	float	base_damage;
	float	base_accuracy;
	float	base_critChance = 5.0f;
	float	base_critMultiplier = 200.0f;

	// Base Deffensive Attributes
	float	base_armor;
	int		base_evasionRating;
	float	base_lifeRegen;
	float	base_blockChance;
	float	base_movementSpeed;

	// Base Charges Attributes
	int		base_maxEnduranceCharges = 3;
	int		base_maxFrenzyCharges = 3;
	int		base_maxPowerCharges = 3;
	float	base_endranceChargeDamageReduction = 0.04f;
	float	base_frenzyChargeAttackSpeedIncrease = 0.04f;
	float	base_frenzyChargeCastSpeedIncrease = 0.04f;
	float	base_frenzyChargeDamageIncrease = 0.04f;
	float	base_powerChargeCritChanceIncrease = 0.5f;

	// Immediate values
	int		current_level;
	float	current_life;

	// Life Regen timer
	Timer lifeRegenTimer;
};

/*
Strength: +1  increased life for each 2 strenght
		  +2% increased damage for each 5 streangth
		  +5% increased life regern for each 5 strength

Intelligence: +1% increased crit chance for each 5 intelligence
			  +1% increased crit multiplier for each 2 intelligence
			  +1% increased cast speed for each 2 intelligence

Dexterity: +2 increased accuracy for each 1 dexterity
		   +1% increased evasion rating for 5 dexterity
		   +1% increased attack speed for 2 dexterity
*/

class PlayerAttributes : public Attributes
{

public:

	PlayerAttributes(AttributeBuilder builder);

	bool update();

	void reset();

	// Immediate Getters
	float getRage()const	{ return current_rage; }
	float getStamina()const	{ return current_stamina; }

	// Adders / setters
	bool addStamina(float val);
	bool addRage(float val);
	bool setLevel(int val);

	// Getters
	float getMaxRage()const;
	float getMaxStamina()const;
	float getStaminaRegen()const;
	float getItemRarity()const;

protected:

	// Mod Getter - gets flat and nonflat modifier values
	float getMod(modifierType type)const;

private:

	// Base General Attributes
	float	base_rage;
	float	base_stamina;

	// Base Utility Attributes
	float	base_item_rarity = 0.1f;

	// Immediate values
	float	current_rage;
	float	current_stamina;

	float	rageRegen = -2.0f;
	float	staminaRegen = 1.0f;
	float	maxRage;
	float	maxStamina;

	// Timers
	Timer	rageDegenTimer;
	Timer	staminaRegenTimer;
};









#endif
