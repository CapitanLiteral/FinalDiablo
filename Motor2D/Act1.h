#ifndef _Act1_H
#define _Act1_H

#include "Scene.h"
#include "p2Point.h"
#include <list>

using namespace std;

struct Sprite;
struct SDL_Texture;
struct SDL_Rect;
class GuiElement;
class Entity;
class Map;


class Act1 : public Scene
{
public:
	Act1();
	~Act1();
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
	
	void createProps();

private:

	Sprite* propProva;
	Sprite* propParet;
	Sprite* propParet1;
	Sprite* fire;
	Sprite* woodHouse;
	Sprite* wood;
	Sprite* woodUp;
	Sprite* carriage;
	Sprite* stuff;
	Sprite* carriage1;
	Sprite* treeCut;
	Sprite* tent;
	Sprite* wood1;
	Sprite* wood2;
	Sprite* wood3;
	Sprite* woodUp1;
	Sprite* tent1;
	Sprite* carriage2;
	Sprite* carriage3;
	Sprite* carriage4;
	SDL_Texture* propAtlas = nullptr;
	std::string propFileName;

};

#endif _Act1_H