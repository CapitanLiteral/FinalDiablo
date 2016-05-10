#ifndef __ENT_ENEMY_H__
#define __ENT_ENEMY_H__

#include "EntMobile.h"
#include "Timer.h"
#include "Animation.h"
#include <map>
#include "p2SString.h"

enum ENTITY_STATE;

enum ENTITY_DIRECTION;

enum ENEMY_TYPE
{
	ENEMY_WOLF,
	ENEMY_PALADIN,
	ENEMY_GRISWOLD/*,
	ENEMY_COUNCIL,*/
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

	bool mouseHover();
	
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
	float	vision;

	ENEMY_TYPE enemy_type;
	int		damage;

	const char*  name;
	p2SString nameString;

};

class EnemyPaladin : public EntEnemy
{
public:

	//Constructor
	EnemyPaladin(const iPoint &p, uint ID);

	bool update(float dt);

	ENTITY_STATE updateAction();
	void EntityEvent(ENTITY_EVENT even);
	void StateMachine();

public:
	uint last_update;
	std::map<std::pair<ENTITY_STATE, ENTITY_DIRECTION>, Animation>	*paladinAnim;
	
};

class EnemyWolf : public EntEnemy
{
public:

	//Constructor
	EnemyWolf(const iPoint &p, uint ID);

	bool update(float dt);

	ENTITY_STATE updateAction();
	void EntityEvent(ENTITY_EVENT even);
	void StateMachine();

public:
	uint last_update;
	std::map<std::pair<ENTITY_STATE, ENTITY_DIRECTION>, Animation>	*wolfAnim;

};

class EnemyGriswold : public EntEnemy
{
public:

	//Constructor
	EnemyGriswold(const iPoint &p, uint ID);

	bool update(float dt);

	ENTITY_STATE updateAction();
	void EntityEvent(ENTITY_EVENT even);
	void StateMachine();

public:
	uint last_update;
	std::map<std::pair<ENTITY_STATE, ENTITY_DIRECTION>, Animation>	*griswoldAnim;

};

/*

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
	
};*/
#endif