#include "snIntro.h"
#include "App.h"
#include "Render.h"
#include "SceneManager.h"
#include "Input.h"
#include "Act1.h"
#include "Act2.h"
#include "Gui.h"
#include "Audio.h"
#include "Animation.h"
#include "Textures.h"
#include "snWin.h"


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
	background = app->gui->addGuiImage({ -83, 0 }, {0, 0, 800, 600}, NULL, this);
	
	//Logo
	logoSprite = app->tex->Load("images/LogoAnim.png");

	//logoAnim.SetFrames(0, 0, 400, 170, 30);
	logoAnim.setAnimations(0, 0, 400, 170, 15, 2, 30);
	logoAnim.speed = 0.2f;

	logo = app->gui->addGuiAnimation({ 250, 0 }, { 1829, 82, 800, 600 }, NULL, this, logoSprite, &logoAnim);

	//Play button
	playButton = app->gui->addGuiButton({ 315, 250 }, { 0, 960, 270, 35 }, { 0, 960, 270, 35 }, { 0, 1024, 270, 35 }, "Single player", NULL, this);
	
	//Exit button
	exitButton = app->gui->addGuiButton({ 315, 320 }, { 0, 960, 270, 35 }, { 0, 960, 270, 35 }, { 0, 1024, 270, 35 }, "ExiT Diablo II", NULL, this);

	//app->audio->PlayMusic("audio/music/introedit.ogg",0.5f);
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
	if (playButton == element)
	{
		switch (even)
		{

		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			playButton->button_image.SetTextureRect(playButton->click_tex);
			playButton->button_label.SetLocalPosition({ 50, 8 });
		}
			break;

		case EVENT_MOUSE_EXIT:
		{
			playButton->button_image.SetTextureRect(playButton->idle_tex);
			playButton->button_label.Center(true, true);
		}
			break;

		case EVENT_MOUSE_LEFTCLICK_UP:
		{
			playButton->button_image.SetTextureRect(playButton->idle_tex);
			playButton->button_label.Center(true, true);
			app->sm->fadeToBlack(app->sm->act1);
			app->audio->active = false;
		}
			break;
		}
	}
	//Exit button
	if (exitButton == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			exitButton->button_image.SetTextureRect(exitButton->click_tex);
			exitButton->button_label.SetLocalPosition({ 50, 8 });
		}
			break;

		case EVENT_MOUSE_EXIT:
		{
			exitButton->button_image.SetTextureRect(exitButton->idle_tex);
			exitButton->button_label.Center(true, true);
		}
			break;

		case EVENT_MOUSE_LEFTCLICK_UP:
		{
			exitButton->button_image.SetTextureRect(exitButton->idle_tex);
			exitButton->button_label.Center(true, true);
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