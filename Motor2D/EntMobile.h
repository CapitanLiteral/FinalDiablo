#ifndef __ENT_MOBILE_H__
#define __ENT_MOBILE_H__

#include "Entity.h"

enum ENTITY_EVENT
{
	ENTITY_STATE_CHANGE,
};

enum ENTITY_STATE
{
	ENTITY_IDLE,
	ENTITY_WALKING,
	ENTITY_ATTACKING,
	ENTITY_DEATH,
	ENTITY_GET_HIT,
};

enum ENTITY_INPUT
{
	ENTITY_INPUT_MOVE,
	ENTITY_INPUT_STOP_MOVE,
	ENTITY_INPUT_NULL,
	ENTITY_INPUT_ATTACK,
	ENTITY_INPUT_DEATH
};

enum ENTITY_DIRECTION
{
	ENTITY_D_BACK,
	ENTITY_D_BACK_RIGHT,
	ENTITY_D_RIGHT,
	ENTITY_D_FRONT_RIGHT,
	ENTITY_D_FRONT,
	ENTITY_D_FRONT_LEFT,
	ENTITY_D_LEFT,
	ENTITY_D_BACK_LEFT,
	ENTITY_D_DEFAULT
};

class EntMobile : public Entity
{
public:

	//Constructor
	EntMobile(const iPoint &p, uint ID);

	//Destructor
	~EntMobile();

	//Setters
	virtual void SetAnimations() {}
	void SetDirection();

	//Events and states
	virtual void EntityEvent(ENTITY_EVENT even) {}
	virtual void StateMachine() {}

	//Movement
	void Move(float dt);
	void SetMovement(int x, int y);
	void SetInitVelocity();
	void SetTarget(iPoint target);
	void SetNewPath(int x, int y);
	void updateVelocity(float dt);
	void updateMovement(float dt);
	bool IsTargetReached();
	void getNewTarget();
	iPoint FindClosestWalkable(iPoint pos);

public:
	//Attributes
	Animation			current_animation;
	vector<Animation>	idle;
	vector<Animation>	walk;
	//NOTE: not all the mobile entities would do this
	vector<Animation>	attack;
	vector<Animation>	death;

	SDL_Texture*		animations;

	vector<iPoint>		path;
	iPoint				target;
	int					current_node;
	float				target_radius = 2.5f;
	fPoint				velocity;
	float				speed;
	bool				movement;
	bool				target_reached;
	bool				path_on = true;
	bool				dead = false;

	vector<Animation>	current_animation_set;
	ENTITY_STATE		current_action;
	ENTITY_DIRECTION	current_direction;
	ENTITY_STATE		last_action;
	ENTITY_DIRECTION	last_direction;
	ENTITY_INPUT		current_input;
	ENTITY_INPUT		previous_input = ENTITY_INPUT_NULL;

};

#endif