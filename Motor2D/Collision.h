#ifndef __Collision_H__
#define __Collision_H__

#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"

using namespace std;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,

	COLLIDER_PLAYER = 0,
	COLLIDER_ENEMY = 1,
	COLLIDER_PLAYER_PARTICLE = 2,
	COLLIDER_ENEMY_PARTICLE = 3,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete;
	COLLIDER_TYPE type;
	Module* callback;

	Collider(SDL_Rect SDL_Rect, COLLIDER_TYPE type, Module* callback = NULL) : rect(SDL_Rect), type(type), callback(callback), to_delete(false)
	{ }

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(SDL_Rect &r) const;
};

class Collision : public Module
{

private:

	list<Collider*> colliders;

	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;

public:

	Collision();

	//Destructor
	~Collision();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Call before first frame
	bool start();

	// Called before all updates
	bool preUpdate();

	// Called each loop iteration
	bool update(float dt);

	// Called before quitting
	bool cleanUp();

	Collider* addCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback = NULL);
	void drawDebug(Collider* col);
};

#endif  __Collision_H__