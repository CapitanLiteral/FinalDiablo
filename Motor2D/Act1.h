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
	
	//sundcheck
	bool soundcheck = true;

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

	//ROAD1
	Particle hFire;
	Particle* hollyFire = NULL;
	Particle* hollyFire1 = NULL;
	Particle* hollyFire2 = NULL;
	//ROAD2
	Particle* hollyFire3 = NULL;
	Particle* hollyFire4 = NULL;
	Particle* hollyFire5 = NULL;
	Particle* hollyFire6 = NULL;
	Particle* hollyFire7 = NULL;
	Particle* hollyFire8 = NULL;
	//ROAD3
	Particle* hollyFire9 = NULL;
	Particle* hollyFire10 = NULL;
	Particle* hollyFire11 = NULL;
	Particle* hollyFire12 = NULL;
	Particle* hollyFire13 = NULL;
	Particle* hollyFire14 = NULL;
	Particle* hollyFire15 = NULL;
	//ROAD4
	Particle* hollyFire16 = NULL;
	Particle* hollyFire17 = NULL;
	Particle* hollyFire18 = NULL;
	Particle* hollyFire19 = NULL;
	Particle* hollyFire20 = NULL;
	Particle* hollyFire21 = NULL;
	Particle* hollyFire22 = NULL;
	Particle* hollyFire23 = NULL;
	//ROAD5
	Particle* hollyFire24 = NULL;
	Particle* hollyFire25 = NULL;
	Particle* hollyFire26 = NULL;
	Particle* hollyFire27 = NULL;
	Particle* hollyFire28 = NULL;
	Particle* hollyFire29 = NULL;
	Particle* hollyFire30 = NULL;
	//ROAD6
	Particle* hollyFire31 = NULL;
	Particle* hollyFire32 = NULL;
	Particle* hollyFire33 = NULL;
	Particle* hollyFire34 = NULL;
	Particle* hollyFire35 = NULL;
	Particle* hollyFire36 = NULL;

private:
	SDL_Texture* wallsTexture = nullptr;
	Sprite* walls;

private:
    //Fx sounds
	int hollyFireFx;


};

#endif _Act1_H