#include "snIntro.h"
#include "App.h"
#include "Render.h"
#include "SceneManager.h"
#include "Input.h"
#include "Act1.h"
#include "Gui.h"
#include "Audio.h"
#include "Animation.h"
#include "Textures.h"


snIntro::snIntro() :Scene()
{}


snIntro::~snIntro()
{}

// Called before render is available
bool snIntro::awake(pugi::xml_node& conf)
{
	return true;
}

bool snIntro::start()
{
	//Music
	//NOTE : deactivated for debugging mode
	//app->audio->PlayMusic("audio/music/introedit.ogg", 0);

	pass = false;
	exit = false;

	//Gui Elements

	//Background
	background = app->gui->addGuiImage({ -83, 0 }, {1829, 82, 800, 600}, NULL, this);
	intro_gui.push_back(background);
	
	//Logo
	logoSprite = app->tex->Load("images/LogoAnim.png");

	//logoAnim.SetFrames(0, 0, 400, 170, 30);
	logoAnim.setAnimations(0, 0, 400, 170, 15, 2, 30);
	logoAnim.speed = 0.2f;

	logo = app->gui->addGuiAnimation({ 250, 0 }, { 1829, 82, 800, 600 }, NULL, this, logoSprite, &logoAnim);
	intro_gui.push_back(logo);

	//Play button
	play_button = app->gui->addGuiButton({ 315, 250 }, { 0, 0, 270, 35 }, { 0, 0, 270, 35 }, { 0, 36, 270, 35 }, "Single player", NULL, this);
	intro_gui.push_back(play_button);

	
	//Exit button
	exit_button = app->gui->addGuiButton({ 315, 320 }, { 0, 0, 270, 35 }, { 0, 0, 270, 35 }, { 0, 36, 270, 35 }, "ExiT Diablo II", NULL, this);
	intro_gui.push_back(exit_button);

	//-----------
	

	

	return true;
}

// preUpdate
bool snIntro::preUpdate()
{
	return true;
}

// update
bool snIntro::update(float dt)
{
	//Exit game quick
	if (app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		std::exit(0);
	}
	//Insert iterations for this scene here

	/*
	// When pressing SPACE it changes to the scene outdoor1
	if (app->input->getKey(SDL_SCANCODE_SPACE) == true)
	{
		//The scene manager changes the scene to outdoor1
		app->sm->fadeToBlack(app->sm->outdoor1);
	}
	//
	*/
	if (pass == true)
	{
		app->sm->fadeToBlack(app->sm->act1);
		pass = false;
	}

	else if (exit == true)
	{
		std::exit(0);
	}

	return true;
}

// postUpdate
bool snIntro::postUpdate()
{
	return true;
}

// Called before quitting
bool snIntro::cleanUp()
{
	// NOTA : hacer función en UI que elimine un elemento de la lista(no por el nodo), quizas mejor aun si va por conjunto (para quitar todos los elementos de una escena
	//deletes the elements from the scene from the gui_elements list
	//maybe just by deactivating the iteration of this elements is a good way, who knows, ask ric

	for (list<GuiElement*>::iterator item = intro_gui.begin(); item != intro_gui.end(); item++)
	{
		for (list<GuiElement*>::iterator item2 = app->gui->gui_elements.begin(); item2 != app->gui->gui_elements.end(); item2++)
		{
			if ((*item2) == (*item))
			{
				RELEASE(*item2);
				app->gui->gui_elements.erase(item2);
				break;
			}
		}
	}

	intro_gui.clear();

	return true;
}

void snIntro::OnEvent(GuiElement* element, GUI_Event even)
{	
	//NOTE clicking the button could charge the scene, now sets the bool pass to true and changes it, I don't think it's bad, but I don't know if the other way is better or not.
	//Play button
	if (play_button == element)
	{
		switch (even)
		{

		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			play_button->button_image.SetTextureRect(play_button->click_tex);
			play_button->button_label.SetLocalPosition({ 50, 8 });
		}
			break;

		case EVENT_MOUSE_EXIT:
		{
			play_button->button_image.SetTextureRect(play_button->idle_tex);
			play_button->button_label.Center(true, true);
		}
			break;

		case EVENT_MOUSE_LEFTCLICK_UP:
		{
			play_button->button_image.SetTextureRect(play_button->idle_tex);
			play_button->button_label.Center(true, true);
			pass = true;
		}
			break;
		}
	}
	//Exit button
	if (exit_button == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			exit_button->button_image.SetTextureRect(exit_button->click_tex);
			//NOTE : put this coordinates at the config.
			exit_button->button_label.SetLocalPosition({ 50, 8 });
		}
			break;

		case EVENT_MOUSE_EXIT:
		{
			exit_button->button_image.SetTextureRect(exit_button->idle_tex);
			exit_button->button_label.Center(true, true);
		}
			break;

		case EVENT_MOUSE_LEFTCLICK_UP:
		{
			exit_button->button_image.SetTextureRect(exit_button->idle_tex);
			exit_button->button_label.Center(true, true);
			exit = true;
		}
			break;

		}
	}
}

//Load/unLoad, called when the scene changes
bool snIntro::Load()
{
	start();
	return true;
}

bool snIntro::unLoad()
{
	cleanUp();
	return true;
}