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

bool snIntro::start()
{
	//Music
	//NOTE : deactivated for debugging mode
	//app->audio->PlayMusic("audio/music/introedit.ogg", 0);

	exit = false;

	//Gui Elements

	//Background
	background = app->gui->addGuiImage({ -83, 0 }, {1829, 82, 800, 600}, NULL, this);
	
	//Logo
	logoSprite = app->tex->Load("images/LogoAnim.png");

	//logoAnim.SetFrames(0, 0, 400, 170, 30);
	logoAnim.setAnimations(0, 0, 400, 170, 15, 2, 30);
	logoAnim.speed = 0.2f;

	logo = app->gui->addGuiAnimation({ 250, 0 }, { 1829, 82, 800, 600 }, NULL, this, logoSprite, &logoAnim);

	//Play button
	play_button = app->gui->addGuiButton({ 315, 250 }, { 0, 0, 270, 35 }, { 0, 0, 270, 35 }, { 0, 36, 270, 35 }, "Single player", NULL, this);
	
	//Exit button
	exit_button = app->gui->addGuiButton({ 315, 320 }, { 0, 0, 270, 35 }, { 0, 0, 270, 35 }, { 0, 36, 270, 35 }, "ExiT Diablo II", NULL, this);

	return true;
}

bool snIntro::update(float dt)
{
	bool ret;

	if (ret = !exit)
	{
		ret = !(app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN);
	}

	return ret;
}

void snIntro::OnEvent(GuiElement* element, GUI_Event even)
{	
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
			app->sm->fadeToBlack(app->sm->act1);
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

bool snIntro::Load()
{
	start();
	return true;
}

bool snIntro::unLoad()
{
	app->gui->gui_elements.clear();
	return true;
}