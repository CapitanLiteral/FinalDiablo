#include "Hud.h"
#include "App.h"
#include "Gui.h"
#include "Attributes.h"
#include "Fonts.h"
#include "Input.h"
#include "SceneManager.h"
#include "p2Log.h"
#include <sstream>
#include <string>

template <typename T>
std::string NumberToString(T Number)
{
	ostringstream ss;
	ss << Number;
	std::string ret;
	ret.assign(ss.str());
	return ret;
}

//Constructor
Hud::Hud() : Module(), exit(false)
{
	t_1Mod = new Modifier(0.0f, FLAT_ARMOR);
	t_2Mod = new Modifier(0.0f, NONFLAT_DAMAGE);
	t_3Mod = new Modifier(0.0f, NONFLAT_LIFE);
	t_4Mod = new Modifier(0.0f, NONFLAT_LIFE_LEACH);
	t_5Mod = new Modifier(0.0f, NONFLAT_LIFE_REGEN);
	t_6Mod = new Modifier(0.0f, FLAT_CRIT_CHANCE);
}

Hud::~Hud()
{}

bool Hud::start()
{
	// Base
	lifePos = { -136, -39 };
	ragePos = { 366, -39 };

	lifeRect = { 576, 704, 79, 78 };
	rageRect = { 448, 704, 78, 78 };
	staminaRect = { 320, 703, 102, 18 };
	expRect = { 173, 867, 121, 4 };

	menuExpandOpened = { 1151, 576, 14, 25 };
	menuExpandClosed = { 1183, 576, 14, 25 };

	GuiImage* base = app->gui->addGuiImage({ 166, 425 }, { 166, 882, 309, 52 }, NULL, this);/**/

	stamina = app->gui->addGuiImage({ 27, 25 }, { 320, 703, 102, 18 }, base, this);/**/
	staminaDorn = app->gui->addGuiImage({ 9, 24 }, { 1088, 576, 16, 20 }, base, this);/**/
	menuExpand = app->gui->addGuiImage({ 145, 13 }, menuExpandOpened, base, this);/**/

	panel = app->gui->addGuiImage({ 98, -25 }, { 960, 576, 110, 25 }, base, this);/**/
	character = app->gui->addGuiImage({ 3, 3 }, { 1344, 448, 20, 19 }, panel, this);/**/
	inventory = app->gui->addGuiImage({ 24, 3 }, { 1376, 448, 20, 19 }, panel, this);/**/
	tree = app->gui->addGuiImage({ 45, 3 }, { 1344, 480, 20, 19 }, panel, this);/**/
	map = app->gui->addGuiImage({ 66, 3 }, { 1376, 480, 20, 19 }, panel, this);/**/
	pause = app->gui->addGuiImage({ 87, 3 }, { 1408, 448, 20, 19 }, panel, this);/**/

	app->gui->addGuiImage({ -50, 5 }, { 960, 832, 50, 47 }, base, this); // leftSkill/**/
	current_skill = basic_attack = app->gui->addGuiImage({ 308, 5 }, { 960, 832, 50, 47 }, base, this);/**/
	frenzy = app->gui->addGuiImage({ 162, -70 }, { 896, 832, 50, 47 }, base, this);/**/
	whack = app->gui->addGuiImage({ 233, -70 }, { 832, 832, 50, 47 }, base, this);/**/
	growl = app->gui->addGuiImage({ 305, -70 }, { 832, 832, 50, 47 }, base, this);/**/

	character->interactable = true;
	inventory->interactable = true;
	tree->interactable = true;
	map->interactable = true;
	pause->interactable = true;
	staminaDorn->interactable = true;
	menuExpand->interactable = true;
	basic_attack->interactable = true;
	frenzy->interactable = true;
	whack->interactable = true;
	growl->interactable = true;
	
	frenzy->active = false;
	whack->active = false;
	growl->active = false;

	slot1 = app->gui->addGuiInventory({ 176, 9 }, { 342, 896, 30, 30 }, 1, 1, 30, 30, base, this);
	slot2 = app->gui->addGuiInventory({ 207, 9 }, { 342, 896, 30, 30 }, 1, 1, 30, 30, base, this);
	slot3 = app->gui->addGuiInventory({ 238, 9 }, { 342, 896, 30, 30 }, 1, 1, 30, 30, base, this);
	slot4 = app->gui->addGuiInventory({ 269, 9 }, { 342, 896, 30, 30 }, 1, 1, 30, 30, base, this);

	// Map - loads earlier to be in background
	mapMenu = app->gui->addGuiImage({ 0, 0 }, { 0, 0, 1, 1 }, NULL, this);

	// Character
	characterMenu = app->gui->addGuiImage({ 0, 0 }, { 2047, 1, 289, 430 }, NULL, this);
	levelLabel = app->gui->addGuiLabel("0", NULL, { 10, 30 }, characterMenu, FONT_WHITE, this);
	expLabel = app->gui->addGuiLabel("0", NULL, { 10, 60 }, characterMenu, FONT_WHITE, this);
	lifeLabel = app->gui->addGuiLabel("0", NULL, { 10, 75 }, characterMenu, FONT_WHITE, this);
	rageLabel = app->gui->addGuiLabel("0", NULL, { 10, 90 }, characterMenu, FONT_WHITE, this);
	strengthLabel = app->gui->addGuiLabel("0", NULL, { 10, 120 }, characterMenu, FONT_WHITE, this);
	intelligenceLabel = app->gui->addGuiLabel("0", NULL, { 10, 135 }, characterMenu, FONT_WHITE, this);
	dexterityLabel = app->gui->addGuiLabel("0", NULL, { 10, 150 }, characterMenu, FONT_WHITE, this);
	lifeRegenLabel = app->gui->addGuiLabel("0", NULL, { 10, 180 }, characterMenu, FONT_WHITE, this);
	armorLabel = app->gui->addGuiLabel("0", NULL, { 10, 210 }, characterMenu, FONT_WHITE, this);
	lifeLeachLabel = app->gui->addGuiLabel("0", NULL, { 10, 240 }, characterMenu, FONT_WHITE, this);
	critChanceLabel = app->gui->addGuiLabel("0", NULL, { 10, 270 }, characterMenu, FONT_WHITE, this);

	// Inventory
	inventoryMenu = app->gui->addGuiImage({ 321, 0 }, { 832, 1, 319, 430 }, NULL, this);
	//i_inventory = app->gui->addGuiInventory({ 16, 255 }, { 1144, 843, 290, 116 }, 10, 4, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE, characterMenu, this);

	// Tree
	treeMenu = app->gui->addGuiImage({ 321, 0 }, { 1216, 1, 321, 429 }, NULL, this);
	t_1 = app->gui->addGuiImage({ 46, 37 }, { 960, 449, 48, 48 }, treeMenu, this); // armor
	t_2 = app->gui->addGuiImage({ 114, 35 }, { 832, 448, 48, 48 }, treeMenu, this); // damage
	t_3 = app->gui->addGuiImage({ 45, 172 }, { 1009, 498, 48, 48 }, treeMenu, this); // life
	t_4 = app->gui->addGuiImage({ 114, 242 }, { 832, 497, 48, 48 }, treeMenu, this); // life leach
	t_5 = app->gui->addGuiImage({ 45, 310 }, { 881, 497, 48, 48 }, treeMenu, this); // life regen
	t_6 = app->gui->addGuiImage({ 114, 310 }, { 1009, 449, 48, 48 }, treeMenu, this); // attack speed
	t_exit = app->gui->addGuiImage({ 168, 382 }, { 1280, 576, 38, 38 }, treeMenu, this);

	points = app->gui->addGuiLabel("0", NULL, { 265, 62 }, treeMenu, FONT_WHITE, this);

	t_1Label = app->gui->addGuiLabel("+100 Armor each", NULL, { 321 + 45, 37 - 17 }, NULL, FONT_WHITE, this);
	t_2Label = app->gui->addGuiLabel("+2% Damage each", NULL, { 321 + 114, 35 - 17 }, NULL, FONT_WHITE, this);
	t_3Label = app->gui->addGuiLabel("+5% Life each", NULL, { 321 + 45, 172 - 17 }, NULL, FONT_WHITE, this);
	t_4Label = app->gui->addGuiLabel("+2% Life Leach each", NULL, { 321 + 114, 242 - 17 }, NULL, FONT_WHITE, this);
	t_5Label = app->gui->addGuiLabel("+5% Life Regen each.", NULL, { 321 + 45, 310 - 17 }, NULL, FONT_WHITE, this);
	t_6Label = app->gui->addGuiLabel("+1% Crit. Chance each", NULL, { 321 + 114, 310 - 17 }, NULL, FONT_WHITE, this);

	t_1QuantLabel = app->gui->addGuiLabel("0", NULL, { 44 + 46, 44 + 37 }, treeMenu, FONT_WHITE, this);
	t_2QuantLabel = app->gui->addGuiLabel("0", NULL, { 44 + 114, 44 + 35 }, treeMenu, FONT_WHITE, this);
	t_3QuantLabel = app->gui->addGuiLabel("0", NULL, { 44 + 45, 44 + 172 }, treeMenu, FONT_WHITE, this);
	t_4QuantLabel = app->gui->addGuiLabel("0", NULL, { 44 + 114, 44 + 242 }, treeMenu, FONT_WHITE, this);
	t_5QuantLabel = app->gui->addGuiLabel("0", NULL, { 44 + 45, 44 + 310 }, treeMenu, FONT_WHITE, this);
	t_6QuantLabel = app->gui->addGuiLabel("0", NULL, { 44 + 114, 44 + 310 }, treeMenu, FONT_WHITE, this);

	t_1->interactable = true;
	t_2->interactable = true;
	t_3->interactable = true;
	t_4->interactable = true;
	t_5->interactable = true;
	t_6->interactable = true;

	t_1Label->active = false;
	t_2Label->active = false;
	t_3Label->active = false;
	t_4Label->active = false;
	t_5Label->active = false;
	t_6Label->active = false;

	// tree mods
	playerAtt->addMod(t_1Mod);
	playerAtt->addMod(t_2Mod);
	playerAtt->addMod(t_3Mod);
	playerAtt->addMod(t_4Mod);
	playerAtt->addMod(t_5Mod);
	playerAtt->addMod(t_6Mod);


	// Pause Menu

	pauseMenu = app->gui->addGuiImage({ 0, 0 }, { 0, 0, 1, 1 }, NULL, this);
	p_exit = app->gui->addGuiImage({ 54, 200 }, { 94, 642, 534, 35 }, pauseMenu, this);/**/
	p_back = app->gui->addGuiImage({ 75, 270 }, { 629, 642, 438, 35 }, pauseMenu, this);/**/

	p_exit->interactable = true;
	p_back->interactable = true;

	// hide all tabs
	characterMenu->Desactivate();
	inventoryMenu->Desactivate();
	treeMenu->Desactivate();
	mapMenu->Desactivate();
	pauseMenu->Desactivate();


	// above menus

	app->gui->addGuiImage({ -132, -5 }, { 704, 730, 76, 53 }, base, this); // lifeback/**/
	app->gui->addGuiImage({ 364, -13 }, { 832, 730, 80, 53 }, base, this); // rageback/**/

	life = app->gui->addGuiImage({ -136, -39 }, { 576, 704, 79, 78 }, base, this);/**/
	rage = app->gui->addGuiImage({ 366, -39 }, { 448, 704, 78, 78 }, base, this);/**/

	app->gui->addGuiImage({ -166, -41 }, { 0, 840, 116, 103 }, base, this); // lifefront/**/
	app->gui->addGuiImage({ 359, -50 }, { 524, 832, 116, 102 }, base, this); // ragefront/**/

	exp = app->gui->addGuiImage({ 10, 12 }, { 173, 867, 121, 4 }, base, this);

	return true;
}

//Called before each loop iteration
bool Hud::preUpdate()
{
	// Stamina
	if ((app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_DOWN)
		|| (app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT))
	{
		staminaDorn->SetTextureRect({ 1120, 576, 18, 22 });
	}
	else
	{
		staminaDorn->SetTextureRect({ 1088, 576, 18, 22 });
	}

	// Pause menu
	if (app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (pauseMenu->active)
		{
			activatePanel();
			pauseMenu->Desactivate();
		}
		else if (characterMenu->active
			|| inventoryMenu->active
			|| treeMenu->active
			|| mapMenu->active)
		{
			clearTabs();
		}
		else
		{
			panel->Desactivate();
			pauseMenu->Activate();
		}
	}
	else
	{
		if (app->input->getKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			if (characterMenu->active)
			{
				characterMenu->Desactivate();
				if (!inventoryMenu->active && !treeMenu->active) activatePanel();
			}
			else
			{
				hidePanel();
				characterMenu->Activate();
			}
		}
		else if(app->input->getKey(SDL_SCANCODE_I) == KEY_DOWN)
		{
			if (inventoryMenu->active)
			{
				inventoryMenu->Desactivate();
				if (!characterMenu->active) activatePanel();
			}
			else
			{
				clearRightTabs();
				hidePanel();
				inventoryMenu->Activate();
			}
		}
		else if(app->input->getKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			if (treeMenu->active)
			{
				treeMenu->Desactivate();
				if (!characterMenu->active) activatePanel();

				t_1Label->active = false;
				t_2Label->active = false;
				t_3Label->active = false;
				t_4Label->active = false;
				t_5Label->active = false;
				t_6Label->active = false;
			}
			else
			{
				clearRightTabs();
				hidePanel();
				treeMenu->Activate();

				t_1Label->active = false;
				t_2Label->active = false;
				t_3Label->active = false;
				t_4Label->active = false;
				t_5Label->active = false;
				t_6Label->active = false;
			}
		}
		else if(app->input->getKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		{
			mapMenu->active ? mapMenu->Desactivate() : mapMenu->Activate();
		}
	}

	if (app->input->getKey(SDL_SCANCODE_1) == KEY_DOWN) useSlotItem(slot1);
	if (app->input->getKey(SDL_SCANCODE_2) == KEY_DOWN) useSlotItem(slot2);
	if (app->input->getKey(SDL_SCANCODE_3) == KEY_DOWN) useSlotItem(slot3);
	if (app->input->getKey(SDL_SCANCODE_4) == KEY_DOWN) useSlotItem(slot4);

	// life & rage & stamina debug utility:
	if (playerAtt != NULL)
	{
		if (app->input->getKey(SDL_SCANCODE_5) == KEY_REPEAT) playerAtt->addLife(-20.0f);
		if (app->input->getKey(SDL_SCANCODE_6) == KEY_REPEAT) playerAtt->addRage(2.0f);
		if (app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) playerAtt->addStamina(-0.1f);   
	}

	return true;
}

//Called each frame
bool Hud::update(float dt)
{
	if (exit) return false;

	// exit if no character assigned
	if (playerAtt == NULL)
	{
		if (app->debug) LOG("NO PLAYER ASSIGNED TO HUD");
		return true;
	}

	SDL_Rect rect;
	iPoint pos;
	float dif;
	int inc;

	// update life image
	float maxLife = playerAtt->getMaxLife();
	if (playerAtt->getLife() >= maxLife)
	{
		life->SetLocalPosition(lifePos);
		life->SetTextureRect(lifeRect);
	}
	else if (playerAtt->getLife() > 0.0f)
	{
		rect = lifeRect;
		pos = lifePos;

		dif = maxLife - playerAtt->getLife();
		dif *= rect.h;
		dif /= maxLife;

		inc = int(dif);
		rect.y += inc;
		rect.h -= inc;
		pos.y += inc;

		life->SetLocalPosition(pos);
		life->SetTextureRect(rect);
	}
	else
	{
		life->SetLocalPosition(lifePos);
		life->SetTextureRect({ 0, 0, 0, 0 });
	}


	// update rage image
	float maxRage = playerAtt->getMaxRage();
	if (playerAtt->getRage() >= maxRage)
	{
		rage->SetLocalPosition(ragePos);
		rage->SetTextureRect(rageRect);
	}
	else if (playerAtt->getRage() >= 0.0f)
	{
		rect = rageRect;
		pos = ragePos;

		dif = maxRage - playerAtt->getRage();
		dif *= rect.h;
		dif /= maxRage;

		inc = int(dif);
		rect.y += inc;
		rect.h -= inc;
		pos.y += inc;

		rage->SetLocalPosition(pos);
		rage->SetTextureRect(rect);
	}
	else
	{
		rage->SetLocalPosition(ragePos);
		rage->SetTextureRect({ 0, 0, 0, 0 });
	}

	// update stamina image
	float maxStamina = playerAtt->getMaxStamina();
	if (playerAtt->getStamina() >= maxStamina)
	{
		stamina->SetTextureRect(staminaRect);
	}
	else if (playerAtt->getStamina() > 0.0f)
	{
		rect = staminaRect;

		dif = maxStamina - playerAtt->getStamina();
		dif *= rect.w;
		dif /= maxStamina;

		rect.w -= int(dif);

		stamina->SetTextureRect(rect);
	}
	else
	{
		stamina->SetTextureRect({ 0, 0, 0, 0 });
	}

	//playerAtt->addExp(5 * playerAtt->getLevel());
	
	// update exp image
	float maxExp = playerAtt->getMaxExp();
	if (playerAtt->getExp() >= maxExp)
	{
		exp->SetTextureRect(expRect);
	}
	else if (playerAtt->getExp() > 0.0f)
	{
		rect = expRect;

		dif = maxExp - playerAtt->getExp();
		dif *= rect.w;
		dif /= maxExp;

		rect.w -= int(dif);

		exp->SetTextureRect(rect);
	}
	else
	{
		exp->SetTextureRect({ 0, 0, 0, 0 });
	}

	std::string text;

	// Character

	if (characterMenu->active)
	{
		text.assign("Lvl: ");
		text.append(NumberToString(int(playerAtt->getLevel())));
		levelLabel->SetText(text);

		text.assign("Exp: ");
		text.append(NumberToString(int(playerAtt->getExp())));
		text.append("/");
		text.append(NumberToString(int(playerAtt->getMaxExp())));
		expLabel->SetText(text);
		text.assign("Life: ");
		text.append(NumberToString(int(playerAtt->getLife())));
		text.append("/");
		text.append(NumberToString(int(playerAtt->getMaxLife())));
		lifeLabel->SetText(text);
		text.assign("Rage: ");
		text.append(NumberToString(int(playerAtt->getRage())));
		text.append("/");
		text.append(NumberToString(int(playerAtt->getMaxRage())));
		rageLabel->SetText(text);

		text.assign("Strength: ");
		text.append(NumberToString(int(playerAtt->getStrength())));
		strengthLabel->SetText(text);
		text.assign("Intelligence: ");
		text.append(NumberToString(int(playerAtt->getIntelligence())));
		intelligenceLabel->SetText(text);
		text.assign("Dexterity: ");
		text.append(NumberToString(int(playerAtt->getDexterity())));
		dexterityLabel->SetText(text);

		text.assign("Life Regen: ");
		text.append(NumberToString(int(playerAtt->getLifeRegen())));
		lifeRegenLabel->SetText(text);
		text.assign("Armor: ");
		text.append(NumberToString(int(playerAtt->getArmor())));
		armorLabel->SetText(text);
		text.assign("Life Leach: ");
		text.append(NumberToString(int(playerAtt->getLifeLeach() * 100)));
		text.append("%");
		lifeLeachLabel->SetText(text);
		text.assign("Crit Chance: ");
		text.append(NumberToString(int(playerAtt->getCritChance() * 100)));
		text.append("%");
		critChanceLabel->SetText(text);
	}


	// Tree

	if (treeMenu->active)
	{
		text.assign(NumberToString(int(t_1Mod->value / 100.f)));
		t_1QuantLabel->SetText(text);
		text.assign(NumberToString(int(t_2Mod->value / 0.02f)));
		t_2QuantLabel->SetText(text);
		text.assign(NumberToString(int(t_3Mod->value / 0.05f)));
		t_3QuantLabel->SetText(text);
		text.assign(NumberToString(int(t_4Mod->value / 0.02f)));
		t_4QuantLabel->SetText(text);
		text.assign(NumberToString(int(t_5Mod->value / 0.05f)));
		t_5QuantLabel->SetText(text);
		text.assign(NumberToString(int(t_6Mod->value / 0.01f)));
		t_6QuantLabel->SetText(text);
		text.assign(NumberToString(playerAtt->availablePoints));
		points->SetText(text);
	}
	return true;
}

void Hud::OnEvent(GuiElement* element, GUI_Event even)
{
	if (character == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearLeftTabs();
		hidePanel();
		characterMenu->Activate();
	}
	else if (inventory == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearRightTabs();
		hidePanel();
		inventoryMenu->Activate();
	}
	else if (tree == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearRightTabs();
		hidePanel();
		treeMenu->Activate();
	}
	else if (map == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		mapMenu->active ? mapMenu->Desactivate() : mapMenu->Activate();
	}
	else if (pause == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearTabs();
		hidePanel();
		pauseMenu->Activate();
	}
	else if (menuExpand == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		if (panel->active)
		{
			hidePanel();
		}
		else
		{
			activatePanel();
		}
	}

	// Character


	// Inventory


	// Tree
	else if (t_1 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (playerAtt->availablePoints > 0 && t_1Mod->value < 500.0f)
			{
				t_1Mod->value += 100.0f;
				playerAtt->availablePoints--;
			}
			break;
		}
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
		{
			if (t_1Mod->value > 0.0f && t_3Mod->value <= 0.0f)
			{
				t_1Mod->value -= 100.0f;
				playerAtt->availablePoints++;
			}
			break;
		}
		case EVENT_MOUSE_ENTER: t_1Label->Activate(); break;
		case EVENT_MOUSE_EXIT: t_1Label->Desactivate(); break;
		}
	}
	else if (t_2 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (playerAtt->availablePoints > 0 && t_2Mod->value < 0.09f)
			{
				t_2Mod->value += 0.02f;
				playerAtt->availablePoints--;
			}
			break;
		}
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
		{
			if (t_2Mod->value > 0.0f && t_4Mod->value <= 0.0f)
			{
				t_2Mod->value -= 0.02f;
				playerAtt->availablePoints++;
			}
			break;
		}
		case EVENT_MOUSE_ENTER: t_2Label->Activate(); break;
		case EVENT_MOUSE_EXIT: t_2Label->Desactivate(); break;
		}
	}
	else if (t_3 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (playerAtt->availablePoints > 0 && t_3Mod->value < 0.25f && t_1Mod->value >= 500.0f)
			{
				t_3Mod->value += 0.05f;
				playerAtt->availablePoints--;
			}
			break;
		}
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
		{
			if (t_3Mod->value > 0.0f && t_5Mod->value <= 0.0f)
			{
				t_3Mod->value -= 0.05f;
				playerAtt->availablePoints++;
			}
			break;
		}
		case EVENT_MOUSE_ENTER: t_3Label->Activate(); break;
		case EVENT_MOUSE_EXIT: t_3Label->Desactivate(); break;
		}
	}
	else if (t_4 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (playerAtt->availablePoints > 0 && t_4Mod->value < 0.09f && t_2Mod->value >= 0.09f)
			{
				t_4Mod->value += 0.02f;
				playerAtt->availablePoints--;
			}
			break;
		}
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
		{
			if (t_4Mod->value > 0.0f && t_6Mod->value <= 0.0f)
			{
				t_4Mod->value -= 0.02f;
				playerAtt->availablePoints++;
			}
			break;
		}
		case EVENT_MOUSE_ENTER: t_4Label->Activate(); break;
		case EVENT_MOUSE_EXIT: t_4Label->Desactivate(); break;
		}
	}
	else if (t_5 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (playerAtt->availablePoints > 0 && t_5Mod->value < 0.25f && t_3Mod->value >= 0.25f)
			{
				t_5Mod->value += 0.05f;
				playerAtt->availablePoints--;
			}
			break;
		}
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
		{
			if (t_5Mod->value > 0.0f)
			{
				t_5Mod->value -= 0.05f;
				playerAtt->availablePoints++;
			}
			break;
		}
		case EVENT_MOUSE_ENTER: t_5Label->Activate(); break;
		case EVENT_MOUSE_EXIT: t_5Label->Desactivate(); break;
		}
	}
	else if (t_6 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (playerAtt->availablePoints > 0 && t_6Mod->value <= 0.04f && t_4Mod->value >= 0.09f)
			{
				t_6Mod->value += 0.01f;
				playerAtt->availablePoints--;
			}
			break;
		}
		case EVENT_MOUSE_RIGHTCLICK_DOWN:
		{
			if (t_6Mod->value > 0.0f)
			{
				t_6Mod->value -= 0.01f;
				playerAtt->availablePoints++;
			}
			break;
		}
		case EVENT_MOUSE_ENTER: t_6Label->Activate(); break;
		case EVENT_MOUSE_EXIT: t_6Label->Desactivate(); break;
		}
	}
	else if (t_exit == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearTabs();
	}

	// Map


	// Pause Menu
	else if (p_exit == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		exit = true;
	}
	else if (p_back == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		activatePanel();
		pauseMenu->Desactivate();
	}
}

bool Hud::addItem()
{
	bool ret = true;
	

	return ret;
}

void Hud::clearTabs()
{
	if (characterMenu->active) characterMenu->Desactivate();
	if (inventoryMenu->active) inventoryMenu->Desactivate();
	if (treeMenu->active) treeMenu->Desactivate();
	if (pauseMenu->active) pauseMenu->Desactivate();

	t_1Label->active = false;
	t_2Label->active = false;
	t_3Label->active = false;
	t_4Label->active = false;
	t_5Label->active = false;
	t_6Label->active = false;

	activatePanel();
}
void Hud::clearLeftTabs()
{
	if (characterMenu->active) characterMenu->Desactivate();
	activatePanel();
}
void Hud::clearRightTabs()
{
	if (inventoryMenu->active) inventoryMenu->Desactivate();
	if (treeMenu->active) treeMenu->Desactivate();

	t_1Label->active = false;
	t_2Label->active = false;
	t_3Label->active = false;
	t_4Label->active = false;
	t_5Label->active = false;
	t_6Label->active = false;

	activatePanel();
}

bool Hud::useSlotItem(GuiInventory* slot)
{
	bool ret;

	if (ret = ((slot != NULL) && (!slot->items.empty())))
	{
		GuiItem* item = *(slot->items.begin());
		item->Effect();
		item->FreeSlots();
		slot->items.remove(item);
		RELEASE(item);
	}

	return ret;
}

void Hud::hidePanel()
{
	panel->Desactivate();
	menuExpand->SetTextureRect(menuExpandClosed);
}

void Hud::activatePanel()
{
	panel->Activate();
	menuExpand->SetTextureRect(menuExpandOpened);
}