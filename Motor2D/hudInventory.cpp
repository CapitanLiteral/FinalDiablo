#include "hudInventory.h"
#include "App.h"
#include "Gui.h"
#include "Input.h"
#include "GuiItem.h"
#include "GuiInventory.h"



//Constructor
//Don't now why the code makes me put semicolons everywhere
;
hudInventory::hudInventory() : hudElement()
{

}

//Destructor
hudInventory::~hudInventory()
{

}

//Called before fist frame
bool hudInventory::start()
{
	active = false;
	
	background = app->gui->addGuiImage({ 321, 0 }, { 1128, 588, 319, 430 }, NULL, this);
	background->active = false;
	Hud_gui_elements.push_back(background);
	
	//Creation of the inventory
	inventory = app->gui->addGuiInventory({ 16, 255 }, { 1144, 843, 290, 116 }, 10, 4, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE, background, this);
	inventory->active = false;
	Hud_gui_elements.push_back(inventory);
	
	return true;
}

//Called before each loop iteration
bool hudInventory::preUpdate()
{
	//adding different item to the inventory
	/*if (app->input->getKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		//Potion
		GuiItem* new_item;
		iPoint coords[1] = { { 0, 0 } };
		new_item = new GuiItem(1, coords, { 2285, 799, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE });
		if (!inventory->AutomaticaddItem(new_item))
		{
			delete new_item;
		}
	
	}

	if (app->input->getKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		//Gauntlets
		GuiItem* new_item;
		int size = 4;
		iPoint coords[4] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
		new_item = new GuiItem(4, coords, { 2286, 740, ITEM_SECTION_SIZE*2, ITEM_SECTION_SIZE*2 });
		if (!inventory->AutomaticaddItem(new_item))
		{
			delete new_item;
		}

	}

	if (app->input->getKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		//Wand
		GuiItem* new_item;
		const int size = 2;
		iPoint coords[size] = { { 0, 0 }, { 0, 1 }};
		new_item = new GuiItem(size, coords, { 2315, 799, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE * 2});
		if (!inventory->AutomaticaddItem(new_item))
		{
			delete new_item;
		}

	}

	if (app->input->getKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		//Axe
		GuiItem* new_item;
		const int size = 6;
		iPoint coords[size] = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 }, { 1, 2} };
		new_item = new GuiItem(size, coords, { 2345, 740, ITEM_SECTION_SIZE * 2, ITEM_SECTION_SIZE * 3 });
		if (!inventory->AutomaticaddItem(new_item))
		{
			delete new_item;
		}

	}*/
	return true;
}

//Called each frame
bool hudInventory::update(float dt)
{
	return true;
}

//Called after each loop iteration
bool hudInventory::postUpdate()
{
	return true;
}

//Called before quitting
bool hudInventory::cleanUp()
{
	//WARNING: can't do this plox
	app->gui->dragged_item = NULL;
	inventory = NULL;

	return true;
}

void hudInventory::Activate()
{
	active = !active;

	for (int i = 0; i < Hud_gui_elements.size(); i++)
	{
		Hud_gui_elements[i]->active = active;
	}


}

bool hudInventory::addPotion()
{
	bool ret = true;
	//Potion
	GuiItem* new_item;
	iPoint coords[1] = { { 0, 0 } };
	new_item = new GuiItem(1, coords, { 2285, 799, ITEM_SECTION_SIZE, ITEM_SECTION_SIZE });
	if (!inventory->AutomaticaddItem(new_item))
	{
		delete new_item;
		ret = false;
	}

	return ret;
}

//Called when there's a gui event
void hudInventory::OnEvent(GuiElement* element, GUI_Event even)
{

}
