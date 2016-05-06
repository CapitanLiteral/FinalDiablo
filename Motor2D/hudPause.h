#ifndef _HUDPAUSE_H_
#define _HUDPAUSE_H_

#include "hudElement.h"

class Hud;
class GuiElement;
class GuiImage;




class hudPause : public hudElement
{
public:
	//Constructor
	hudPause();

	//Destructor
	~hudPause();

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

	
	//Utils
	void ActivateMenu();

public:

	bool main_menu;

	//In game menu elements -------
	GuiImage* options = NULL;
	GuiImage* saveandexit = NULL;
	GuiImage* returntogame = NULL;
};


#endif _HUDPAUSE_H_