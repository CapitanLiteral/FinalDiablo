// ----------------------------------------------------
// Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __Module_H__
#define __Module_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class App;
class GuiElement;
enum  GUI_Event;
class Collider;


class Module
{
public:

	Module() : active(false)
	{}

	virtual ~Module()
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool preUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool postUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool cleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnEvent(GuiElement* element, GUI_Event even)
	{}

	//NOTE: provisional for the game system, maybe goes to another kind of module
	virtual void draw()
	{}

	virtual void OnCollision(Collider* c1, Collider* c2)
	{}

public:

	p2SString	name;
	bool		active;

};

#endif // __Module_H__