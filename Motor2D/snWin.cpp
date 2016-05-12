#include "snWin.h"
#include "Render.h"
#include "App.h"
#include "Timer.h"
#include "Textures.h"
#include "SceneManager.h"
#include "snIntro.h"

	// Constructor
snWin::snWin()
{}

	// Destructor
snWin::~snWin()
{}

	// Called the first frame
bool snWin::start()
{

	background = app->gui->addGuiImage({ 0, 0 }, { 5, 1065, 669, 501}, NULL, this);
	backMenuButton = app->gui->addGuiButton({ 315, 320 }, { 0, 0, 270, 35 }, { 0, 0, 270, 35 }, { 0, 36, 270, 35 }, "Back To Menu", NULL, this);

	//ret.start();
	return true;
}

	// update
bool snWin::update(float dt)
{
	return true;
}

	// Called on certain event
void snWin::OnEvent(GuiElement* element, GUI_Event even)
{
	if (backMenuButton == element)
	{
		switch (even)
		{
		case EVENT_MOUSE_LEFTCLICK_DOWN:
		{
			backMenuButton->button_image.SetTextureRect(backMenuButton->click_tex);
			backMenuButton->button_label.SetLocalPosition({ 50, 8 });
		}
		break;

		case EVENT_MOUSE_EXIT:
		{
			backMenuButton->button_image.SetTextureRect(backMenuButton->idle_tex);
			backMenuButton->button_label.Center(true, true);
		}
		break;

		case EVENT_MOUSE_LEFTCLICK_UP:
		{
			backMenuButton->button_image.SetTextureRect(backMenuButton->idle_tex);
			backMenuButton->button_label.Center(true, true);
			app->sm->fadeToBlack(app->sm->intro);
		}
		break;

		}
	}
}

	//Load/unLoad, called when the scene changes
bool snWin::Load()
{
	start();
	return true;
}
bool snWin::unLoad()
{
	app->gui->clearUI();
	return true;
}


