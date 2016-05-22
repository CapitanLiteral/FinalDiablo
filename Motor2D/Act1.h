#ifndef _Act1_H
#define _Act1_H

#include "Scene.h"
#include "ParticleManager.h"
#include "p2Point.h"
#include <list>

using namespace std;

struct Sprite;
struct SDL_Texture;
struct SDL_Rect;
class GuiElement;
class Entity;
class Map;
class Particle;
class Collider;


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
	//Collisions
	void OnCollision(Collider* c1, Collider* c2);

	//NOTE: provisional.
	SDL_Texture* debug = NULL;

	void createHollyFire();

	list<Entity*>		entity_list;

	bool win;

	Entity* counselor;
	Entity* healer;

private:
	SDL_Texture* wallsTexture = nullptr;
	Sprite* walls;

	Particle hFire;
	Particle* hollyFire=NULL;

	Particle hFire1;
	Particle* hollyFire1 = NULL;

	Particle hFire2;
	Particle* hollyFire2 = NULL;

	Particle hFire3;
	Particle* hollyFire3 = NULL;

	Particle hFire4;
	Particle* hollyFire4 = NULL;

};

#endif _Act1_H