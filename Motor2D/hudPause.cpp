#include "hudPause.h"
#include "App.h"
#include "Gui.h"
#include "Input.h"
#include "SceneManager.h"
#include "Game.h"
#include "snIntro.h"



hudPause::hudPause()
{

}

//Destructor
hudPause::~hudPause()
{

}

//Called before fist frame
bool hudPause::start()
{
	main_menu = false;
	active = true;
	//Menu
	options = app->gui->addGuiImage({ 210, 130 }, { 395, 71, 194, 31 }, NULL, this);
	options->interactable = true;
	options->active = false;
	Hud_gui_elements.push_back(options);

	saveandexit = app->gui->addGuiImage({ 45, 200 }, { 460, 0, 534, 35 }, NULL, this);
	saveandexit->interactable = true;
	saveandexit->active = false;
	Hud_gui_elements.push_back(saveandexit);

	returntogame = app->gui->addGuiImage({ 95, 270 }, { 994, 0, 438, 35 }, NULL, this);
	returntogame->interactable = true;
	returntogame->active = false;
	Hud_gui_elements.push_back(returntogame);

	return true;
}

//Called before each loop iteration
bool hudPause::preUpdate()
{
	if (app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ActivateMenu();
	}

	if (main_menu == true)
	{
		ActivateMenu();
		app->sm->ChangeScene(app->sm->intro);
	}

	return true;
}

//Called each frame
bool hudPause::update(float dt)
{
	return true;
}

//Called after each loop iteration
bool hudPause::postUpdate()
{
	return true;
}


//Called before quitting
bool hudPause::cleanUp()
{
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
void hudPause::OnEvent(GuiElement* element, GUI_Event even)
{
	//Game menu --------------------------------
	//SaveAndExit button -----------------------------------------------------------------------------
	if (saveandexit == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
			main_menu = true;
			break;
		}
	}

	//ReturnToGame button -----------------------------------------------------------------------------
	if (returntogame == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
			ActivateMenu();
			break;
		}
	}
}

void hudPause::ActivateMenu()
{
	options->active = !options->active;
	returntogame->active = !returntogame->active;
	saveandexit->active = !saveandexit->active;

	app->game->pause = !app->game->pause;
}