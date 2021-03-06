#ifndef _HUDINVENTORY_H_
#define _HUDINVENTORY_H_

#include "hudElement.h"

class Hud;
class GuiElement;
class GuiImage;
class GuiInventory;




class hudInventory : public hudElement
{
public:
	//Constructor
	hudInventory();

	//Destructor
	~hudInventory();

	//Called before fist frame
	bool start();

	//Called before each loop iteration
	bool preUpdate();

	//Called each frame
	bool update(float dt);

	//Called after each loop iteration
	bool postUpdate();

	//Called before quitting
	bool cleanUp();

	//Called when there's a gui event
	void OnEvent(GuiElement* element, GUI_Event even);

	//Activation/Desactivation
	void Activate();

	//NOTE: master of nope, but now i need it
	//Change it for an addItem
	bool addPotion();

	/*
	//Potion
	GuiItem* new_item;
	iPoint coords[1] = { { 0, 0 } };
	new_item = new GuiItem(1, coords, { 2285, 799, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE });
	if (!i_inventory->AutomaticaddItem(new_item))
	{
	delete new_item;
	ret = false;
	}
	*/

public:

	GuiImage* background = NULL;
	GuiInventory* inventory = NULL;
	

};


#endif _HUDINVENTORY_H_