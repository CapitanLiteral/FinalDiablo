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
{}

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

	menuExpandOpened = { 1151, 576, 14, 25 };
	menuExpandClosed = { 1183, 576, 14, 25 };

	GuiImage* base = app->gui->addGuiImage({ 166, 425 }, { 166, 882, 309, 52 }, NULL, this);/**/

	app->gui->addGuiImage({ -132, -5 }, { 704, 730, 76, 53 }, base, this); // lifeback/**/
	app->gui->addGuiImage({ 364, -13 }, { 832, 730, 80, 53 }, base, this); // rageback/**/

	life = app->gui->addGuiImage({ -136, -39 }, { 576, 704, 79, 78 }, base, this);/**/
	rage = app->gui->addGuiImage({ 366, -39 }, { 448, 704, 78, 78 }, base, this);/**/
	stamina = app->gui->addGuiImage({ 27, 25 }, { 320, 703, 102, 18 }, base, this);/**/

	app->gui->addGuiImage({ -166, -41 }, { 0, 840, 116, 103 }, base, this); // lifefront/**/
	app->gui->addGuiImage({ 359, -50 }, { 524, 832, 116, 102 }, base, this); // ragefront/**/

	lifeLabel = app->gui->addGuiLabel("0", NULL, { 10, 5 }, NULL, FONT_WHITE, this);
	rageLabel = app->gui->addGuiLabel("0", NULL, { 10, 20 }, NULL, FONT_WHITE, this);

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

	lifeLabel->active = false;
	rageLabel->active = false;
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
	characterMenu = app->gui->addGuiImage({ 321, 0 }, { 1128, 588, 319, 430 }, NULL, this);
	i_inventory = app->gui->addGuiInventory({ 16, 255 }, { 1144, 843, 290, 116 }, 10, 4, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE, characterMenu, this);

	characterMenu->active = false;
	i_inventory->active = false;

	// Inventory
	inventoryMenu = app->gui->addGuiImage({ 0, 0 }, { 0, 0, 1, 1 }, NULL, this);

	// Tree
	treeMenu = app->gui->addGuiImage({ 0, 0 }, { 0, 0, 1, 1 }, NULL, this);

	// Pause Menu

	pauseMenu = app->gui->addGuiImage({ 0, 0 }, { 0, 0, 1, 1 }, NULL, this);
	p_exit = app->gui->addGuiImage({ 94, 200 }, { 94, 642, 534, 35 }, pauseMenu, this);/**/
	p_back = app->gui->addGuiImage({ 95, 270 }, { 629, 642, 438, 35 }, pauseMenu, this);/**/

	p_exit->interactable = true;
	p_back->interactable = true;

	// hide all tabs
	characterMenu->Desactivate();
	inventoryMenu->Desactivate();
	treeMenu->Desactivate();
	mapMenu->Desactivate();
	pauseMenu->Desactivate();







	exp = app->gui->addGuiImage({ 0, 0 }, { 530, 118, 102, 18 }, NULL, this);

	return true;
}

//Called before each loop iteration
bool Hud::preUpdate()
{
	// Stamina
	if ((app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_DOWN)
		|| (app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT))
	{
		staminaDorn->SetTextureRect({ 153, 301, 18, 22 });
	}
	else
	{
		staminaDorn->SetTextureRect({ 153, 280, 18, 22 });
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
				clearTabs();
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
				clearTabs();
			}
			else
			{
				hidePanel();
				inventoryMenu->Activate();
			}
		}
		else if(app->input->getKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			if (treeMenu->active)
			{
				clearTabs();
			}
			else
			{
				hidePanel();
				treeMenu->Activate();
			}
		}
		else if(app->input->getKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			clearTabs();
			characterMenu->Activate();
		}
		else if(app->input->getKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		{
			characterMenu->Activate();
		}
	}

	if (app->input->getKey(SDL_SCANCODE_1) == KEY_DOWN) useSlotItem(slot1);
	if (app->input->getKey(SDL_SCANCODE_2) == KEY_DOWN) useSlotItem(slot2);
	if (app->input->getKey(SDL_SCANCODE_3) == KEY_DOWN) useSlotItem(slot3);
	if (app->input->getKey(SDL_SCANCODE_4) == KEY_DOWN) useSlotItem(slot4);

	/* life & rage & stamina debug utility:
	if (character != NULL)
	{
	if (app->input->getKey(SDL_SCANCODE_5) == KEY_REPEAT) character->addLife(-20.0f);
	if (app->input->getKey(SDL_SCANCODE_6) == KEY_REPEAT) character->addRage(2.0f);
	if (app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) character->addStamina(-0.1f);
	}*/

	return true;
}

//Called each frame
bool Hud::update(float dt)
{
	if (exit) return false;

	// exit if no character assigned
	if (playerAtt == NULL)
	{
		LOG("NO PLAYER ASSIGNED TO HUD");
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

	playerAtt->addExp(5);

	// update exp image
	float maxExp = playerAtt->getMaxExp();
	if (playerAtt->getExp() >= maxExp)
	{
		exp->SetTextureRect(expRext);
	}
	else if (playerAtt->getExp() > 0.0f)
	{
		rect = expRext;

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

	if (lifeLabel->active)
	{
		text.assign("Life: ");
		text.append(NumberToString(int(playerAtt->getLife())));
		text.append("/");
		text.append(NumberToString(int(playerAtt->getMaxLife())));
		lifeLabel->SetText(text);
	}
	if (rageLabel->active)
	{
		text.assign("Rage: ");
		text.append(NumberToString(int(playerAtt->getRage())));
		text.append("/");
		text.append(NumberToString(int(playerAtt->getMaxRage())));
		rageLabel->SetText(text);
	}

	return true;
}

void Hud::OnEvent(GuiElement* element, GUI_Event even)
{
	if (character == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearTabs();
		hidePanel();
		characterMenu->Activate();
	}
	else if (inventory == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearTabs();
		hidePanel();
		inventoryMenu->Activate();
	}
	else if (tree == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		clearTabs();
		hidePanel();
		treeMenu->Activate();
	}
	else if (map == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		mapMenu->Activate();
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


	// Map


	// Pause Menu
	else if (p_exit == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		exit = false;
		app->sm->changeScene(4);
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
	/*
	//Potion
	GuiItem* new_item;
	iPoint coords[1] = { { 0, 0 } };
	new_item = new GuiItem(1, coords, { 2285, 799, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE });
	if (!inventory->AutomaticaddItem(new_item))
	{
		delete new_item;
		ret = false;
	}
	*/

	return ret;
}

void Hud::clearTabs()
{
	if (characterMenu->active) characterMenu->Desactivate();
	if (inventoryMenu->active) inventoryMenu->Desactivate();
	if (treeMenu->active) treeMenu->Desactivate();
	if (mapMenu->active) mapMenu->Desactivate();
	if (pauseMenu->active) pauseMenu->Desactivate();
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