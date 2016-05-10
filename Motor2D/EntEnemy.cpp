#include "EntEnemy.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "Textures.h"
#include "Player.h"
#include "Game.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "p2Point.h"
#include "snWin.h"
#include "Act1.h"
#include "Collision.h"
#include "EntMobile.h"
//Provisional?
#include "Audio.h"


//EntEnemy
//----------------------------

//Constructor
EntEnemy::EntEnemy(const iPoint &p, uint ID) : EntMobile(p, ID)
{
	type = ENEMY;
}

//Drawing methods
void EntEnemy::draw()
{
	//iPoint pos = getBlitPosition();

	if (sprite)
	{
		SDL_Rect current_sprite = current_animation.getCurrentFrame();
		fPoint p = getPivotPosition();
		iPoint pos(p.x, p.y);
		iPoint pivot(current_animation.pivot.x, current_animation.pivot.y);
		sprite->updateSprite(tex, pos, pivot, current_sprite);
	}

}

void EntEnemy::drawDebug()
{
	iPoint t_pos = getTilePosition();
	fPoint p_pos = getPivotPosition();
	SDL_Rect rect = getPlayerRect();

	app->render->DrawQuad({ p_pos.x, p_pos.y, 3, 3 }, 255, 0, 0, 255, false);

	app->render->DrawQuad(rect, 255, 0, 0, 255, false);

	app->render->DrawCircle(p_pos.x, p_pos.y, attack_range, 255, 0, 0);
	app->render->DrawCircle(p_pos.x, p_pos.y, vision, 0, 0, 255);
}


bool EntEnemy::PlayerInRange()
{
	fPoint target_enemy = app->game->player->getPivotPosition();

	fPoint dist;

	dist.x = target_enemy.x - position.x;
	dist.y = target_enemy.y - position.y;

	float ret = dist.getModule();
	ret = ret;

	if (vision > ret)
	{
		return true;
	}

	return false;
}
//----------------------------


void EntEnemy::TakeDamage(int damage)
{
	//NOTE: this will be changed when the defense is applied
	HP_current -= damage;
	
	//Sounds-------
	if (name == "crawler")
		app->audio->PlayFx(app->game->em->crawler_gethitfx);
	if (name == "wolf")
		app->audio->PlayFx(app->game->em->wolf_gethitfx);
	//-------------
	
	if (HP_current <= 0)
	{
		HP_current = 0;
		current_input = ENTITY_INPUT_DEATH;
		if (name == "crawler")
			app->audio->PlayFx(app->game->em->crawler_deathfx);
		if (name == "wolf")
			app->audio->PlayFx(app->game->em->wolf_deathfx);
	}
}

void EntEnemy::DrawHPbar()
{
	//NOTE: make this to GUI so it can put the enemy's name
	float total_width = 200;
	int height = 20;
	int current_width = (total_width/HP_max)* HP_current;

	app->render->DrawQuad({ 220, 0, total_width, height }, 0, 0, 0, 255, true, false);
	app->render->DrawQuad({ 220, 0, current_width, height }, 255, 0, 0, 255, true, false);

}

/*
//---- Attack
*/

bool EntEnemy::PlayerInAttackRange()
{
	fPoint target_enemy = app->game->player->getPivotPosition();

	fPoint dist;

	dist.x = target_enemy.x - position.x;
	dist.y = target_enemy.y - position.y;

	float ret = dist.getModule();

	if (attack_range > ret)
	{
		return true;
	}

	return false;
}

void EntEnemy::updateAttack()
{
	if (current_animation.Finished())
	{
		if (!enemy->Alive() || !PlayerInAttackRange())
		{
			current_input = ENTITY_INPUT_STOP_MOVE;
		}
			
		attacking = false;
		current_animation.Reset();
		//input_locked = false;
	}
}
void EntEnemy::CheckToAttack()
{
	if (enemy && !attacking)
	{
		if (PlayerInAttackRange())
		{

			fPoint target = enemy->getPivotPosition();

			fPoint dist = { target - position };
			velocity = dist;

			SetDirection();

			enemy->TakeDamage(damage);

			movement = false;
			current_input = ENTITY_INPUT_ATTACK;
			attacking = true;
			if (name == "paladin")
				//app->audio->PlayFx(app->game->em->crawler_attackfx);
			if (name == "wolf")
				app->audio->PlayFx(app->game->em->wolf_attackfx);
		}
	}
}

//############################
//###     EnemyPaladin     ###
//############################

//Constructor
EnemyPaladin::EnemyPaladin(const iPoint &p, uint ID) : EntEnemy(p, ID)
{
	name = "paladin";

	tex = app->game->em->paladinTex;

	paladinAnim = &app->game->em->paladinAnim;

	current_animation = paladinAnim->find({ current_action, current_direction })->second;

	enemy_type = ENEMY_PALADIN;

	HP_max = HP_current = 100;
	speed = 100.0f;

	movement = false;

	attack_range = 50.0f;
	vision = 190.0f;

	damage = 5;

	last_update = PATHFINDING_FRAMES;

	collider_rect.w = 50;
	collider_rect.h = 80;

	colliderOffset.x = collider_rect.w / 1.5;
	colliderOffset.y = collider_rect.h / 1.5;

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_ENEMY, app->game->em);

	//Sprite creation

	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	SDL_Rect current_sprite = current_animation.getCurrentFrame();
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}

//update
bool EnemyPaladin::update(float dt)
{
	if (!dead)
	{
		updateAction();

		fPoint player_pos = app->game->player->getPivotPosition();

		//NOTE: The enemy is for following the player one it has been founded, but for now, better not, because of the low framerate
		if ((PlayerInRange()) && !attacking && last_update >= PATHFINDING_FRAMES)
		{
			last_update = 0;
			int target_x = player_pos.x;
			int target_y = player_pos.y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);


			enemy = app->game->player;
		}

		if (app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != NULL &&
			app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != this &&
			last_update >= PATHFINDING_FRAMES &&
			movement == false)
		{
			int target_x = FindClosestWalkable(getMapPosition()).x;
			int target_y = FindClosestWalkable(getMapPosition()).y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);

		}

		CheckToAttack();

		switch (current_action)
		{
		case ENTITY_ATTACKING:
			updateAttack();
		case ENTITY_WALKING:
			updateMovement(dt);
		}

		last_update++;
	}

	if (current_action != last_action || current_direction != last_direction)
	{
		last_action = current_action;
		last_direction = current_direction;
		current_animation = paladinAnim->find({ current_action, current_direction })->second;
	}
	
	return true;
}

ENTITY_STATE EnemyPaladin::updateAction()
{
	if (current_input != ENTITY_INPUT_NULL && current_input != previous_input)
	{
		switch (current_action)
		{
		case ENTITY_IDLE:
		{
			if (current_input == ENTITY_INPUT_MOVE)
			{
				current_action = ENTITY_WALKING;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_WALKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_ATTACKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
		}
		break;
		}

		previous_input = current_input;
		EntityEvent(ENTITY_STATE_CHANGE);
	}

	current_input = ENTITY_INPUT_NULL;

	return current_action;
}

void EnemyPaladin::EntityEvent(ENTITY_EVENT even)
{
	switch (even)
	{
	case ENTITY_STATE_CHANGE:
	{
		StateMachine();
	}
	}
}

void EnemyPaladin::StateMachine()
{
	iPoint pos(position.x - 30, position.y + 30);
	int r;

	switch (current_action)
	{
	case ENTITY_IDLE:

		break;

	case ENTITY_WALKING:

		break;

	case ENTITY_DEATH:

		//Item out
		r = rand() % 4;
		if (r == 0)
			app->game->em->add(pos, ITEM_HEALTH);

		dead = true;
		break;


	case ENTITY_ATTACKING:

		break;
	}
}

//############################
//###      EnemyWolf       ###
//############################

//Constructor
EnemyWolf::EnemyWolf(const iPoint &p, uint ID) : EntEnemy(p, ID)
{
	name = "wolf";

	tex = app->game->em->wolfTex;

	wolfAnim = &app->game->em->wolfAnim;

	current_animation = wolfAnim->find({ current_action, current_direction })->second;

	enemy_type = ENEMY_WOLF;

	HP_max = HP_current = 100;
	speed = 100.0f;

	movement = false;

	attack_range = 50.0f;
	vision = 190.0f;

	damage = 5;

	last_update = PATHFINDING_FRAMES;

	/*collider_rect.w = 50;
	collider_rect.h = 80;

	colliderOffset.x = collider_rect.w / 1.5;
	colliderOffset.y = collider_rect.h / 1.5;*/

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_ENEMY, app->game->em);

	//Sprite creation

	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	SDL_Rect current_sprite = current_animation.getCurrentFrame();
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}

//update
bool EnemyWolf::update(float dt)
{
	if (!dead)
	{
		updateAction();

		fPoint player_pos = app->game->player->getPivotPosition();

		//NOTE: The enemy is for following the player one it has been founded, but for now, better not, because of the low framerate
		if ((PlayerInRange()) && !attacking && last_update >= PATHFINDING_FRAMES)
		{
			last_update = 0;
			int target_x = player_pos.x;
			int target_y = player_pos.y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);


			enemy = app->game->player;
		}

		if (app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != NULL &&
			app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != this &&
			last_update >= PATHFINDING_FRAMES &&
			movement == false)
		{
			int target_x = FindClosestWalkable(getMapPosition()).x;
			int target_y = FindClosestWalkable(getMapPosition()).y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);

		}

		CheckToAttack();

		switch (current_action)
		{
		case ENTITY_ATTACKING:
			updateAttack();
		case ENTITY_WALKING:
			updateMovement(dt);
		}

		last_update++;
	}

	if (current_action != last_action || current_direction != last_direction)
	{
		last_action = current_action;
		last_direction = current_direction;
		current_animation = wolfAnim->find({ current_action, current_direction })->second;
	}

	return true;
}

ENTITY_STATE EnemyWolf::updateAction()
{
	if (current_input != ENTITY_INPUT_NULL && current_input != previous_input)
	{
		switch (current_action)
		{
		case ENTITY_IDLE:
		{
			if (current_input == ENTITY_INPUT_MOVE)
			{
				current_action = ENTITY_WALKING;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_WALKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_ATTACKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
		}
		break;
		}

		previous_input = current_input;
		EntityEvent(ENTITY_STATE_CHANGE);
	}

	current_input = ENTITY_INPUT_NULL;

	return current_action;
}

void EnemyWolf::EntityEvent(ENTITY_EVENT even)
{
	switch (even)
	{
	case ENTITY_STATE_CHANGE:
	{
		StateMachine();
	}
	}
}

void EnemyWolf::StateMachine()
{
	iPoint pos(position.x - 30, position.y + 30);
	int r;

	switch (current_action)
	{
	case ENTITY_IDLE:

		break;

	case ENTITY_WALKING:

		break;

	case ENTITY_DEATH:

		//Item out
		r = rand() % 4;
		if (r == 0)
			app->game->em->add(pos, ITEM_HEALTH);

		dead = true;
		break;


	case ENTITY_ATTACKING:

		break;
	}
}

//############################
//###    EnemyGriswold     ###
//############################

//Constructor
EnemyGriswold::EnemyGriswold(const iPoint &p, uint ID) : EntEnemy(p, ID)
{
	name = "griswold";

	tex = app->game->em->griswoldTex;

	griswoldAnim = &app->game->em->griswoldAnim;

	current_animation = griswoldAnim->find({ current_action, current_direction })->second;

	enemy_type = ENEMY_GRISWOLD;

	HP_max = HP_current = 100;
	speed = 100.0f;

	movement = false;

	attack_range = 50.0f;
	vision = 190.0f;

	damage = 5;

	last_update = PATHFINDING_FRAMES;

	/*collider_rect.w = 50;
	collider_rect.h = 80;

	colliderOffset.x = collider_rect.w / 1.5;
	colliderOffset.y = collider_rect.h / 1.5;*/

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_ENEMY, app->game->em);

	//Sprite creation

	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	SDL_Rect current_sprite = current_animation.getCurrentFrame();
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}

//update
bool EnemyGriswold::update(float dt)
{
	if (!dead)
	{
		updateAction();

		fPoint player_pos = app->game->player->getPivotPosition();

		//NOTE: The enemy is for following the player one it has been founded, but for now, better not, because of the low framerate
		if ((PlayerInRange()) && !attacking && last_update >= PATHFINDING_FRAMES)
		{
			last_update = 0;
			int target_x = player_pos.x;
			int target_y = player_pos.y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);


			enemy = app->game->player;
		}

		if (app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != NULL &&
			app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != this &&
			last_update >= PATHFINDING_FRAMES &&
			movement == false)
		{
			int target_x = FindClosestWalkable(getMapPosition()).x;
			int target_y = FindClosestWalkable(getMapPosition()).y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);

		}

		CheckToAttack();

		switch (current_action)
		{
		case ENTITY_ATTACKING:
			updateAttack();
		case ENTITY_WALKING:
			updateMovement(dt);
		}

		last_update++;
	}

	if (current_action != last_action || current_direction != last_direction)
	{
		last_action = current_action;
		last_direction = current_direction;
		current_animation = griswoldAnim->find({ current_action, current_direction })->second;
	}

	return true;
}

ENTITY_STATE EnemyGriswold::updateAction()
{
	if (current_input != ENTITY_INPUT_NULL && current_input != previous_input)
	{
		switch (current_action)
		{
		case ENTITY_IDLE:
		{
			if (current_input == ENTITY_INPUT_MOVE)
			{
				current_action = ENTITY_WALKING;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_WALKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_ATTACKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
		}
		break;
		}

		previous_input = current_input;
		EntityEvent(ENTITY_STATE_CHANGE);
	}

	current_input = ENTITY_INPUT_NULL;

	return current_action;
}

void EnemyGriswold::EntityEvent(ENTITY_EVENT even)
{
	switch (even)
	{
	case ENTITY_STATE_CHANGE:
	{
		StateMachine();
	}
	}
}

void EnemyGriswold::StateMachine()
{
	iPoint pos(position.x - 30, position.y + 30);
	int r;

	switch (current_action)
	{
	case ENTITY_IDLE:

		break;

	case ENTITY_WALKING:

		break;

	case ENTITY_DEATH:

		//Item out
		r = rand() % 4;
		if (r == 0)
			app->game->em->add(pos, ITEM_HEALTH);

		dead = true;
		break;


	case ENTITY_ATTACKING:

		break;
	}
}

/*
//############################
//###      EnemyBoss       ###
//############################

//Constructor
EntEnemyCouncil::EntEnemyCouncil(const iPoint &p, uint ID) : EntEnemy(p, ID)
{
	name = "council member";
	tex = idle_tex = app->game->em->boss_idle;
	walk_tex = app->game->em->boss_walk;
	death_tex = app->game->em->boss_death;
	attack_tex = app->game->em->boss_attack;

	SetAnimations();
	current_animation_set = idle;
	current_animation = &current_animation_set[current_direction];

	enemy_type = ENEMY_COUNCIL;

	HP_max = HP_current = 400;
	speed = 100.0f;

	movement = false;

	attack_range = 50.0f;
	vision = 150.0f;

	damage = 7;

	last_update = PATHFINDING_FRAMES;

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_ENEMY, app->game->em);

	//Sprite creation

	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	SDL_Rect current_sprite = current_animation->getCurrentFrame();
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}

//update
bool EntEnemyCouncil::update(float dt)
{
	if (!dead)
	{
		updateAction();

		fPoint player_pos = app->game->player->getPivotPosition();

		//NOTE: The enemy is for following the player one it has been founded, but for now, better not, because of the low framerate
		if ((PlayerInRange()) && !attacking && last_update >= PATHFINDING_FRAMES)
		{
			last_update = 0;
			int target_x = player_pos.x;
			int target_y = player_pos.y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);


			enemy = app->game->player;
		}

		if (app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != NULL &&
			app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != this &&
			last_update >= PATHFINDING_FRAMES &&
			movement == false)
		{
			int target_x = FindClosestWalkable(getMapPosition()).x;
			int target_y = FindClosestWalkable(getMapPosition()).y;

			iPoint _target = { target_x, target_y };
			_target = app->map->WorldToMap(_target.x, _target.y);
			SetMovement(_target.x, _target.y);

		}

		CheckToAttack();

		switch (current_action)
		{
		case ENTITY_ATTACKING:
			updateAttack();
		case ENTITY_WALKING:
			updateMovement(dt);
		}

		last_update++;
	}
	else
	{
		if (win.ReadSec() > 5)
		{
			app->sm->act1->win = true;
		}
	}

	return true;
}

ENTITY_STATE EntEnemyCouncil::updateAction()
{
	if (current_input != ENTITY_INPUT_NULL && current_input != previous_input)
	{
		switch (current_action)
		{
		case ENTITY_IDLE:
		{
			if (current_input == ENTITY_INPUT_MOVE)
			{
				current_action = ENTITY_WALKING;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_WALKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
			if (current_input == ENTITY_INPUT_ATTACK)
			{
				current_action = ENTITY_ATTACKING;
			}
		}
		break;

		case ENTITY_ATTACKING:
		{
			if (current_input == ENTITY_INPUT_STOP_MOVE)
			{
				current_action = ENTITY_IDLE;
			}
			if (current_input == ENTITY_INPUT_DEATH)
			{
				current_action = ENTITY_DEATH;
			}
		}
		break;
		}

		previous_input = current_input;
		EntityEvent(ENTITY_STATE_CHANGE);
	}

	current_input = ENTITY_INPUT_NULL;
	return current_action;
}

void EntEnemyCouncil::EntityEvent(ENTITY_EVENT even)
{
	switch (even)
	{
	case ENTITY_STATE_CHANGE:
	{
		StateMachine();
	}
	}
}

void EntEnemyCouncil::StateMachine()
{
	//NOTE: so maaaany wrong things
	iPoint pos(position.x - 30, position.y + 30);
	switch (current_action)
	{
		//NOTE: so maaaany wrong things
		//PIVOT DOESN'T CHANGE!
		//Related to the collider rect, not the sprite
	case ENTITY_IDLE:
		tex = idle_tex;
		current_animation_set = idle;

		sprite_rect.w = sprite_dim.x = 78;
		sprite_rect.h = sprite_dim.y = 51;
		sprite_pivot = pivot = { collider_rect.w / 2, collider_rect.h - 20 };
		sprite_pivot.y += 5;

		break;

	case ENTITY_WALKING:
		tex = walk_tex;
		current_animation_set = walk;

		sprite_rect.w = sprite_dim.x = 78;
		sprite_rect.h = sprite_dim.y = 54;
		sprite_pivot = pivot = { collider_rect.w / 2, collider_rect.h - 20 };
		sprite_pivot.y += 5;
		sprite_pivot.x += 40;

		break;

	case ENTITY_DEATH:
		tex = death_tex;
		current_animation_set = death;

		sprite_rect.w = sprite_dim.x = 190;
		sprite_rect.h = sprite_dim.y = 129;
		sprite_pivot = pivot = { collider_rect.w / 2, collider_rect.h - 20 };
		sprite_pivot.y += 5;
		sprite_pivot.x += 40;

		win.start();

		dead = true;

		break;


	case ENTITY_ATTACKING:
		tex = attack_tex;
		current_animation_set = attack;

		sprite_rect.w = sprite_dim.x = 170;
		sprite_rect.h = sprite_dim.y = 81;
		sprite_pivot = pivot = { collider_rect.w / 2, collider_rect.h - 20 };
		sprite_pivot.y += 5;
		sprite_pivot.x += 40;

		break;
	}
}

void EntEnemyCouncil::SetAnimations()
{
	//NOTE: this should not go here
	collider_rect.w = sprite_rect.w = sprite_dim.x = 77;
	collider_rect.h = sprite_rect.h = sprite_dim.y = 97;

	sprite_pivot = pivot = { collider_rect.w / 2, collider_rect.h - 20 };
	sprite_pivot.y += 5;



	//NOTE: Do it like a player, no number, variables
	//Idle
	for (int i = 0; i < 8; i++)
	{
		Animation tmp;
		tmp.SetFrames(0, (97 + 0) * i, 77, 97, 7, 0);
		tmp.speed = 0.2f;

		idle.push_back(tmp);
	}


	//Walk
	for (int i = 0; i < 8; i++)
	{
		Animation tmp;
		tmp.SetFrames(0, (117 + 0) * i, 140, 117, 11, 0);
		tmp.speed = 0.2f;

		walk.push_back(tmp);
	}

	//Death
	for (int i = 0; i < 8; i++)
	{
		Animation tmp;
		int width = 190;
		int height = 129;
		int margin = 0;
		tmp.SetFrames(0, (height + margin) * i, width, height, 19, margin);
		tmp.loop = false;
		tmp.speed = 0.2f;

		death.push_back(tmp);
	}

	//Attack
	for (int i = 0; i < 8; i++)
	{
		Animation tmp;
		int width = 150;
		int height = 105;
		int margin = 0;
		tmp.SetFrames(0, (height + margin) * i, width, height, 10, margin);
		tmp.loop = false;
		tmp.speed = 0.2f;

		attack.push_back(tmp);
	}
}*/