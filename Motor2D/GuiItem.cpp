#include "GuiItem.h"
#include "App.h"
#include "Input.h"
#include "GuiSlot.h"
#include "GuiInventory.h"
#include "Gui.h"
#include "Render.h"
//Absolutely not
#include "Player.h"
#include "Game.h"



GuiItem::GuiItem(int s, iPoint* coord, SDL_Rect r) 
	:GuiElement({ 0, 0 }, r, GUI_ITEM, NULL, NULL)
	, image({ 0, 0 }, r, this, NULL)
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
	//If there's no dragged_item, when clicking over the item, it's freed from the inventory
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

	//If this is the dragging item, move it
	/*if (app->gui->dragged_item == this)
	{
		Move();
	}*/
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
	//TODO 6: iterate all the slots that the item occupies and free them (inventory_item = NULL)
	//Hint: functionality very similiar to AssignItemToSlots(), but now at item.cpp, and freeing the slots
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
	//TODO 4: Fill Move() function so, the item movement is exactly like the cursor (hint: not using the draggable property or mouse motion)
	//and also make that the item is centered at the cursor (is at the middle not at the vertex)
	iPoint tmp = app->input->getMousePosition();
	tmp.x -= getLocalRect().w / 2;
	tmp.y -= getLocalRect().h / 2;

	SetLocalPosition(tmp);
}

//WARNING: guarrada
void GuiItem::Effect()
{
	//Player* player = app->game->player;
	//player->RestoreHP(50);
}
