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

	// Called before render is available
bool snWin::awake(pugi::xml_node& conf)
{
	return true;
}

	// Called the first frame
bool snWin::start()
{
	
	back = app->tex->Load("textures/win.jpg");
	ret.start();
	return true;
}

	// preUpdate
bool snWin::preUpdate()
{
	return true;
}

	// update
bool snWin::update(float dt)
{
	app->render->Blit(back, -225, 0, NULL);

	if (ret.ReadSec() > 5)
		app->sm->fadeToBlack(app->sm->intro);
	return true;
}

	// postUpdate
bool snWin::postUpdate()
{
	return true;
}

	// Called before quitting
bool snWin::cleanUp()
{
	app->tex->unLoad(back);
	return true;
}

	// Called on certain event
void snWin::OnEvent(GuiElement* element, GUI_Event even)
{
	
}


	//Load/unLoad, called when the scene changes
bool snWin::Load()
{
	start();
	return true;
}
bool snWin::unLoad()
{
	cleanUp();
	return true;
}


