#include "EntMobile.h"
#include "Map.h"
#include "App.h"
#include "Player.h"
#include "Game.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Collision.h"

//Constructor
EntMobile::EntMobile(const iPoint &p, uint ID) : Entity(p, ID)
{
	last_action = current_action = ENTITY_IDLE;
	current_input = ENTITY_INPUT_NULL;
	last_direction = current_direction = ENTITY_D_FRONT;
}

//Destructor
EntMobile::~EntMobile()
{
}

void EntMobile::SetDirection()
{
	float angle = velocity.getAngle();

	ENTITY_DIRECTION dir;

	if (angle < 22.5 && angle > -22.5)
		dir = ENTITY_D_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = ENTITY_D_FRONT_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = ENTITY_D_FRONT;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = ENTITY_D_FRONT_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = ENTITY_D_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = ENTITY_D_BACK_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = ENTITY_D_BACK;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = ENTITY_D_BACK_RIGHT;

	if (dir != current_direction)
	{
		current_direction = dir;
	}
}

//Movement
//---------------------------
void EntMobile::SetInitVelocity()
{
	//NOTE: This only works when the target is the player, may be changed in other cases
	//		Maybe a SetInitVelocity(target) would solve this possible issue
	target = app->game->player->getMapPosition();

	velocity.x = target.x - position.x;
	velocity.y = target.y - position.y;

	velocity.SetModule(speed);

}

void EntMobile::Move(float dt)
{
	fPoint vel = velocity * dt;

	position.x += int(vel.x);
	position.y += int(vel.y);

	collider->rect.x += int(vel.x);
	collider->rect.y += int(vel.y);
}

void EntMobile::updateVelocity(float dt)
{
	velocity.x = target.x - position.x;
	velocity.y = target.y - position.y;

	velocity.SetModule(speed);

	SetDirection();
}

bool EntMobile::IsTargetReached()
{
	fPoint vel;

	vel.x = target.x - position.x;
	vel.y = target.y - position.y;

	//NOTE: This may be modified to insert attack

	if (vel.getModule() <= target_radius)
	{
		if (!path_on)
		{
			current_input = ENTITY_INPUT_STOP_MOVE;
			movement = false;
		}

		return true;
	}

	return false;
}

void EntMobile::SetTarget(iPoint _target)
{
	target = _target;
	movement = true;
	target_reached = false;
}

void EntMobile::getNewTarget()
{
	//NOTE: This may be modified to insert attack

	if ((uint)current_node + 1< path.size())
	{
		current_node++;
		SetTarget(app->map->getTileCenter(path[current_node].x, path[current_node].y));
	}
	else
	{
		current_input = ENTITY_INPUT_STOP_MOVE;
		movement = false;
	}

}

iPoint EntMobile::FindClosestWalkable(iPoint pos)
{
	bool found = false;
	int lenght = 0;

	iPoint tile = { pos.x, pos.y };

	while (!found && lenght < 10)
	{
		while (tile.y < pos.y + lenght && !found)
		{
			tile.y++;
			if (app->pathfinding->IsWalkable(tile))
				found = true;
		}

		while (tile.x > pos.x - lenght && !found)
		{
			tile.x--;
			if (app->pathfinding->IsWalkable(tile))
				found = true;
		}

		while (tile.y > pos.y - lenght && !found)
		{
			tile.y--;
			if (app->pathfinding->IsWalkable(tile))
				found = true;

		}

		while (tile.x < pos.x + lenght && !found)
		{
			tile.x++;
			if (app->pathfinding->IsWalkable(tile))
				found = true;
		}
		lenght += 1;
	}
	return tile;
}

void EntMobile::updateMovement(float dt)
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

void EntMobile::SetMovement(int x, int y)
{
	if (path_on)
	{
		SetNewPath(x, y);
	}
	else
	{
		iPoint target = app->map->getTileCenter(x, y);
		SetTarget(target);

		//StateMachine change
		current_input = ENTITY_INPUT_MOVE;
	}
}

void EntMobile::SetNewPath(int x, int y)
{
	iPoint start = app->map->WorldToMap(position.x, position.y);
	iPoint goal = { x, y };

	if (app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != NULL &&
		app->game->em->entityOnCoords(app->map->MapToWorld(getMapPosition().x, getMapPosition().y)) != this)
	{
		goal = FindClosestWalkable(goal);
	}

	int steps = app->pathfinding->getNewPath(start, goal, path);

	if (steps > 0)
	{
		//StateMachine change
		current_input = ENTITY_INPUT_MOVE;

		movement = true;
		current_node = -1;
		getNewTarget();
	}

}
//---------------------------