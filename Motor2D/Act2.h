#ifndef _Act2_H
#define _Act2_H

#include "Scene.h"
#include "p2Point.h"
#include <list>

using namespace std;

struct Sprite;
struct SDL_Texture;
struct SDL_Rect;
class GuiElement;
class Entity;


class Act2 : public Scene
{
public:
	Act2();
	~Act2();
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

	//NOTE: provisional.
	SDL_Texture* debug = NULL;

	list<Entity*>		entity_list;

	bool win;
};

#endif _Act2_H