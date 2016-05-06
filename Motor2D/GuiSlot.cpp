#include "GuiSlot.h"
#include "App.h"
#include "Render.h"
#include "GuiSlot.h"

GuiSlot::GuiSlot(iPoint p, SDL_Rect r, GuiElement* par, Module* list)
	:GuiElement({ r.x, r.y }, r, GUI_SLOT, par, list),
	coords(p), inventory_item(NULL), state(DEFAULT)
{

}

GuiSlot::~GuiSlot()
{}

void GuiSlot::update(GuiElement* hover, GuiElement* focus)
{
	//The states goes to deafult each iteration
	state = DEFAULT;
}

void GuiSlot::draw()
{
	//According to its state, it draws diferent color squares
	switch (state)
	{
	case DEFAULT:
		break;
	case GREEN:
		app->render->DrawQuad(getScreenRect(), 0, 100, 0, 50, true, false);
		break;
	case YELLOW:
		app->render->DrawQuad(getScreenRect(), 200, 200, 0, 50, true, false);
		break;
	case RED:
		app->render->DrawQuad(getScreenRect(), 200, 0, 0, 50, true, false);
		break;
	}
}

void GuiSlot::drawDebug()
{
	SDL_Rect rect = getScreenRect();
	rect.x -= app->render->camera.x;
	rect.y -= app->render->camera.y;

	app->render->DrawQuad(rect, 255, 0, 0, 1000, false);


	if (inventory_item)
	{
		app->render->DrawQuad(rect, 0, 0, 255, 50);
	}
}