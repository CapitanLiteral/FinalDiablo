#ifndef __GAME_H__
#define __GAME_H__

#include "Module.h"
#include <list>

using namespace std;

class Player;
class PlayerAttributes;
class Hud;
class EntityManager;

class Game : public Module
{
public:
	Game();

	//Destructor
	~Game();

	// Called before render is available
	bool awake(pugi::xml_node& conf);

	// Called before the first frame
	bool start();

	//preUpdate
	bool preUpdate();

	//update
	bool update(float dt);

	//postUpdate
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	void draw();

	//Utils
	void addModule(Module* module);

public:
	//Modules

	Player*			player = NULL;
	Hud*			hud = NULL;
	EntityManager*	em = NULL;


	bool        pause = false;

private:
	list<Module*>	game_modules;
};

#endif __GAME_H__