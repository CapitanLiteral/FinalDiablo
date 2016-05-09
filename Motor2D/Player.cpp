#include "Player.h"
#include "Render.h"
#include "Textures.h"
//NOTE: doubts about including the map to player
#include "Map.h"
#include "App.h"
#include "Game.h"
#include "Input.h"
#include "Hud.h"
#include "Gui.h"
#include "hudBelt.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "hudInventory.h"
#include "hudBlood.h"
#include "Entity.h"
#include "EntEnemy.h"
#include "EntPortal.h"
#include "PlayerSkills.h"
#include "SDL/include/SDL.h"
#include "Audio.h"
#include "FileSystem.h"
#include "p2Log.h"

#include "Collision.h"


Player::Player()
{
	
}

//Destructor
Player::~Player()
{

}

// Called before render is available
bool Player::awake(pugi::xml_node& conf)
{
	return true;
}

// Called the first frame
bool Player::start()
{
	bool ret = true;
	
	ret = loadAnimations();

	//Create skills:
	basic_attack = new sklBasicAttack();

	//
	player_attack = app->audio->LoadFx("audio/fx/PlayerAttack.ogg");
	player_death = app->audio->LoadFx("audio/fx/PlayerDeath.ogg");
	player_gethit = app->audio->LoadFx("audio/fx/PlayergetHit.ogg");

	//Debug tile
	p_debug = app->tex->Load("maps/mini_path.png");
	
	//Sprites
	barbarianImage = app->tex->Load("images/Barbarian.png");
	butcherImage = app->tex->Load("images/Butcher.png");
	diabloImage = app->tex->Load("images/Diablo.png");

	/*p_sprite = p_idle = app->tex->Load("textures/vamp_idle.png");
	p_walk = app->tex->Load("textures/vamp_walk.png");
	p_attack = app->tex->Load("textures/vamp_attack.png");
	p_casting = app->tex->Load("textures/vamp_cast.png");
	p_run = app->tex->Load("textures/vamp_run.png");
	p_death = app->tex->Load("textures/vamp_death.png");
	SetAnimations();*/



	//states
	currentPhase = BARBARIAN;
	previous_action = NOTHING;
	input_locked = false;
	current_action = IDLE;
	current_direction = D_FRONT;
	current_input = INPUT_NULL;
	//current_animation_set = idle;
	current_animation = &barbarianAnim.find({ current_action, current_direction })->second;

	current_skill = left_skill = basic_attack;

	//Positioning
	p_position = { 0, 500 };
	p_pivot = { (PLAYER_SPRITE_W / 2), (PLAYER_SPRITE_H - PLAYER_PIVOT_OFFSET) };
	movement = false;
	attacking = false;
	running = false;

	//Collider
	p_collider = app->collision->addCollider({getPivotPosition().x-20, getBlitPosition().y + 20, 37, getPlayerRect().h - 20}, COLLIDER_PLAYER, this);

	//initial stats
	HP_max = HP_current = 200.0f;
	MP_max = MP_current = 100;
	ST_max = ST_current = 200.0f;
	blood_current = 0;



	//Sprite creation
	SDL_Rect current_sprite = current_animation->getCurrentFrame();

	iPoint pos(p_position.x, p_position.y);
	iPoint pivot(0, 0);
	sprite = new Sprite(barbarianImage, pos, pivot, current_sprite);
	sprite->layer = SCENE;
	app->render->addSpriteToList(sprite);
	
	return ret;
}

//preUpdate
bool Player::preUpdate()
{

	updateAction();

	return true;
}

//update
bool Player::update(float dt)
{
	
	if (current_action != DEATH)
	{
		if (!app->gui->mouse_hovering)
		{
			HandleInput();
		}

		if (current_skill->skill_type == SKILL_MELEE)
		{
			CheckToAttack();
		}

		//NOTE:Make this more elegant
		switch (current_action)
		{
		case IDLE:
			break;
		case WALKING:
			updateMovement(dt);
			break;
		case RUNNING:
			updateMovement(dt);
			//LowerStamina();
			break;
		case SKILL:
			current_skill->skillUpdate();
			break;

		}

		if (current_action != RUNNING)
		{
			//RecoverStamina();
		}
	}
	else
	{
		if (respawn_timer.ReadSec() >= 5)
		{
			Respawn();
		}
	}

		app->render->CenterCamera(p_position.x, p_position.y);

		

	return true;
}

//postUpdate
bool Player::postUpdate()
{
	return true;
}

// Called before quitting
bool Player::cleanUp()
{
	app->tex->unLoad(barbarianImage);
	app->tex->unLoad(butcherImage);
	app->tex->unLoad(diabloImage);
	app->tex->unLoad(p_debug);
	/*app->tex->unLoad(p_idle);
	app->tex->unLoad(p_walk);
	app->tex->unLoad(p_run);
	app->tex->unLoad(p_attack);
	app->tex->unLoad(p_casting);
	app->tex->unLoad(p_death);*/
	
	//Skills deleted
	if (basic_attack)
	{
		RELEASE(basic_attack);
	}	

	//Take an eye on this
	if (sprite)
	{
		app->render->sprites.remove(sprite);
		RELEASE(sprite);
	}

	return true;
}

void Player::Respawn()
{
	//Maybe ERROR, watch out // We need to reset Atributes
	

	//Reset state and animation
	current_action = IDLE;
	current_direction = D_FRONT;
	current_input = INPUT_STOP_MOVE;
	//current_animation_set = idle;
	//current_animation = &current_animation_set[current_direction];


	//Init position and booleans
	//Maybe ERROR, watch out // Must put a correct position
	p_position = { 0, 2000 };
	movement = false;
	attacking = false;
	enemy = NULL;
}

//Draws the player sprite to the scene
//NOTE: had to take out the const because of the animation
void Player::draw() 
{
	if (current_action == SKILL) // TODO: guarrada temporal fins que es toqui el sistema d'atac
	{
		current_action = BASIC_ATTACK;
		if (current_animation->isOver())
			current_animation->Reset();
	}

	switch (currentPhase)
	{
	case BARBARIAN:
		current_animation = &barbarianAnim.find({ current_action, current_direction })->second;
		break;
	case BUTCHER:
		current_animation = &butcherAnim.find({ current_action, current_direction })->second;
		break;
	case DIABLO:
		current_animation = &diabloAnim.find({ current_action, current_direction })->second;
		break;
	}

	//NOTE: for pause mode, this will have to be on update to vary on dt
	if (sprite)
	{
		SDL_Rect section = current_animation->getCurrentFrame();
		iPoint pos(p_position.x, p_position.y);
		iPoint pivot(current_animation->pivot.x, current_animation->pivot.y);
		//iPoint pivot(0, 0);
		sprite->updateSprite(pos, pivot, section);
	}

	//Debug mode
	if (app->debug)
	{
		drawDebug();
	}
	

}

//Debug

void Player::drawDebug() const
{
	iPoint t_pos = getTilePosition();
	fPoint p_pos = getPivotPosition();

	app->render->Blit(p_debug, t_pos.x, t_pos.y);
	//app->render->DrawQuad(getPlayerRect(), 255, 0, 0, 1000, false);
	//app->render->DrawCircle(p_pos.x, p_pos.y, 5, 255, 0, 0, 1000);
	app->render->DrawQuad({ p_pos.x, p_pos.y, 3, 3 }, 255, 0, 0, 255, false);

	
	app->render->DrawCircle(p_pos.x, p_pos.y, attack_range, 255, 0, 0);

	if (movement)
	{
		app->render->DrawLine(p_position.x, p_position.y, p_target.x, p_target.y, 0, 0, 255);
		app->render->DrawLine(p_position.x, p_position.y, p_velocity.x+ p_position.x, p_velocity.y + p_position.y, 0, 255, 255);

		//Path
		for (int i = 0; i < path.size(); i++)
		{
			iPoint tmp = path[i];
			tmp = app->map->getTileBlit(tmp.x, tmp.y);
			app->render->Blit(p_debug, tmp.x, tmp.y);
		}
	}
	
}
/*
//-------getters
*/

//NOTE: some of these things can go to map
iPoint Player::getMapPosition()const
{
	return app->map->WorldToMap(p_position.x , p_position.y);
}

iPoint Player::getTilePosition()const
{
	iPoint ret = getMapPosition();
	ret = app->map->MapToWorld(ret.x, ret.y);
	ret.x -= app->map->data.tile_width / 2;
	return ret;
}

iPoint Player::getBlitPosition()const
{
	fPoint tmp = getPivotPosition();
	iPoint ret(tmp.x, tmp.y);
	/*ret.x -= p_pivot.x;
	ret.y -= p_pivot.y;*/

	return  ret;
}

fPoint Player::getPivotPosition()const
{
	//NOTE : put a tile pivot?, more accesible
	return p_position;
}

SDL_Rect Player::getPlayerRect() const
{
	//NOTE: this may be adapted when we use colliders
	iPoint pos = getBlitPosition();

	return{ pos.x, pos.y, PLAYER_SPRITE_W, PLAYER_SPRITE_H};
}


bool Player::RunOn()
{
	running = !running;
	if (current_action == RUNNING || current_action == WALKING)
	{
		SetInput(INPUT_MOVE);
	}

	return running;
}

/*
//-------Events
*/
//NOTE : this is because in the future, the player will need events, for example, when it get hits, low the HP downor other things
//maybe it ends being provisional
//There's HP_DOWN and HP_UP instead of HP change because it will act different (when down, it's a inmediat change, when up it's slower)
void Player::PlayerEvent(PLAYER_EVENT even)
{
	switch (even)
	{
		case GET_DAMAGE:
		
			break;
		case TELEPORT:
			{
				EntPortal* portal = (EntPortal*)objective;
				objective = NULL;
				teleport = true;
				scene_to_teleport = portal->destiny;
			}
			break;
		case STATE_CHANGE:
				StateMachine();
			break;
	}
}

/*
//-------------------------------------------Linear Movement
*/
void Player::SetInitVelocity()
{
	p_target = app->input->getMouseWorldPosition();

	p_velocity.x = p_target.x - p_position.x;
	p_velocity.y = p_target.y - p_position.y;

	p_velocity.SetModule(p_speed);

}

void Player::Move(float dt)
{
	fPoint vel = p_velocity * dt;

	p_position.x += int(vel.x);
	p_position.y += int(vel.y);

	//NOTE: Collider movement, may be changed
	p_collider->rect.x += int(vel.x);
	p_collider->rect.y += int(vel.y);
}

void Player::updateVelocity(float dt)
{
	p_velocity.x = p_target.x - p_position.x;
	p_velocity.y = p_target.y - p_position.y;

	p_velocity.SetModule(p_speed);

	SetDirection();
}

bool Player::IsTargetReached()
{
	fPoint vel;

	vel.x = p_target.x - p_position.x;
	vel.y = p_target.y - p_position.y;



		if (vel.getModule() <= target_radius)
		{
			if (!path_on)
			{
				SetInput(INPUT_STOP_MOVE);
				movement = false;
			}

			return true;
		}

	return false;
}

void Player::SetTarget(iPoint target)
{
	p_target = target;
	movement = true;
	target_reached = false;
}

void Player::getNewTarget()
{
		if ((uint)p_current_node + 1 < path.size())
		{
			p_current_node++;
			SetTarget(app->map->getTileCenter(path[p_current_node].x, path[p_current_node].y));
		}
		else
		{
			SetInput(INPUT_STOP_MOVE);
			movement = false;
		}
}

void Player::updateMovement(float dt)
{
	if (movement)
	{
		if (!target_reached)
		{
			updateVelocity(dt);
			Move(dt);
			if (IsTargetReached())
				target_reached = true;
		}
		else
		{
			getNewTarget();
		}
	}
}

void Player::SetMovement(int x, int y)
{
	if (path_on)
	{ 
		SetNewPath(x,y);
	}
	else
	{
		iPoint target = app->map->getTileCenter(x, y);
		SetTarget(target);
		
		//StateMachine change
		SetInput(INPUT_MOVE);
	}
}

void Player::SetNewPath(int x, int y)
{
	iPoint start = app->map->WorldToMap(p_position.x, p_position.y);
	iPoint goal = { x, y };

	int steps = app->pathfinding->getNewPath(start, goal, path);

	if (steps > 0)
	{
		//StateMachine change
		current_input = INPUT_MOVE;

		movement = true;
		p_current_node = -1;
		getNewTarget();
	}

}
//---------------------------

/*
//---------Attack
*/

bool Player::IsInRange(Entity* enemy)
{
	fPoint target_enemy = enemy->getPivotPosition();

	fPoint dist;

	dist.x = target_enemy.x - p_position.x;
	dist.y = target_enemy.y - p_position.y;

	float ret = dist.getModule();
	ret = ret;

	if (attack_range > ret)
	{
		return true;
	}

	return false;
}

void Player::updateAttack()
{
	//NOTE: provisional attack state
		if (current_animation->Finished())
		{
			current_input = INPUT_STOP_MOVE;
			attacking = false;
			input_locked = false;
		}
	
}

void Player::CheckToAttack()
{
	if (enemy && !attacking)
	{
		if (IsInRange(enemy))
		{

			fPoint target = enemy->getPivotPosition();

			fPoint dist = { target - p_position };
			p_velocity = dist;

			SetDirection();

			enemy->TakeDamage(atk_damage);
			app->audio->PlayFx(player_attack, 0);

			movement = false;
			current_input = INPUT_SKILL;
			enemy = NULL;
			objective = NULL;
			attacking = true;
			input_locked = true;
		}
	}

	else if (objective && !enemy)
	{
		if (IsInRange(objective))
		{
			fPoint target = objective->getPivotPosition();

			fPoint dist = { target - p_position };
			p_velocity = dist;

			SetDirection();

			movement = false;
			current_input = INPUT_STOP_MOVE;
			
			if (objective->type == PORTAL)
				PlayerEvent(TELEPORT);
			objective = NULL;
		}
	}
}


bool Player::Alive()
{
	return current_action != DEATH;
}

/*
//--------Input
*/

void Player::HandleInput()
{
	
	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		current_skill = left_skill;

		current_skill->skillInit();

		if (current_skill->skill_type != SKILL_MELEE)
		{ 
			current_input = INPUT_SKILL;
		}
		else
		{
			iPoint target;
			//NOTE: this will be later changed
			objective = app->game->em->entityOnMouse();

			if (objective && objective->type == ENEMY)
			{
				enemy = (EntEnemy*)app->game->em->entityOnMouse();
			}

			target = app->input->getMouseWorldPosition();
			target = app->map->WorldToMap(target.x, target.y);
			SetMovement(target.x, target.y);
		}
	

	}

	//NOTE: Debug purposes, must be changed! (Particle system)
	//------------
	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		current_skill = right_skill;

		current_skill->skillInit();

		if (current_skill->skill_type != SKILL_MELEE)
		{
			current_input = INPUT_SKILL;
		}
		/*if (!input_locked)
		{
			particle_destination.x = app->input->getMouseWorldPosition().x;
			particle_destination.y = app->input->getMouseWorldPosition().y;
			SetDirection(particle_destination);
			SetInput(INPUT_CAST);
			input_locked = true;
		}*/
		else
		{
			iPoint target;
			//NOTE: this will be later changed
			objective = app->game->em->entityOnMouse();

			if (objective && objective->type == ENEMY)
			{
				enemy = (EntEnemy*)app->game->em->entityOnMouse();
			}

			target = app->input->getMouseWorldPosition();
			target = app->map->WorldToMap(target.x, target.y);
			SetMovement(target.x, target.y);
		}
	}
	//------------
}

void Player::SetInput(INPUT_STATE input)
{
	if (!input_locked)
	{
		current_input = input;
	}
}

//StateMachine Functions
ACTION_STATE Player::updateAction()
{
	if (current_input != INPUT_NULL)
	{
		switch (current_action)
		{
		case IDLE:
		{
			if (current_input == INPUT_MOVE)
			{
				if (!running)
					current_action = WALKING;
				else
					current_action = RUNNING;
			}

			else if (current_input == INPUT_SKILL)
			{
				current_action = SKILL;
			}

			else if (current_input == INPUT_DEATH)
			{
				current_action = DEATH;
			}
			
		}
		break;

		case WALKING:
		{
			if (current_input == INPUT_STOP_MOVE)
			{
				current_action = IDLE;
			}

			else if (current_input == INPUT_SKILL)
			{
				current_action = SKILL;
			}

			else if (current_input == INPUT_MOVE && running)
			{
				current_action = RUNNING;
			}

			else if (current_input == INPUT_DEATH)
			{
				current_action = DEATH;
			}
		}
		break;

		case RUNNING:
		{
			if (current_input == INPUT_STOP_MOVE)
			{
				current_action = IDLE;
			}
			else if (current_input == INPUT_MOVE && !running)
			{
				current_action = WALKING;
			}
			
			else if (current_input == INPUT_SKILL)
			{
				current_action = SKILL;
			}

			else if (current_input == INPUT_DEATH)
			{
				current_action = DEATH;
			}
		}
		break;

		case SKILL:
		{
			if (current_input == INPUT_STOP_MOVE)
			{
				current_action = IDLE;
			}
			else if (current_input == INPUT_DEATH)
			{
				current_action = DEATH;
			}
			
		}
		break;

		case DEATH:
		{
			if (current_input == INPUT_STOP_MOVE)
			{
				current_action = IDLE;
			}
		}
		break;
		}

		if (previous_action != current_action)
			PlayerEvent(STATE_CHANGE);

		previous_action = current_action;
	}

	current_input = INPUT_NULL;
	return current_action;
}

/*
//-------Stats related
*/



bool Player::TeleportReady()
{
	return teleport;
}

Scene* Player::getDestiny()
{
	return scene_to_teleport;
}

void Player::ResetTeleport()
{
	teleport = NULL;
	scene_to_teleport = NULL;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	int i = 0;
}


void Player::SetDirection()
{
	float angle = p_velocity.getAngle();

	DIRECTION dir;

	if (angle < 22.5 && angle > -22.5)
		dir = D_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = D_FRONT_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = D_FRONT;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = D_FRONT_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = D_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = D_BACK_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = D_BACK;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = D_BACK_RIGHT;

	if (dir != current_direction)
	{
		current_direction = dir;
	}

}

void Player::SetDirection(fPoint pos)
{
	//NOTE: This has been created to change the direction without moving the player
	fPoint direction;
	direction.x = pos.x - p_position.x;
	direction.y = pos.y - p_position.y;

	direction.SetModule(1);

	float angle = direction.getAngle();

	DIRECTION dir;

	if (angle < 22.5 && angle > -22.5)
		dir = D_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = D_FRONT_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = D_FRONT;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = D_FRONT_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = D_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = D_BACK_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = D_BACK;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = D_BACK_RIGHT;

	if (dir != current_direction)
	{
		current_direction = dir;
	}

}

void Player::SetPosition(fPoint pos)
{
	p_position = pos;
	p_collider->rect.x = getPivotPosition().x - 20;
	p_collider->rect.y = getBlitPosition().y + 20;
}

void Player::StateMachine()
{
	switch (currentPhase)
	{
	case BARBARIAN:
		break;
	case BUTCHER:
		break;
	case DIABLO:
		break;
	default:
		break;
	}

	switch (current_action)
	{
	case IDLE:
		//p_sprite = p_idle;
		//current_animation_set = idle;
		break;
	case WALKING:
		//p_sprite = p_walk;
		//current_animation_set = walk;
		p_speed = PLAYER_SPEED;
		break;
	case RUNNING:
		//p_sprite = p_run;
		//current_animation_set = run;
		p_speed = PLAYER_RUN_SPEED;
		break;
	case SKILL:
		//p_sprite = current_skill->skill_tex;
		//current_animation_set = current_skill->skill_animation_set;
		break;
	case DEATH:
		//p_sprite = p_death;
		//current_animation_set = death;
		respawn_timer.start();
		break;
	}
	//current_animation = &current_animation_set[current_direction];
}



// ------------------------------------
// ------------------------------------

bool Player::loadAnimations()
{
	bool ret = true;

	pugi::xml_document	anim_file;
	pugi::xml_node		anim;
	char* buff;
	int size = app->fs->Load("animations/player_animations.xml", &buff);
	pugi::xml_parse_result result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations");

	if (ret == true)
	{
		for (pugi::xml_node ent = anim.child("BARBARIAN"); ent != NULL; ent = ent.next_sibling())
		{
			for (pugi::xml_node action = ent.child("IDLE"); action != NULL; action = action.next_sibling())
			{
				for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
				{
					std::pair<ACTION_STATE, DIRECTION> p;
					int state = action.child("name").attribute("value").as_int();
					p.first = (ACTION_STATE)state;

					int di = dir.first_child().attribute("name").as_int();
					p.second = (DIRECTION)di;

					Animation anims;
					int x = dir.first_child().attribute("x").as_int();
					int y = dir.first_child().attribute("y").as_int();
					int w = dir.first_child().attribute("w").as_int();
					int h = dir.first_child().attribute("h").as_int();
					int fN = dir.first_child().attribute("frameNumber").as_int();
					int margin = dir.first_child().attribute("margin").as_int();
					bool loop = action.child("loop").attribute("value").as_bool();
					int pivotX = dir.first_child().attribute("pivot_x").as_int();
					int pivotY = dir.first_child().attribute("pivot_y").as_int();
					float animSpeed = action.child("speed").attribute("value").as_float();
					anims.setAnimation(x, y, w, h, fN, margin);
					anims.loop = loop;
					anims.speed = animSpeed;
					anims.pivot.x = pivotX;
					anims.pivot.y = pivotY;

					int entity = ent.child("name").attribute("value").as_int();
					iPoint piv;
					switch (entity)
					{
					case 0:
						barbarianAnim.insert(std::pair<std::pair<ACTION_STATE, DIRECTION>, Animation >(p, anims));
						barbarianAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
						piv = barbarianAnim.find({ p.first, p.second })->second.pivot;
						break;

					case 1:
						butcherAnim.insert(std::pair<std::pair<ACTION_STATE, DIRECTION>, Animation >(p, anims));
						butcherAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
						break;

					case 2:
						diabloAnim.insert(std::pair<std::pair<ACTION_STATE, DIRECTION>, Animation >(p, anims));
						diabloAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
						break;
					}

				}
			}
		}
	}


	return ret;
}