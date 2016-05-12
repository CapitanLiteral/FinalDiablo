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

	void displayDamage(int x, int y, float damage, bool crit = false){}
	void levelChanged(int level, int prev){}

private:

	void clearTabs();
	bool useSlotItem(GuiInventory* inv = NULL);
	void hidePanel();
	void activatePanel();

public:

	// Player dependancies
	PlayerAttributes* playerAtt = NULL;

private:

	bool exit;
	
	// Base
	iPoint lifePos;
	iPoint ragePos;

	iPoint basic_attackPos;
	iPoint frenzyPos;
	iPoint whackPos;
	iPoint growlPos;

	SDL_Rect lifeRect;
	//SDL_Rect evilBarRect;
	SDL_Rect rageRect;
	SDL_Rect staminaRect;

	SDL_Rect menuExpandOpened;
	SDL_Rect menuExpandClosed;

	GuiImage* life = NULL;
	//GuiImage* evilBar = NULL;
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
	GuiImage* pause = NULL;

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
	GuiImage* characterMenu = NULL;

	// Inventory
	GuiImage* inventoryMenu = NULL;

	// Tree
	GuiImage* treeMenu = NULL;

	// Map
	GuiImage* mapMenu = NULL;

	// Pause Menu
	GuiImage* pauseMenu = NULL;
	GuiImage* p_exit = NULL;
	GuiImage* p_back = NULL;
};

#endif _HUD_H_