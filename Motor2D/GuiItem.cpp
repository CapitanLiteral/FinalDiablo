#include "GuiItem.h"
#include "App.h"
#include "Input.h"
#include "GuiSlot.h"
#include "GuiInventory.h"
#include "Gui.h"
#include "Render.h"
#include "Attributes.h"



GuiItem::GuiItem(int s, iPoint* coord, SDL_Rect r, int quantity, PlayerAttributes* lis)
	: GuiElement({ 0, 0 }, r, GUI_ITEM, NULL, NULL), quantity(quantity), lis(lis), image({ 0, 0 }, r, this, NULL)
{
	size = s;
	coords = new iPoint[size];
	reference_slot = NULL;
	inventory = NULL;
	pivot = { ITEM_SECTION_SIZE / 2, ITEM_SECTION_SIZE / 2 };

	for (int i = 0; i < s; i++)
	{
		coords[i] = coord[i];
	}
}

GuiItem::~GuiItem()
{
	delete[] coords;
}

void GuiItem::draw()
{
	image.draw();
}

void GuiItem::drawDebug()
{
	image.drawDebug();

	iPoint tmp = getPivotPosition();
	
	app->render->DrawQuad({ tmp.x, tmp.y, 2, 2 }, 0, 255, 0, 1000, true, false);

}

void GuiItem::update(GuiElement* hover, GuiElement* focus)
{
	if (!(app->gui->dragged_item))
	{
		if (CheckCollision(app->input->getMousePosition()))
		{
			//Feedback :D
			inventory->SetSlotsState(this, GREEN);
			
			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				inventory->FreeItem(this);
			}
		}
	}
}

//Gives the pivot position over the screen
iPoint GuiItem::getPivotPosition()
{
	iPoint ret = pivot;
	ret += getScreenPosition();

	return ret;
}

//Frees the slots under the item
void GuiItem::FreeSlots()
{
	for (int i = 0; i < size; i++)
	{
		GuiSlot* slot = inventory->getSlotFromCoord(reference_slot->coords + coords[i]);
		if (slot)
			slot->inventory_item = NULL;
	}
}

//Moves accordingly to the mouse
void GuiItem::Move()
{ 
	iPoint tmp = app->input->getMousePosition();
	tmp.x -= getLocalRect().w / 2;
	tmp.y -= getLocalRect().h / 2;

	SetLocalPosition(tmp);
}


void Potion::Effect()
{
	if (lis)
	{
		TempMod* mod = new TempMod(5.0f, 1.5f, NONFLAT_LIFE_REGEN);
		lis->addMod(mod);
	}
}


