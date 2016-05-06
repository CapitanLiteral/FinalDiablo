#ifndef _GUIITEM_H_
#define _GUIITEM_H_

#include "GuiElements.h"
#include "p2Point.h"

#define ITEM_SECTION_SIZE 29

class GuiSlot;
class GuiInventory;

class GuiItem : public GuiElement
{
public:
	GuiItem(int s, iPoint* coord, SDL_Rect r);
	~GuiItem();
	
	void draw();
	void drawDebug();
	void update(GuiElement* hover, GuiElement* focus);

	//NOTE: the final GUARRADA
	void Effect();

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
};

#endif _GUISLOT_H_