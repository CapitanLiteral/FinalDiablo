#ifndef _HUDBELT_H_
#define _HUDBELT_H_

#include "hudElement.h"
#include "SDL\include\SDL.h"
#include "p2Point.h"

enum GUI_EVENT;
class Hud;
class GuiElement;
class GuiImage;
class GuiLabel;
class GuiInventory;
class Player;
class PlayerAttributes;


#define STAT_LOCAL_Y 34
#define STAT_TEX_Y 196
#define STAT_MAX_H float(78)
#define STAT_MAX_W float(102)

//NOTE : Do Hud Elements
class hudBelt : public hudElement
{
public:

	//Constructor
	hudBelt();

	//Destructor
	~hudBelt();

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

	//Use inventory item in belt
	bool useBeltInvItem(GuiInventory* inv = NULL);

public:

	PlayerAttributes* character = NULL;

	//Some buttons bools
	bool minipanel_pressed = false;
	bool attack1_pressed = false;
	bool attack2_pressed = false;
	bool inventorybutton_pressed = false;
	bool skilltreebutton_pressed = false;

	iPoint lifePos;
	iPoint ragePos;

	SDL_Rect lifeRect;
	SDL_Rect rageRect;
	SDL_Rect staminaRect;

	//Belt Gui Elements
	GuiImage* Hud = NULL;
	GuiImage* HUDornament = NULL;
	GuiImage* minipanel = NULL;
	GuiImage* HUDback1 = NULL;
	GuiImage* HUDback2 = NULL;

	GuiImage* life = NULL;
	GuiImage* rage = NULL;
	GuiImage* stamina = NULL;
	GuiImage* life_holder = NULL;
	GuiImage* rage_holder = NULL;

	GuiImage* runbutton = NULL; // man in black
	GuiImage* minipanelbutton = NULL; // pointing up

	GuiImage* stats = NULL;
	GuiImage* inventorybutton = NULL;
	GuiImage* skilltree = NULL;
	GuiImage* map = NULL;
	GuiImage* game_menu = NULL;

	GuiImage* attack1 = NULL;
	GuiImage* attack2 = NULL;
	GuiImage* skill11 = NULL;
	GuiImage* skill12 = NULL;
	GuiImage* skill13 = NULL;
	GuiImage* skill21 = NULL;
	GuiImage* skill22 = NULL;
	GuiImage* skill23 = NULL;
	
	GuiInventory* inventory1 = NULL;
	GuiInventory* inventory2 = NULL;
	GuiInventory* inventory3 = NULL;
	GuiInventory* inventory4 = NULL;
};


#endif _HUDBELT_H_