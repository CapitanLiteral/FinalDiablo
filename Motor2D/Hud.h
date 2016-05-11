#ifndef _HUD_H_
#define _HUD_H_

#include "Module.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"
#include <vector>

using namespace std;

enum GUI_Event;
class GuiImage;
class GuiLabel;
class GuiInventory;
class PlayerAttributes;


class Hud : public Module
{
public:
	Hud();
	~Hud();

	bool start();
	bool preUpdate();
	bool update(float dt);

	void OnEvent(GuiElement* element, GUI_Event even);

public:

	void displayDamage(int x, int y, float damage, bool crit){}
	void levelChanged(int level, int prev){}
	bool useSlotItem(GuiInventory* inv = NULL);

public:

	// Player dependancies
	PlayerAttributes* playerAtt = NULL;

private:
	
	// Base
	iPoint lifePos;
	iPoint ragePos;

	SDL_Rect lifeRect;
	SDL_Rect rageRect;
	SDL_Rect staminaRect;

	GuiImage* life = NULL;
	GuiImage* rage = NULL;
	GuiImage* stamina = NULL;

	GuiLabel* lifeLabel = NULL;
	GuiLabel* rageLabel = NULL;

	GuiImage* staminaDorn = NULL; // man in black
	GuiImage* menuExpand = NULL; // pointing up

	GuiImage* panel = NULL;
	GuiImage* character = NULL;
	GuiImage* inventory = NULL;
	GuiImage* tree = NULL;
	GuiImage* map = NULL;
	GuiImage* pauseMenu = NULL;

	GuiImage* current_skill = NULL;
	GuiImage* basic_attack = NULL;
	GuiImage* frenzy = NULL;
	GuiImage* whack = NULL;
	GuiImage* growl = NULL;

	GuiInventory* slot1 = NULL;
	GuiInventory* slot2 = NULL;
	GuiInventory* slot3 = NULL;
	GuiInventory* slot4 = NULL;
	
	// Character


	// Inventory


	// Tree


	// Map


	// Pause Menu
	GuiImage* pause = NULL;
	GuiImage* p_exit = NULL;
	GuiImage* p_back = NULL;
};

#endif _HUD_H_