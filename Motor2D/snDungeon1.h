#ifndef _SNDUNGEON1_H
#define _SNDUNGEON1_H

#include "Scene.h"
#include "p2Point.h"
#include <list>

using namespace std;

struct Sprite;
struct SDL_Texture;
struct SDL_Rect;
class GuiElement;
class Entity;



class snDungeon1 : public Scene
{
public:
	// Constructor
	snDungeon1();

	// Destructor
	~snDungeon1();

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
	SDL_Texture* debug = NULL;

	list<Entity*>		entity_list;

	bool win;
};

#endif _SNDUNGEON1_H