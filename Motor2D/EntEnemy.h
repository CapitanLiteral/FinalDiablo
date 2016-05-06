#ifndef __ENT_ENEMY_H__
#define __ENT_ENEMY_H__

#include "EntMobile.h"
#include "Timer.h"

enum ENEMY_TYPE
{
	ENEMY_WOLF,
	ENEMY_CRAWLER,
	ENEMY_COUNCIL,
};

class Player;

#define PATHFINDING_FRAMES 60


class EntEnemy : public EntMobile
{
public:

	//Constructor
	EntEnemy(const iPoint &p, uint ID);

	//Drawing methods
	void draw();
	void drawDebug();

	bool PlayerInRange();
	bool PlayerInAttackRange();

	//Damage
	void TakeDamage(int damage);
	void DrawHPbar();
	
	//Attack
	void updateAttack();
	void CheckToAttack();

	
public:

	//Attributes

	//Health
	int		HP_max;
	int		HP_current;

	//Attack
	float	attack_range;
	bool attacking;
	//NOTE: in the future it will be an entity
	Player* enemy;

	//Detection
	float	agro_range;

	ENEMY_TYPE enemy_type;
	int		damage;

	//Pure Blood System;
	int		blood_drop;

	const char*  name;

};


class EntEnemyWolf : public EntEnemy
{
public:

	//Constructor
	EntEnemyWolf(const iPoint &p, uint ID);

	bool update(float dt);

	ENTITY_STATE updateAction();
	void EntityEvent(ENTITY_EVENT even);
	void StateMachine();

	void SetAnimations();

public:
	uint last_update;

};


class EntEnemyCrawler : public EntEnemy
{
public:

	//Constructor
	EntEnemyCrawler(const iPoint &p, uint ID);

	bool update(float dt);

	ENTITY_STATE updateAction();
	void EntityEvent(ENTITY_EVENT even);
	void StateMachine();

	void SetAnimations();

public:
	uint last_update;
	
};


class EntEnemyCouncil : public EntEnemy
{
public:

	//Constructor
	EntEnemyCouncil(const iPoint &p, uint ID);

	bool update(float dt);

	ENTITY_STATE updateAction();
	void EntityEvent(ENTITY_EVENT even);
	void StateMachine();

	void SetAnimations();

public:
	uint last_update;
	Timer win;
	
};
#endif