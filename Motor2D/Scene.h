#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

struct Sprite;
class GuiElement;
class GuiLabel;
class GuiImage;
class GuiInputBox;
class GuiSlider;

class Scene : public Module
{
public:

	// Constructor
	Scene(){}

	// Destructor
	virtual ~Scene(){}

	// Called before render is available
	virtual bool awake()
	{
		return true;
	}

	// Called before the first frame
	virtual bool start()
	{
		return true;
	}

	// Called before all updates
	virtual bool preUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool update(float dt)
	{
		return true;
	}

	// Called before all updates
	virtual bool postUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool cleanUp()
	{
		return true;
	}

	//Called on certain event
	virtual void OnEvent(GuiElement* element, GUI_Event even){}

	//Load and unLoad NOTE:(maybe there's no need to make it virtual)
	virtual bool Load()
	{
		return true;
	}

	virtual bool unLoad()
	{
		cleanUp();
		return true;
	}

private:
};

#endif // __SCENE_H__