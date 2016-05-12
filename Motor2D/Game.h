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
	~Game();

	bool awake(pugi::xml_node& conf);
	bool start();
	bool preUpdate();
	bool update(float dt);
	bool postUpdate();
	bool cleanUp();

public:

	Player*			player = NULL;
	Hud*			hud = NULL;
	EntityManager*	em = NULL;

	bool        pause = false;
};

#endif __GAME_H__