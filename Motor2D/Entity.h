#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "p2Point.h"
#include <map>
#include "Animation.h"
#include "Collision.h"

class Sprite;
class Attributes;
class Player;

enum entityType
{
	UNKNOWN,
	PALADIN,
	WOLF,
	GRISWOLD,
	NPC_COUNCELOUR,
	NPC_HEALER,
	NPC_GOSSIP,
	DUMMY
};

enum entityState
{
	E_IDLE = 0,
	E_WALK,
	E_BASIC_ATTACK,
	E_DEATH,
	E_WAIT_TO_ATTACK,
	E_INACTIVE
};

enum entityInputState
{
	EI_NULL = 0,
	EI_WALK,
	EI_ATTACK,
	EI_STOP,
	EI_DIE
};

enum entityDirection
{
	E_UP = 0,
	E_UP_RIGHT = 1,
	E_RIGHT = 2,
	E_DOWN_RIGHT = 3,
	E_DOWN = 4,
	E_DOWN_LEFT = 5,
	E_LEFT = 6,
	E_UP_LEFT = 7
};

class Entity
{
public:
	Entity();

	virtual ~Entity();

	virtual bool entityUpdate(float internDT);

	virtual bool entityPostUpdate();

protected:
	//Entity dades
	uint id;
	iPoint worldPosition;
	iPoint mapPosition;
	

	bool alive;

	iPoint colliderOffset;
	iPoint colliderSize;

	Collider* collider = NULL;

public:
	Attributes* attributes = NULL;
	entityType type = UNKNOWN;

//Generic methods----------------------------
//Getters
public:
	Collider* getCollider()const;
	uint getId()const;

	iPoint getWorldPosition()const;
	iPoint getMapPos()const; 
	iPoint getStartingWorldPosition() const;

	iPoint getColliderOffset() const;
	iPoint getColliderSize() const;

	Sprite* getSprite() const;

//Setters
	void setMapPosition(iPoint tile);
	void setWorldPosition(iPoint coords);
	void setStartingWorldPosition(iPoint coords);
	void setColliderPosition(iPoint coords);
	void setId(int id);

//-------------------------------------------

	bool mouseHover();

//Draw------------------------------------
public:
	virtual void draw();
	virtual void drawDebug();
//----------------------------------------


//MOVEMENT--------------------------------
private:
	std::vector<iPoint> path;
	iPoint target;

	fPoint velocity;
	int currentNode;
	float targetRadius = 30.0f; //Maybe ERROR, watch out

	bool movement;
	bool targetReached;

//Movement methods
	vector<iPoint> getNewPath(iPoint target);
	void getNewTarget();
	void setTarget(iPoint target);
	void updateVelocity(float dt);
	void updateMovement(float dt);
	bool isTargetReached();
	void move(float dt);
	void setMovement(int x, int y);
	bool isInDestiny();

	void setDirection();
	void setDirection(fPoint pos);

	entityState updateAction();
	void handleInput();

//---------------------------------------

//Attack---------------------------
	Player* player = NULL;


//---------------------------------

//Visual----------------------------------------------
protected:
	Sprite* imageSprite = NULL;
	iPoint spritePivot;
	SDL_Rect spriteRect;
private:
	
	std::map<std::pair<entityState, entityDirection>, Animation>*	entityAnim = NULL;

	Animation* currentAnimation = NULL;

//Enum instances
	entityInputState current_input = EI_NULL;
	entityState currentState = E_IDLE;
	entityState previousState = E_IDLE;
	entityDirection direction = E_DOWN;

//--------------------------------------
};

#endif