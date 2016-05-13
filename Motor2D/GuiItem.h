#ifndef _GUIITEM_H_
#define _GUIITEM_H_

#include "GuiElements.h"
#include "p2Point.h"

#define ITEM_SECTION_SIZE 29

class GuiSlot;
class GuiInventory;
class PlayerAttributes;

class GuiItem : public GuiElement
{
public:
	GuiItem(int s, iPoint* coord, SDL_Rect r, int quantity = 1, PlayerAttributes* lis = NULL);
	~GuiItem();
	
	void draw();
	void drawDebug();
	void update(GuiElement* hover, GuiElement* focus);

	virtual void Effect(){}
	void Move();
	iPoint getPivotPosition();
	void FreeSlots();

public:

	int				size;
	iPoint*			coords;
	iPoint			pivot;
	GuiSlot*		reference_slot;
	GuiInventory*	inventory;
	GuiImage		image;

	int quantity;
	int maxquantity = 5;

	PlayerAttributes* lis = NULL;
};


class Potion : public GuiItem
{

public:

	Potion(int s, iPoint* coord, SDL_Rect r, int quantity, PlayerAttributes* lis) : GuiItem(s, coord, r, quantity, lis){}
	~Potion(){}

	void Effect();
};




#endif _GUISLOT_H_