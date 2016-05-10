#ifndef _HUD_H_
#define _HUD_H_

#include "Module.h"
#include <vector>

using namespace std;

enum GUI_Event;

class hudElement;
class hudBelt;
class hudPause;
class hudInventory;
class hudBlood;
class hudSkilltree;


class Hud : public Module
{
public:
	Hud();

	~Hud();
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

	//Called when there's a GUI event
	void OnEvent(GuiElement* element, GUI_Event even);

	// Unit damaged
	void displayDamage(int x, int y, float damage, bool crit){}
	void levelChanged(int level){}

public:
	//Hud main belt
	hudBelt* belt = NULL;
	hudPause* pause_menu = NULL;
	hudInventory* inventory = NULL;
	hudBlood* blood = NULL;
	hudSkilltree* skilltree = NULL;
	vector<hudElement*> Hud_elements;
};

#endif _HUD_H_