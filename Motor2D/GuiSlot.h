#ifndef _GUISLOT_H_
#define _GUISLOT_H_

#include "GuiElements.h"

class GuiItem;
class Module;

enum SLOT_STATE
{
	DEFAULT,
	GREEN,
	RED,
	YELLOW,
};

class GuiSlot : public GuiElement
{
public:
	GuiSlot(iPoint p, SDL_Rect r, GuiElement* parent, Module* listener);
	~GuiSlot();

	void update(GuiElement* hover, GuiElement* focus);
	void draw();
	void drawDebug();

public:
	iPoint coords;
	GuiItem* inventory_item;
	SLOT_STATE state;
};

#endif _GUISLOT_H_