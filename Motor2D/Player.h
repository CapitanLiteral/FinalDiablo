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


class Player :
	public Module
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
	bool update(float dt);

//Player interm way of life
private:
	Collider* collider = NULL; // This must be updated every time player position changes
	iPoint colliderOffset;
	iPoint colliderSize;
	bool alive;

//This have nosense in my opinion by CapitánLiteral
public:
	fPoint getPivotPosition();

public:
	bool Alive();
	
//getters and setters
public:
	iPoint getMapPosition() const;
	iPoint getWorldPosition() const;

	void setMapPosition(iPoint tile);
	void setWorldPosition(iPoint coords);

	Collider* getCollider() const;
	iPoint getColliderOffset() const;
	iPoint getColliderSize() const;

	Sprite* getSprite() const;

//Animation things

private:
	Sprite* sprite = NULL;

	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	barbarianAnim;
	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	butcherAnim;
	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	diabloAnim;

	bool loadAnimations();

	PHASE currentPhase;
	Animation* current_animation;

	SDL_Texture* barbarianImage = NULL;
	SDL_Texture* butcherImage = NULL;
	SDL_Texture* diabloImage = NULL;
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

