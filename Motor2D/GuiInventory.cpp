#include "GuiInventory.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Gui.h"
#include "Game.h"
#include "Player.h"


//Constructor
GuiInventory::GuiInventory(iPoint p, SDL_Rect r,int col, int row, int slot_w, int slot_h, GuiElement* par, Module* list)
:GuiElement(p, r, GUI_INVENTORY, par, list), 
	image({ 0, 0 }, r, this, NULL),
	columns(col), rows(row), slot_width(slot_w), slot_height(slot_h)
{
	//Creation of all the slots
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			//TODO 1: create each slot from the inventory and add the to vector slots:
			iPoint pos = { j, i };
			pos = SlotToInventory(pos);
			SDL_Rect rect = { pos.x, pos.y, slot_w, slot_w };
			GuiSlot tmp({ j, i }, rect, this, list);
			slots.push_back(tmp);
		}
	}

	//items.push_back(new Potion(1, new iPoint({ 0, 0 }), { 703, 892, 29, 29 }, 1, app->game->player->attributes));
}

//Destructor
GuiInventory::~GuiInventory()
{

}

//Called each loop iteration
void GuiInventory::update(GuiElement* hover, GuiElement* focus)
{
	//Create this varaible to check if the dragged_item changes
	GuiItem* drag_item = app->gui->dragged_item;

	//update the slots
	for (int i = 0; i < slots.size(); i++)
	{
		slots[i].update(hover, focus);
	}
	//---

	//update the items
	list<GuiItem*>::iterator it = items.begin();
	for (; it != items.end(); it++)
	{
		(*it)->update(hover, focus);
	}
	//---

	//Checks the activity of the dragged_item
	if (this == hover && app->gui->dragged_item && drag_item == app->gui->dragged_item)
	{
		CheckDraggedItem(drag_item);
	}
}

//Checks the activy of the dragged item over the inventory
void GuiInventory::CheckDraggedItem(GuiItem* drag_item)
{
	//Item that can be exchanged
	GuiItem* extra_item = NULL;
	iPoint coord = getCoordFromItem(drag_item);

	//Cheks if the drag_item is placeable
	//TODO 7: when beginning this to do, change the false at IsPlaceable() for true, indicating that an exchange can be made
	if (IsPlaceable(drag_item, coord, true, &extra_item))
	{
		//TODO 8(hint): According to if there's an item to be exchanged, the slots has to give us feedback here, no more hints about the others
		if (!extra_item)
			SetSlotsState(drag_item, GREEN);
		else
			SetSlotsState(extra_item, YELLOW);

		//If left mouse button pressed while being able to place, add the item
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			GuiSlot* slot = getSlotFromCoord(coord);

			//According to if there's an exchange item, it does an exchange or just adds the item
			if (extra_item)
				Exchange(drag_item, slot, extra_item);

			else
				addItem(drag_item, slot);
		}
	}
}

//Blit to screen
void GuiInventory::draw()
{
	image.draw();

	list<GuiItem*>::iterator it = items.begin();
	for (; it != items.end(); it++)
	{
		(*it)->draw();
	}

	for (int i = 0; i < slots.size(); i++)
	{
		slots[i].draw();
	}
}

//Draws debug elements
void GuiInventory::drawDebug()
{
	image.drawDebug();

	for (int i = 0; i < slots.size(); i++)
	{
		slots[i].drawDebug();
	}
	
	list<GuiItem*>::iterator it = items.begin();
	for (; it != items.end(); it++)
	{
		(*it)->drawDebug();
	}

}

//From slot coordinate to local position in the inventory
iPoint GuiInventory::SlotToInventory(iPoint pos)
{
	iPoint ret = { 0, 0 };

	ret.x = pos.x * slot_width;
	ret.y = pos.y * slot_height;

	return ret;

}

//From local position in the inventory to the slot coordinate
iPoint GuiInventory::InventoryToSlot(iPoint pos)
{
	iPoint ret = { 0, 0 };

	ret.x = pos.x / slot_width;
	ret.y = pos.y / slot_height;

	return ret;
}

//From a screen point to the slot coordinate
iPoint GuiInventory::ScreenToSlot(iPoint pos)
{
	iPoint ret = pos;

	ret -= getScreenPosition();

	return InventoryToSlot(ret);
}

//adds item to the inventory
void GuiInventory::addItem(GuiItem* item, GuiSlot* new_slot)
{
	AssignItemToSlots(item, new_slot->coords);

	item->reference_slot = new_slot;
	item->SetLocalPosition(item->reference_slot->getScreenPosition());

	//In case there's another inventory , check if
	if (item->inventory != this)
	{
		if (item->inventory)
			item->inventory->items.remove(item);
		
		items.push_back(item);
		item->inventory = this;
	}

	if (item == app->gui->dragged_item)
	{
		app->gui->dragged_item = NULL;
	}

}

//Checks if there's space for an item and then adds it
bool GuiInventory::AutomaticaddItem(GuiItem* item)
{
	/*
	for (int i = 0; i < slots.size(); i++)
	{
			if (IsPlaceable(item, slots[i].coords, false))
			{
				addItem(item, &slots[i]);
				return true;
			}
	}
	*/

	if ((*items.begin())->quantity < (*items.begin())->maxquantity)
	{
		(*items.begin())->quantity++;
	}
	return false;
}


//Frees and Item from the inventory
void GuiInventory::FreeItem(GuiItem* item)
{
	item->FreeSlots();
	app->gui->dragged_item = item;
}

//Places an item in exchange of another
void GuiInventory::Exchange(GuiItem* enter_item, GuiSlot* enter_slot, GuiItem* exit_item)
{
	FreeItem(exit_item);
	addItem(enter_item, enter_slot);
}

//Asigns the item to the group of slots that it will occupy
void GuiInventory::AssignItemToSlots(GuiItem* item, iPoint& coord)
{
	//TODO 2: fill AssignItemToSlots so, given a pointer to the item to be assigned and the coordinate of the slot of reference where it will be assigned
	//To iterate all the slots where the item will be placed and assign the item as the inventory_item of each of these slots.
	//Hint : use the function getSlotFromCoord to get the correct slot for each item coordinate

	for (int i = 0; i < item->size; i++)
	{
		GuiSlot* tmp = getSlotFromCoord(coord + item->coords[i]);
		if (tmp)
			tmp->inventory_item = item;
	}
}

//Checks if there's space for an item
bool GuiInventory::IsPlaceable(GuiItem* item, iPoint& coord, bool exchange, GuiItem** extra_item)
{
	//The way to check if a space is placeable is different according to the permission to do or not an exchange:

	//Placing in case there's no need to do an exchange
	if (exchange == false)
	{
		for (int i = 0; i < item->size; i++)
		{
			GuiSlot* tmp = getSlotFromCoord(coord + item->coords[i]);
			if (tmp == NULL || tmp->inventory_item)
			{
				return false;
			}
		}
		
		return true;
	}
	else
	{
		for (int i = 0; i < item->size; i++)
		{
			GuiSlot* tmp = getSlotFromCoord(coord + item->coords[i]);

			if (tmp == NULL || tmp->inventory_item)
			{
				if (tmp && tmp->inventory_item)
				{
					if ((*extra_item) == NULL)
					{
						*extra_item = tmp->inventory_item;
					}
					else if (*extra_item != tmp->inventory_item)
					{
						SetSlotsState(item, RED);
						return false;
					}
				}
				else if (!tmp)
				{
					return false;
				}
			}
		}

		return true;
	}
	//--------------

}

//Returns a pointer to the slot giving it's coord in the inventory
GuiSlot* GuiInventory::getSlotFromCoord(iPoint& coord)
{
	if (CoordExist(coord))
	{
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i].coords == coord)
			{
				return &slots[i];
			}
		}
	}

	return NULL;
}

//Returns the Coord that the item would have if placed on the inventory;
iPoint GuiInventory::getCoordFromItem(GuiItem* item)
{
	iPoint ret(0, 0);
	//TODO 5:Fill getCoordFromItem() function so, from a given item, returns the coordinate of the slot where the item's pivot is placed 
	//(and so, where the item will be placed by reference slot)
	// Hint : this is an easy one, just 2 lines of code :)
	// Hint 2: to know more where this is working, see at void GuiInventory::CheckDraggedItem(GuiItem* drag_item)

	ret = item->getPivotPosition();
	ret = ScreenToSlot(ret);
	
	return ret;
}

//Checks if the coord exists in the inventory space
bool GuiInventory::CoordExist(iPoint& coord)
{
	if (coord.x >= 0 && coord.x < columns && coord.y >= 0 && coord.y < rows)
	{
		return true;
	}

	return false;
}

//Deletes all items (only for this prototype)
void GuiInventory::CleanItems()
{
	list<GuiItem*>::iterator it = items.begin();
	for (; it != items.end(); it++)
	{
		RELEASE(*it);
	}

	items.clear();
}

//Slot coloring
void GuiInventory::SetSlotsState(GuiItem* item, SLOT_STATE state)
{
	//TODO 8 : Make the function SetSlotsState, so from a given item, set all the slots related to this item to a given state
	//Then put, this function where it's necessary and with the correct item and slot_state, so it follows the following rules:
	//State GREEN: when the item under the mouse can get picked , and when draggin an item, you can place the item at the slots under it
	//State RED: when you can't place the dragged item at the slots under it
	//State YELLOW: when you can exchange the dragged item for the item under it (the slots colored are the ones from the item that can be exchanged)
	//For seeing how the slot state works see here void GuiSlot::draw() (it's a just a swith)


	for (int i = 0; i < item->size; i++)
	{
		GuiSlot* slot = getSlotFromCoord(item->coords[i] + getCoordFromItem(item));
		if (slot)
			slot->state = state;
	}
}


void GuiInventory::addPotion(GuiItem* item)
{
	if ((*items.begin())->quantity < (*items.begin())->maxquantity)
	{
		(*items.begin())->quantity++;
	}
}


int GuiInventory::getQuantity()
{
	int ret = 0;

	if (!items.empty())
	{
		ret = (*items.begin())->quantity;
	}

	return ret;
}