#ifndef _HUDELEMENT_H_
#define _HUDELEMENT_H_

#include "Module.h"
#include <vector>

using namespace std;

class GuiElement;
enum GUI_Event;


class hudElement : public Module
{
public:
	
	hudElement(): Module()
	{

	}
	//Called before fist frame
	bool start()
	{
		return true;
	}

	//Called before each loop iteration
	bool preUpdate()
	{
		return true;
	}

	//Called each frame
	bool update(float dt)
	{
		return true;
	}

	//Called after each loop iteration
	bool postUpdate()
	{
		return true;
	}

	//Called before quitting
	bool cleanUp()
	{
		return true;
	}

	virtual void OnEvent(GuiElement* element, GUI_Event even)
	{
	
	}

	virtual void Activate()
	{
		active = !active;
	}

public:
	
	bool active;
	vector<GuiElement*> Hud_gui_elements;
};

#endif _HUDELEMENT_H_