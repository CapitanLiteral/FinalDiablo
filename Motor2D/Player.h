#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "p2Point.h"
#include "Animation.h"
//WARNING provisional sdl including
#include "SDL/include/SDL.h"
#include "Timer.h"
//#include "PlayerSkills.h"
#include <vector>
#include <list>
#include <map>

using namespace std;

//NOTE: player speed, put it at the config file
#define PLAYER_SPEED 200.0f
#define PLAYER_RUN_SPEED 250.0f
#define DIRECTIONS 8
#define PLAYER_SPRITE_W int (96)
#define PLAYER_SPRITE_H int (92)
#define SPRITE_MARGIN int(1)
#define PLAYER_PIVOT_OFFSET int(10)
#define STAMINA_SPEED 0.1f


//NOTE: provisional, this will go somewhere alse to apply to all entities, or maybe deleted because the state machine
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

enum PLAYER_EVENT
{
	GET_DAMAGE,
	STATE_CHANGE,
	TELEPORT
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

class EntEnemy;
class EntItem;
class Entity;
class Sprite;
class playerParticle;
class Scene;
class Collider;

class Player : public Module
{
//Methods

public:
	Player();

	//Destructor
	virtual ~Player();

	// Called before render is available
	bool awake(pugi::xml_node& conf);

	// Called the first frame
	bool start();

	//preUpdate
	bool preUpdate();

	//update
	bool update(float dt);

	//postUpdate
	bool postUpdate();

	//Draws the player each loop iteration
	void draw();

	// Called before quitting
	bool cleanUp();

	//Debug mode
	void drawDebug() const ;

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

	bool RunOn();
	
	//Attack
	bool IsInRange(Entity* enemy);

	bool Alive();

	void Respawn();

	//NOTE: some of these may go to the entities
	//getters
	iPoint		getMapPosition() const;
	iPoint		getTilePosition() const;
	iPoint		getBlitPosition() const;
	fPoint		getPivotPosition() const;
	SDL_Rect	getPlayerRect() const;

	//Estructuralfunctions
	//void SetAnimations();
	void SetDirection();
	void SetDirection(fPoint pos);
	void SetPosition(fPoint pos);

	//StateMachine functions
	void StateMachine();

	//Utils
	void PlayerEvent(PLAYER_EVENT even);

	//Input
	void HandleInput();

	
	Scene* getDestiny();
	bool TeleportReady();
	void ResetTeleport();

	//Collider
	void OnCollision(Collider* c1, Collider* c2);

//Attributes

public:
	bool		running = false;
	
public:

	//Position
	fPoint p_position;
	iPoint p_pivot;
	
	//Movement
	//NOTE: All this will be used with the moving unities too
	vector<iPoint> path;
	iPoint		p_target;
	int			p_current_node;
	//NOTE: the declaration will go somewhere else
	float		target_radius = 2.5f;
	fPoint		p_velocity;
	float		p_speed = PLAYER_SPEED;
	bool		movement;
	bool		target_reached;
	bool		path_on = true;

	//Attack
	bool		attacking;
	float attack_range = 65.0f;


	//StateMachine Attributes
	ACTION_STATE	current_action;
	DIRECTION		current_direction;
	ACTION_STATE	previous_action = NOTHING;
	bool			input_locked = false;
	Timer			respawn_timer;

	//NOTE: will be cahnged to a enemy once entity manager structure is changed
	//Enemy target
	EntEnemy* enemy = NULL;
	Entity*	objective = NULL;

	//NOTE: Portal attributes, may be changed;
	bool teleport = false;
	Scene* scene_to_teleport = NULL;

	//Collider
	Collider* p_collider;

	//--------------------------------------
	//Animations and sprites section
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
	SDL_Texture* p_debug = NULL;

	
};

#endif // __PLAYER_H__

