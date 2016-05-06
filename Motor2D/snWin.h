#ifndef _SNWIN_H_
#define _SNWIN_H_

#include "Scene.h"
#include "Timer.h"
#include <list>

using namespace std;

class Entity;


class snWin : public Scene
{
public:
	// Constructor
	snWin();

	// Destructor
	~snWin();

	// Called before render is available
	bool awake(pugi::xml_node& conf);

	// Called the first frame
	bool start();

	// preUpdate
	bool preUpdate();

	// update
	bool update(float dt);

	// postUpdate
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Called on certain event
	void OnEvent(GuiElement* element, GUI_Event even);

	//Load/unLoad, called when the scene changes
	bool Load();
	bool unLoad();

	//adding Enemies
	void addEnemies();

private:


public:
	//NOTE: provisional.
	SDL_Texture* back = NULL;
	Timer ret;



};

#endif _SNWIN_