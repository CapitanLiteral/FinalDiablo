#ifndef __ENT_NPC_H__
#define __ENT_NPC_H__

#include "EntStatic.h"
#include "p2Point.h"
#include "EntityManager.h"
#include "Timer.h"

enum NPC_TYPE
{
	NPC_COUNSELOR,
	NPC_HEALER,
	NPC_GOSSIP,
};

class Collider;
class Sprite;

class EntNpc : public EntStatic
{
public:

	//Constructor
	EntNpc(const iPoint &p, uint ID);

	//Drawing methods
	void draw();
	void drawDebug();

	bool PlayerInRange();

	//getters and setters
	iPoint getMapPosition() const;
	iPoint getWorldPosition() const;

	void setMapPosition(iPoint tile);
	void setWorldPosition(iPoint coords);

	Collider* getCollider() const;
	iPoint getColliderOffset() const;
	iPoint getColliderSize() const;

	Sprite* getSprite() const;
public:

	//Attributes
	Collider* collider = NULL;
	iPoint colliderOffset;
	iPoint colliderSize;

	Sprite* sprite = NULL;

	iPoint worldPosition;
	iPoint mapPosition;

	NPC_TYPE npcType;

	float	playerRange;
};

class EntCounselor : public EntNpc
{
public:

	//Constructor
	EntCounselor(const iPoint &p, uint ID);

	bool update(float dt);

	void SetAnimations();

public:

};


class EntHealer : public EntNpc
{
public:

	//Constructor
	EntHealer(const iPoint &p, uint ID);

	bool update(float dt);

	void SetAnimations();

public:

};

class EntGossip : public EntNpc
{
public:

	//Constructor
	EntGossip(const iPoint &p, uint ID);

	bool update(float dt);

	void SetAnimations();

public:

};



#endif