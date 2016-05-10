#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL\include\SDL.h"
#include "p2Point.h"
#include "Animation.h"

struct Sprite;
class Collider;

enum ENTITY_TYPE
{
	ENEMY,
	NPC,
	PROP,
	ITEM,
	ITEM_HEALTH,
	PORTAL
};

class Entity
{

public:

	//Constructors
	Entity(const iPoint &p, uint ID);

	//Destructor
	~Entity();

	//update
	virtual bool update(float dt) { return true; }

	//draw
	virtual void draw() {};

	//Debug draw
	virtual void drawDebug() {};

	//getters
	iPoint		getMapPosition() const;
	iPoint		getTilePosition() const;
	iPoint		getBlitPosition() const;
	fPoint		getPivotPosition() const;
	SDL_Rect	getPlayerRect() const;

	
public:

	//Attributes
	SDL_Rect		collider_rect;
	SDL_Rect		sprite_rect;
	iPoint			sprite_dim;
	ENTITY_TYPE		type;
	iPoint			colliderOffset;
	iPoint			sprite_pivot;
	fPoint			position;
	uint			id;

	Sprite*			sprite;

	SDL_Texture*	tex;

	Collider*		collider;

};

#endif