#include "hudBelt.h"
#include "App.h"
#include "Gui.h"
//NOPE
#include "Game.h"
#include "Hud.h"
#include "Player.h"
#include "Input.h"

//NOTE: PROVISIONAL 
#include "Fonts.h"

//Constructor
hudBelt::hudBelt():hudElement()
{
	active = true;
}

//Destructor
hudBelt::~hudBelt()
{}

//Called before fist frame
bool hudBelt::start()
{
	//Player
	player = app->game->player;

	//
	life_current_h = mana_current_h = 78;

	Hud = app->gui->addGuiImage({ 166, 430 }, { 166, 386, 408, 47 }, NULL, this);
	Hud_gui_elements.push_back(Hud);

	inventory1 = app->gui->addGuiInventory({ 176, 9 }, { 342, 395, 30, 30 }, 1, 1 , 30, 30, Hud, this);
	Hud_gui_elements.push_back(inventory1);
	inventory2 = app->gui->addGuiInventory({ 207, 9 }, { 373, 395, 30, 30 }, 1, 1, 30, 30, Hud, this);
	Hud_gui_elements.push_back(inventory2);
	inventory3 = app->gui->addGuiInventory({ 238, 9 }, { 404, 395, 30, 30 }, 1, 1, 30, 30, Hud, this);
	Hud_gui_elements.push_back(inventory3);
	inventory4 = app->gui->addGuiInventory({ 269, 9 }, { 435, 395, 30, 30 }, 1, 1, 30, 30, Hud, this);
	Hud_gui_elements.push_back(inventory4);
	//Hud elements definition
	//NOTE: these position are very inaccurate

	HUDornament = app->gui->addGuiImage({ 96, -5 }, { 262, 381, 115, 5 }, Hud, this);
	Hud_gui_elements.push_back(HUDornament);

	HUDback1 = app->gui->addGuiImage({ -132, -19 }, { 362, 226, 76, 53 }, Hud, this);
	Hud_gui_elements.push_back(HUDback1);
	
	HUDback2 = app->gui->addGuiImage({ 364, -18 }, { 437, 226, 80, 53 }, Hud, this);
	Hud_gui_elements.push_back(HUDback2);

	life = app->gui->addGuiImage({ -136, -44 }, { 371, 118, 79, 78 }, Hud, this);
	Hud_gui_elements.push_back(life);

	mana = app->gui->addGuiImage({ 366, -44 }, { 451, 118, 78, 78 }, Hud, this);
	Hud_gui_elements.push_back(mana);

	stamina = app->gui->addGuiImage({ 27, 20 }, { 530, 118, 102, 18 }, Hud, this);
	Hud_gui_elements.push_back(stamina);

	life_holder = app->gui->addGuiImage({ -166, -55 }, { 0, 331, 116, 103 }, Hud, this);
	Hud_gui_elements.push_back(life_holder);

	mana_holder = app->gui->addGuiImage({ 358, -55 }, { 524, 331, 116, 102 }, Hud, this);
	Hud_gui_elements.push_back(mana_holder);

	runbutton = app->gui->addGuiImage({ 9, 19 }, { 153, 280, 18, 22 }, Hud, this);
	runbutton->interactable = true;
	Hud_gui_elements.push_back(runbutton);

	minipanelbutton = app->gui->addGuiImage({ 145, 8 }, { 296, 253, 16, 27 }, Hud, this);
	minipanelbutton->interactable = true;
	Hud_gui_elements.push_back(minipanelbutton);

	attack1 = app->gui->addGuiImage({ -50, 0 }, { 102, 280, 50, 47 }, Hud, this);
	attack1->interactable = true;
	Hud_gui_elements.push_back(attack1);

	attack2 = app->gui->addGuiImage({ 308, 0 }, { 102, 280, 50, 47 }, Hud, this);
	attack2->interactable = true;
	Hud_gui_elements.push_back(attack2);

	//Skills
	//Attack 1 ----------------------------------------
	skill11 = app->gui->addGuiImage({ -51, -70 }, { 51, 280, 50, 47 }, Hud, this);
	skill11->interactable = true;
	skill11->active = false;
	Hud_gui_elements.push_back(skill11);

	skill12 = app->gui->addGuiImage({ 20, -70 }, { 0, 280, 50, 47 }, Hud, this);
	skill12->interactable = true;
	skill12->active = false;
	Hud_gui_elements.push_back(skill12);

	skill13 = app->gui->addGuiImage({ 91, -70 }, { 0, 280, 50, 47 }, Hud, this);
	skill13->interactable = true;
	skill13->active = false;
	Hud_gui_elements.push_back(skill13);

	//Attack 2 ----------------------------------------
	skill21 = app->gui->addGuiImage({ 305, -70 }, { 51, 280, 50, 47 }, Hud, this);
	skill21->interactable = true;
	skill21->active = false;
	Hud_gui_elements.push_back(skill21);

	skill22 = app->gui->addGuiImage({ 233, -70 }, { 0, 280, 50, 47 }, Hud, this);
	skill22->interactable = true;
	skill22->active = false;
	Hud_gui_elements.push_back(skill22);

	skill23 = app->gui->addGuiImage({ 162, -70 }, { 0, 280, 50, 47 }, Hud, this);
	skill23->interactable = true;
	skill23->active = false;
	Hud_gui_elements.push_back(skill23);

	//Minipanel's buttons -------------------------------
	
	minipanel = app->gui->addGuiImage({ 98, -25 }, { 170, 253, 110, 25 }, Hud, this);
	Hud_gui_elements.push_back(minipanel);

	/*stats = app->gui->addGuiImage({ 3, 3 }, { 170, 279, 20, 19 }, minipanel, this);
	stats->interactable = true;
	Hud_gui_elements.push_back(stats);*/

	stats = app->gui->addGuiImageWithLabel({ 3, 3 }, { 170, 279, 20, 19 }, "player", app->font->description, { -20, 0 }, minipanel, this);
	stats->interactable = true;
	Hud_gui_elements.push_back(stats);

	inventorybutton = app->gui->addGuiImageWithLabel({ 24, 3 }, { 191, 279, 20, 19 }, "invenTory", app->font->description, {-20, 0}, minipanel, this);
	inventorybutton->interactable = true;
	Hud_gui_elements.push_back(inventorybutton);

	skilltree = app->gui->addGuiImageWithLabel({ 45, 3 }, { 212, 279, 20, 19 }, "skill Tree", app->font->description, {-20, 0}, minipanel, this);
	skilltree->interactable = true;
	Hud_gui_elements.push_back(skilltree);

	map = app->gui->addGuiImageWithLabel({ 66, 3 }, { 233, 279, 20, 19 }, "map", app->font->description, {0, 0}, minipanel, this);
	map->interactable = true;
	Hud_gui_elements.push_back(map);

	game_menu = app->gui->addGuiImageWithLabel({ 87, 3 }, { 254, 279, 20, 19 }, "game menu", app->font->description, {-20, 0}, minipanel, this);
	game_menu->interactable = true;
	Hud_gui_elements.push_back(game_menu);
	


	minipanel->Desactivate();

	return true;
}

//Called before each loop iteration
bool hudBelt::preUpdate()
{
	if (app->input->getKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		RunningOn();
	}

	if (app->input->getKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		list<GuiItem*>::iterator it = inventory1->items.begin();
		if (it != inventory1->items.end())
		{
			GuiItem* i = (*it);
			i->Effect();
			i->FreeSlots();
			inventory1->items.remove(i);
			RELEASE(i);
		}

	}

	if (app->input->getKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		list<GuiItem*>::iterator it = inventory2->items.begin();
		if (it != inventory2->items.end())
		{
			GuiItem* i = (*it);
			i->Effect();
			i->FreeSlots();
			inventory2->items.remove(i);
			RELEASE(i);
		}

	}

	if (app->input->getKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		list<GuiItem*>::iterator it = inventory3->items.begin();
		if (it != inventory3->items.end())
		{
			GuiItem* i = (*it);
			i->Effect();
			i->FreeSlots();
			inventory3->items.remove(i);
			RELEASE(i);
		}

	}

	if (app->input->getKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		list<GuiItem*>::iterator it = inventory4->items.begin();
		if (it != inventory4->items.end())
		{
			GuiItem* i = (*it);
			i->Effect();
			i->FreeSlots();
			inventory4->items.remove(i);
			RELEASE(i);
		}

	}
	return true;
}

//Called each frame
bool hudBelt::update(float dt)
{
	return true;
}

//Called after each loop iteration
bool hudBelt::postUpdate()
{
	return true;
}

//Called before quitting
bool hudBelt::cleanUp()
{

	//NOTE: the problems between lists and vectors begin to grow, but i think this is a good way to approach this
	for (int i = 0; i < Hud_gui_elements.size(); i++)
	{
		for (list<GuiElement*>::iterator item2 = app->gui->gui_elements.begin(); item2 != app->gui->gui_elements.end(); item2++)
		{
			if ((*item2) == Hud_gui_elements[i])
			{
				RELEASE(*item2);
				app->gui->gui_elements.erase(item2);
				break;
			}
		}
	}

	Hud_gui_elements.clear();

	return true;
}

//Called when there's a gui event
void hudBelt::OnEvent(GuiElement* element, GUI_Event even)
{
	//Hud -------------------------------------
	//Run button
	if (runbutton == element)
	{
		switch (even)
		{
			case EVENT_MOUSE_LEFTCLICK_DOWN:
			{
				RunningOn();
			}
			break;
		}
	}

	//Mini panel button
	if (minipanelbutton == element)
	{
		switch (even)
		{
			case EVENT_MOUSE_LEFTCLICK_DOWN:
			{
				if (minipanel_pressed == false)
				{
					minipanel_pressed = true;
					minipanelbutton->SetTextureRect({ 281, 253, 16, 27 });
					minipanel->Activate();
				}
				else
				{
					minipanel_pressed = false;
					minipanelbutton->SetTextureRect({ 296, 253, 16, 27 });
					minipanel->Desactivate();
				}
			}
			break;
		}
	}
	
	//Inventory button
	if (inventorybutton == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (inventorybutton_pressed == false)
				inventorybutton_pressed = true;
			else
				inventorybutton_pressed = false;
		}
		break;
		case EVENT_MOUSE_ENTER:
		{
			inventorybutton->descriptionlabel->Activate();
		}
		break;
		case EVENT_MOUSE_EXIT:
		{
			inventorybutton->descriptionlabel->Desactivate();
		}
		break;
		}
	}

	//Stats button
	if (stats == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_ENTER:
		{
			stats->descriptionlabel->Activate();
		}
			break;
		case EVENT_MOUSE_EXIT:
		{
			stats->descriptionlabel->Desactivate();
		}
			break;
		}
	}

	//Skilltree button
	if (skilltree == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (skilltreebutton_pressed == false)
				skilltreebutton_pressed = true;
			else
				skilltreebutton_pressed = false;
		}
		break;
		case EVENT_MOUSE_ENTER:
		{
			skilltree->descriptionlabel->Activate();
		}
		break;
		case EVENT_MOUSE_EXIT:
		{
			skilltree->descriptionlabel->Desactivate();
		}
		break;
		}
	}

	//Map button
	if (map == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_ENTER:
		{
			map->descriptionlabel->Activate();
		}
		break;
		case EVENT_MOUSE_EXIT:
		{
			map->descriptionlabel->Desactivate();
		}
		break;
		}
	}

	//Game_menu button
	if (game_menu == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_ENTER:
		{
			game_menu->descriptionlabel->Activate();
		}
		break;
		case EVENT_MOUSE_EXIT:
		{
			game_menu->descriptionlabel->Desactivate();
		}
		break;
		}
	}

	//Attack1
	if (attack1 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (attack1_pressed == false)
			{
				attack1_pressed = true;
				skill11->active = true;
				skill12->active = true;
				skill13->active = true;
			}
			else
			{
				attack1_pressed = false;
				skill11->active = false;
				skill12->active = false;
				skill13->active = false;
			}
		}
		break;
		}
	}

	//Attack2
	if (attack2 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			if (attack2_pressed == false)
			{
				attack2_pressed = true;
				skill21->active = true;
				skill22->active = true;
				skill23->active = true;
			}
			else
			{
				attack2_pressed = false;
				skill21->active = false;
				skill22->active = false;
				skill23->active = false;
			}
		}
		break;
		}
	}
	//Skills
	//Attack 1---------------------------------
	if (skill11 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			SDL_Rect rect = skill11->getTextureRect();
			SDL_Rect rect2 = attack1->getTextureRect();
			attack1->SetTextureRect(rect);
			skill11->SetTextureRect(rect2);
		}
		break;
		}
	}
	if (skill12 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			SDL_Rect rect = skill12->getTextureRect();
			SDL_Rect rect2 = attack1->getTextureRect();
			attack1->SetTextureRect(rect);
			skill12->SetTextureRect(rect2);
		}
		break;
		}
	}
	if (skill13 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			SDL_Rect rect = skill13->getTextureRect();
			SDL_Rect rect2 = attack1->getTextureRect();
			attack1->SetTextureRect(rect);
			skill13->SetTextureRect(rect2);
		}
		break;
		}
	}
	//Attack 2---------------------------------
	if (skill21 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			SDL_Rect rect = skill21->getTextureRect();
			SDL_Rect rect2 = attack2->getTextureRect();
			attack2->SetTextureRect(rect);
			skill21->SetTextureRect(rect2);
		}
		break;
		}
	}
	if (skill22 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			SDL_Rect rect = skill22->getTextureRect();
			SDL_Rect rect2 = attack2->getTextureRect();
			attack2->SetTextureRect(rect);
			skill22->SetTextureRect(rect2);
		}
		break;
		}
	}
	if (skill23 == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			SDL_Rect rect = skill23->getTextureRect();
			SDL_Rect rect2 = attack2->getTextureRect();
			attack2->SetTextureRect(rect);
			skill23->SetTextureRect(rect2);
		}
		break;
		}
	}
	//Hud end ---------------------------------
}

//Sets the life stat at the Hud
void hudBelt::SetLife(int max_HP, int HP)
{
	if (HP != max_HP && HP != 0)
	{
		life_current_h = STAT_MAX_H / max_HP;
		life_current_h *= HP;
	}
	else if (max_HP == HP)
	{
		life_current_h = STAT_MAX_H;
	}
	else if (HP == 0)
	{
		life_current_h = 0;
	}

	/*if (app->debug)
	{
	life_debug->SetText(("%d / %d", HP, max_HP));
	}*/

	life->SetTextureRect({ 371, STAT_TEX_Y - int(life_current_h), 79, int(life_current_h) });
	life->SetLocalPosition({ -136, STAT_LOCAL_Y - int(life_current_h) });
}

//Sets the mana stat at the Hud
void hudBelt::SetMana(int max_MP, int MP)
{
	if (MP != max_MP && MP != 0)
	{
		mana_current_h = STAT_MAX_H / max_MP;
		mana_current_h *= MP;
	}
	else if (max_MP == MP)
	{
		mana_current_h = STAT_MAX_H;
	}
	else if (MP == 0)
	{
		mana_current_h = 0;
	}

	/*if (app->debug)
	{
	mana_debug->SetText("FUCK YOU");
	}*/

	mana->SetTextureRect({ 451, STAT_TEX_Y - int(mana_current_h), 78, int(mana_current_h) });
	mana->SetLocalPosition({ 366, STAT_LOCAL_Y - int(mana_current_h) });
}

void hudBelt::SetStamina(int max_ST, int ST)
{
	if (ST != max_ST && ST != 0)
	{
		stamina_current_w = STAT_MAX_W / max_ST;
		stamina_current_w *= ST;
	}
	else if (max_ST == ST)
	{
		stamina_current_w = STAT_MAX_W;
	}
	else if (ST == 0)
	{
		stamina_current_w = 0;
	}

	stamina->SetTextureRect({ 530, 118, int(stamina_current_w), 18 });
}

void hudBelt::RunningOn()
{
	bool run = player->RunOn();

	if (run)
	{
		runbutton->SetTextureRect({ 153, 301, 18, 22 });
	}
	else
	{
		runbutton->SetTextureRect({ 153, 280, 18, 22 });
	}
}