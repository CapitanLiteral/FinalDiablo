#pragma once
#include "Module.h"
#include "p2Point.h"
#include "Animation.h"

#include <map>


class PlayerAttributes;

class Collider;

class Sprite;
class SDL_Texture;
enum ACTION_STATE;
enum DIRECTION;
enum PHASE;
enum INPUT_EVENTS;
enum SKILLS;

class Player : public Module
{
//Constructor and destructor
public:
	Player();
	~Player();

//Player things
public:
	PlayerAttributes* attributes = NULL;
	iPoint worldPosition;	// position in pixels of player // This is the point of the pivot in the world //The feet of the player
	iPoint mapPosition;		// position of the tile where player is

//
public:
	bool start();
	bool awake(pugi::xml_node &config);
	bool preUpdate();
	bool update(float dt);
	void draw();

//Player interm way of life
private:
	Collider* collider = NULL; // This must be updated every time player position changes
	iPoint colliderOffset;
	iPoint colliderSize;
	bool alive;
	iPoint startingPosition;
	bool inputBlocked; // to do PAUSE // need getter and setter
	INPUT_EVENTS current_input_event;
//Player movement
private: //Do all getters and setters
	std::vector<iPoint> path;
	iPoint target;
	fPoint velocity;
	int currentNode;
	float targetRadius = 3.0f; //Maybe ERROR, watch out
	
	bool movement;
	bool targetReached;

	vector<iPoint> getNewPath(iPoint target);
	void getNewTarget();
	void setTarget(iPoint target);
	void updateVelocity(float dt);
	void updateMovement(float dt);
	bool isTargetReached();
	void move(float dt);

private:
	void respawn();
	void handleInput();
	ACTION_STATE updateAction();
	//void SetDirection();
	void SetDirection(fPoint pos);

//This have nosense in my opinion by CapitánLiteral
public:
	fPoint getPivotPosition();

public:
	bool Alive();
	
//getters and setters
public:
	iPoint getMapPosition() const;
	iPoint getWorldPosition() const;
	iPoint getStartingWorldPosition() const;

	void setMapPosition(iPoint tile);
	void setWorldPosition(iPoint coords);
	void setStartingWorldPosition(iPoint coords);

	Collider* getCollider() const;
	iPoint getColliderOffset() const;
	iPoint getColliderSize() const;

	Sprite* getSprite() const;

	//Getters and setters of this shit
	//iPoint target;
	//fPoint velocity;

	//bool movement;
	//bool targetReached;
private:
	void setColliderPosition(iPoint coords);


//Animation things
private:
	Sprite* sprite = NULL;

	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	barbarianAnim;
	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	butcherAnim;
	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	diabloAnim;

	PHASE currentPhase;
	Animation* current_animation;
	ACTION_STATE current_action;
	ACTION_STATE previous_action;
	DIRECTION current_direction;

	SDL_Texture* barbarianImage = NULL;
	SDL_Texture* butcherImage = NULL;
	SDL_Texture* diabloImage = NULL;
	SDL_Texture* pDebug = NULL;

	bool loadAnimations();
};

enum DIRECTION
{
	D_BACK,
	D_BACK_RIGHT,
	D_RIGHT,
	D_FRONT_RIGHT,
	D_FRONT,
	D_FRONT_LEFT,
	D_LEFT,
	D_BACK_LEFT,
	D_DEFAULT
};

enum ACTION_STATE
{
	IDLE = 0,
	WALKING = 1,
	BASIC_ATTACK = 2,
	DEATH = 3,
	RUNNING = 4,
	SKILL, // Do the skill things
	NOTHING,
};

enum PHASE
{
	BARBARIAN,
	BUTCHER,
	DIABLO
};

enum INPUT_EVENTS
{
	I_NULL = 0,
	I_WALK,
	I_RUN,
	I_SKILL,
	I_ATTACK,
	I_STOP,
	I_DIE
};

enum SKILLS
{
	SKILL1,
	SKILL2
};