#include "Hud.h"
#include "App.h"
#include "Gui.h"
#include "Attributes.h"
#include "Fonts.h"
#include "Input.h"
#include "SceneManager.h"

//Constructor
Hud::Hud() : Module()
{}

Hud::~Hud()
{}

bool Hud::start()
{
	// Base
	lifePos = { -136, -44 };
	ragePos = { 366, -44 };

	lifeRect = { 371, 118, 79, 78 };
	rageRect = { 451, 118, 78, 78 };
	staminaRect = { 530, 118, 102, 18 };

	GuiImage* base = app->gui->addGuiImage({ 166, 430 }, { 166, 386, 408, 47 }, NULL, this);

	app->gui->addGuiImage({ -132, -19 }, { 362, 226, 76, 53 }, base, this); // lifeback
	app->gui->addGuiImage({ 364, -18 }, { 437, 226, 80, 53 }, base, this); // rageback

	life = app->gui->addGuiImage({ -136, -44 }, { 371, 118, 79, 78 }, base, this);
	rage = app->gui->addGuiImage({ 366, -44 }, { 451, 118, 78, 78 }, base, this);
	stamina = app->gui->addGuiImage({ 27, 20 }, { 530, 118, 102, 18 }, base, this);

	app->gui->addGuiImage({ -166, -55 }, { 0, 331, 116, 103 }, base, this); // lifefront
	app->gui->addGuiImage({ 358, -55 }, { 524, 331, 116, 102 }, base, this); // ragefront

	lifeLabel = app->gui->addGuiLabel("0", NULL, { 10, 5 }, NULL, FONT_WHITE, this);
	rageLabel = app->gui->addGuiLabel("0", NULL, { 10, 20 }, NULL, FONT_WHITE, this);

	staminaDorn = app->gui->addGuiImage({ 9, 19 }, { 153, 280, 18, 22 }, base, this);
	menuExpand = app->gui->addGuiImage({ 145, 8 }, { 296, 253, 16, 27 }, base, this);

	panel = app->gui->addGuiImage({ 98, -25 }, { 170, 253, 110, 25 }, base, this);
	character = app->gui->addGuiImage({ 3, 3 }, { 170, 279, 20, 19 }, panel, this);
	inventory = app->gui->addGuiImage({ 24, 3 }, { 191, 279, 20, 19 }, panel, this);
	tree = app->gui->addGuiImage({ 45, 3 }, { 212, 279, 20, 19 }, panel, this);
	map = app->gui->addGuiImage({ 66, 3 }, { 233, 279, 20, 19 }, panel, this);
	pauseMenu = app->gui->addGuiImage({ 87, 3 }, { 254, 279, 20, 19 }, panel, this);

	app->gui->addGuiImage({ -50, 0 }, { 102, 280, 50, 47 }, base, this); // leftSkill
	current_skill = basic_attack = app->gui->addGuiImage({ 308, 0 }, { 102, 280, 50, 47 }, base, this);
	frenzy = app->gui->addGuiImage({ 162, -70 }, { 51, 280, 50, 47 }, base, this);
	whack = app->gui->addGuiImage({ 233, -70 }, { 0, 280, 50, 47 }, base, this);
	growl = app->gui->addGuiImage({ 305, -70 }, { 0, 280, 50, 47 }, base, this);

	staminaDorn->interactable = true;
	menuExpand->interactable = true;
	basic_attack->interactable = true;
	frenzy->interactable = true;
	whack->interactable = true;
	growl->interactable = true;
	character->interactable = true;
	inventory->interactable = true;
	tree->interactable = true;
	map->interactable = true;
	pauseMenu->interactable = true;

	frenzy->active = false;
	whack->active = false;
	growl->active = false;

	slot1 = app->gui->addGuiInventory({ 176, 9 }, { 342, 395, 30, 30 }, 1, 1, 30, 30, base, this);
	slot2 = app->gui->addGuiInventory({ 207, 9 }, { 342, 395, 30, 30 }, 1, 1, 30, 30, base, this);
	slot3 = app->gui->addGuiInventory({ 238, 9 }, { 342, 395, 30, 30 }, 1, 1, 30, 30, base, this);
	slot4 = app->gui->addGuiInventory({ 269, 9 }, { 342, 395, 30, 30 }, 1, 1, 30, 30, base, this);

	// Character


	// Inventory


	// Tree


	// Map


	// Pause Menu

	pause = app->gui->addGuiImage({ 0, 0 }, { 0, 0, 1, 1 }, NULL, this);
	p_exit = app->gui->addGuiImage({ 45, 200 }, { 460, 0, 534, 35 }, pause, this);
	p_back = app->gui->addGuiImage({ 95, 270 }, { 994, 0, 438, 35 }, pause, this);

	p_exit->interactable = true;
	p_back->interactable = true;

	pause->Desactivate();

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
		if (pause->active) pause->Desactivate();
		else pause->Activate();
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
	// exit if no character assigned
	if (playerAtt == NULL) return true;

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

	std::string text;
	text.assign("%i/%i", int(playerAtt->getLife()), int(playerAtt->getMaxLife()));
	lifeLabel->SetText(text);
	text.assign("%i/%i", int(playerAtt->getRage()), int(playerAtt->getMaxRage()));
	lifeLabel->SetText(text);

	return true;
}

void Hud::OnEvent(GuiElement* element, GUI_Event even)
{
	//Base


	// Character


	// Inventory


	// Tree


	// Map


	// Pause Menu
	if (p_exit == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		//app->sm->fadeToBlack(app->sm->intro);
	}
	if (p_back == element
		&& even == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		pause->Desactivate();
	}
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