#include "Hud.h"
#include "App.h"
#include "Gui.h"
#include "Player.h"
#include "Game.h"
#include "SceneManager.h"
#include "snIntro.h"
#include "hudElement.h"
#include "hudBelt.h"
#include "hudPause.h"
#include "hudInventory.h"
#include "hudBlood.h"
#include "hudSkilltree.h"


//NOTE : provisional
#include "Input.h"

//Constructor
Hud::Hud() : Module()
{
	belt = new hudBelt();
	pause_menu = new hudPause();
	inventory = new hudInventory();
	blood = new hudBlood();
	skilltree = new hudSkilltree();

	Hud_elements.push_back(skilltree);
	Hud_elements.push_back(inventory);
	Hud_elements.push_back(belt);
	Hud_elements.push_back(pause_menu);
	Hud_elements.push_back(blood);
	
	
}

Hud::~Hud()
{
	for (int i = 0; i < Hud_elements.size(); i++)
	{
		RELEASE(Hud_elements[i]);
	}

	Hud_elements.clear();
}

bool Hud::start()
{
	for (int i = 0; i < Hud_elements.size(); i++)
	{
		Hud_elements[i]->start();
	}

	return true;
}

//Called before each loop iteration
bool Hud::preUpdate()
{

	if (app->input->getKey(SDL_SCANCODE_I) == KEY_DOWN || belt->inventorybutton_pressed == true)
	{
		belt->inventorybutton_pressed = false;
		inventory->Activate();

	}

	if (app->input->getKey(SDL_SCANCODE_S) == KEY_DOWN || belt->skilltreebutton_pressed == true)
	{
		belt->skilltreebutton_pressed = false;
		skilltree->Activate();
	}

	for (int i = 0; i < Hud_elements.size(); i++)
	{
		if (Hud_elements[i]->active)
			Hud_elements[i]->preUpdate();
	}

	return true;
}

//Called each frame
bool Hud::update(float dt)
{	
	for (int i = 0; i < Hud_elements.size(); i++)
	{
		if (Hud_elements[i]->active)
			Hud_elements[i]->update(dt);
	}


	return true;
}

//Called after each loop iteration
bool Hud::postUpdate()
{
	for (int i = 0; i < Hud_elements.size(); i++)
	{
		if (Hud_elements[i]->active)
			Hud_elements[i]->postUpdate();
	}

	return true;
}

bool Hud::cleanUp()
{
	for (int i = 0; i < Hud_elements.size(); i++)
	{
		Hud_elements[i]->cleanUp();
	}

	return true;
}

void Hud::OnEvent(GuiElement* element, GUI_Event even)
{

}




