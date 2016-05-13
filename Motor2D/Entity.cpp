#include "Render.h"
#include "App.h"
#include "Entity.h"
#include "Input.h"
#include "Map.h"
#include "Game.h"
#include "EntityManager.h"
#include "Player.h"
#include "Attributes.h"
#include "Pathfinding.h"
#include "Textures.h"
#include "p2Log.h"

Entity::Entity()
{
	player = app->game->player;

	pDebug = app->tex->Load("maps/mini_path.png");

	if (pDebug == NULL)
	{
		LOG("Mini path entity not loaded correctly");
	}
	else
	{
		LOG("Mini path entity loaded correctly");
	}
}

Entity::~Entity()
{
}

bool Entity::start()
{
	return true;
}

bool Entity::entityUpdate(float internDT)
{
	bool ret = true;

	handleInput();

	if (attributes->getLife() <= 0)
	{
		current_input = EI_DIE;
	}

	updateAction();

	//LOG("currentState: %d", currentState);
	if (currentState != E_DEATH)
	{
		switch (currentState)
		{
		case E_IDLE:
			break;
		case E_WALK:
			updateMovement(internDT);
			break;
		case E_SKILL:
			break;
		case E_BASIC_ATTACK:
			if (currentAnimation->isOver() && player != NULL)
			{
				player->attributes->damage(attributes,0);
				current_input = EI_STOP;
				currentAnimation->Reset();
			}
			break;
		}
	}
	else
	{
		collider->SetPos(10000, 0);
	}

	//LOG("Entity life: %f", attributes->getLife());

	return ret;
}

bool Entity::entityPostUpdate()
{
	bool ret = true;
	draw();

	if (app->debug)
	{
		drawDebug();
	}
	return ret;
}

//Generic methods----------------------------

Collider* Entity::getCollider()const
{
	return collider;
}
uint Entity::getId()const
{
	return id;
}

iPoint Entity::getWorldPosition()const
{
	return worldPosition;
}

iPoint Entity::getMapPos()const
{
	return mapPosition;
}

iPoint Entity::getStartingWorldPosition() const//This might be useless
{
	return worldPosition;
}

iPoint Entity::getColliderOffset() const
{
	return colliderOffset;
}

iPoint Entity::getColliderSize() const
{
	return colliderSize;
}

Sprite* Entity::getSprite() const
{
	return imageSprite;
}

//Setters
void Entity::setMapPosition(iPoint tile)
{
	mapPosition = tile;
	worldPosition = app->map->MapToWorld(tile.x, tile.y);
}

void Entity::setWorldPosition(iPoint coords)
{
	worldPosition = coords;
	mapPosition = app->map->WorldToMap(coords.x, coords.y);
}

void Entity::setStartingWorldPosition(iPoint coords)//This might be useless
{
	worldPosition = coords;
}

void Entity::setColliderPosition(iPoint coords)
{
	collider->SetPos(coords.x,coords.y);
}

void Entity::setId(int id)
{
	this->id = id;
}

//Draw------------------------------------

void Entity::draw()
{
	//Not elegant, but works // May be the vibration of player comes from here ERROR
	setDirection();
	if (previousDirection != currentDirection || previousState != currentState)
	{
		currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;
		previousState = currentState;
		previousDirection = currentDirection;
	}
	else if (currentState == E_DEATH)
	{
		currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;
	}

	imageSprite->updateSprite(worldPosition, currentAnimation->pivot, currentAnimation->getCurrentFrame());
}

void Entity::drawDebug()
{
	app->render->DrawCircle(worldPosition.x, worldPosition.y, targetRadius, 255, 0, 0, 255, true);
	app->render->DrawCircle(worldPosition.x, worldPosition.y, visionRadius, 0, 0, 255, 255, true);

	iPoint t_pos = getMapPos();
	app->render->Blit(pDebug, t_pos.x, t_pos.y);
	if (movement)
	{
		app->render->DrawLine(worldPosition.x, worldPosition.y, target.x, target.y, 0, 0, 255);
		app->render->DrawLine(worldPosition.x, worldPosition.y, velocity.x + worldPosition.x, velocity.y + worldPosition.y, 0, 255, 255);
	}
	//Path
	for (int i = 0; i < path.size(); i++)
	{
		iPoint tmp = path[i];
		tmp = app->map->getTileBlit(tmp.x, tmp.y);
		app->render->Blit(pDebug, tmp.x, tmp.y);
	}
}


//Movement methods
vector<iPoint> Entity::getNewPath(iPoint target)
{
	vector<iPoint> ret;

	iPoint start = app->map->WorldToMap(worldPosition.x, worldPosition.y);
	iPoint goal = target;
	vector<iPoint> _path;
	int steps = app->pathfinding->getNewPath(start, goal, path);
	_path = path;
	if (steps > 0)
	{
		//StateMachine change
		current_input = EI_WALK;

		movement = true;
		currentNode = -1;
		getNewTarget();
	}

	return ret;
}

void Entity::getNewTarget()
{
	if ((uint)currentNode + 1 < path.size())
	{
		currentNode++;
		setTarget(app->map->getTileCenter(path[currentNode].x, path[currentNode].y));
	}
	else
	{
		//Maybe ERROR, watch out //setInput(INPUT_STOP_MOVE);
		movement = false;
	}
}

void Entity::setTarget(iPoint _target)
{
	target = _target;
	movement = true;
	targetReached = false;
}

void Entity::updateVelocity(float dt)
{
	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());

	//Maybe ERROR, watch out //SetDirection();
}

void Entity::updateMovement(float dt)
{
	if (movement)
	{
		if (!targetReached)
		{
			updateVelocity(dt);
			move(dt);
			targetReached = isTargetReached();
		}
		else
		{
			getNewTarget();
		}
	}
	if (isInDestiny() || (player != NULL && player->getMapPosition().DistanceTo(worldPosition) <= targetRadius && currentState == E_WALK))
	{
		current_input = EI_STOP;
	}
}

bool Entity::isTargetReached()
{
	fPoint vel;

	vel.x = target.x - worldPosition.x;
	vel.y = target.y - worldPosition.y;

	if (vel.getModule() <= targetRadius)
	{
		return true;
	}

	return false;
}

void Entity::move(float dt)
{
	fPoint vel = velocity * dt;

	worldPosition.x += int(vel.x);
	worldPosition.y += int(vel.y);

	
	collider->SetPos(worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y); //Maybe ERROR, watch out
	if (isInDestiny() || (player != NULL && player->getWorldPosition().DistanceNoSqrt(worldPosition) <= targetRadius * targetRadius && currentState == E_WALK))
	{
		path.clear();
	}
}

void Entity::setMovement(int x, int y)
{

}

bool Entity::isInDestiny()
{
	if (app->map->WorldToMap(player->worldPosition.x, player->worldPosition.y) == app->map->WorldToMap(worldPosition.x, worldPosition.y))
	{
		return true;
	}

	return false;
}

void Entity::setDirection()
{
	float angle = velocity.getAngle();

	entityDirection dir;

	if (angle < 22.5 && angle > -22.5)
		dir = E_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = E_UP_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = E_UP;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = E_UP_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = E_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = E_DOWN_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = E_DOWN;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = E_DOWN_RIGHT;

	if (dir != currentDirection)
	{
		currentDirection = dir;
	}
}

void Entity::setDirection(fPoint pos)
{
	//NOTE: This has been created to change the direction without moving the player
	fPoint direction;
	direction.x = pos.x - worldPosition.x;
	direction.y = pos.y - worldPosition.y;

	direction.SetModule(1);

	float angle = direction.getAngle();

	entityDirection dir;

	if (angle < 22.5 && angle > -22.5)
		dir = E_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = E_UP_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = E_UP;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = E_UP_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = E_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = E_DOWN_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = E_DOWN;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = E_DOWN_RIGHT;

	if (dir != currentDirection)
	{
		currentDirection = dir;
	}
}

entityState Entity::updateAction()
{
	if (current_input != EI_NULL)
	{
		switch (currentState)
		{
		case E_IDLE:
		{
			if (current_input == EI_WALK)
			{
				currentState = E_WALK;
			}
			else if (current_input == EI_DIE)
			{
				currentState = E_DEATH;
			}
		}
		break;

		case E_WALK:
		{
			if (current_input == EI_STOP)
			{
				currentState = E_IDLE;
			}
			else if (current_input == EI_DIE)
			{
				currentState = E_DEATH;
			}
			else if (current_input == EI_ATTACK)
			{
				currentState = E_BASIC_ATTACK;
			}
		}
		break;

		case E_BASIC_ATTACK:
		{
			if (current_input == EI_STOP)
			{
				currentState = E_IDLE;
			}
			else if (current_input == EI_DIE)
			{
				currentState = E_DEATH;
			}
		}

		break;

		case E_DEATH:
		{
			if (current_input == EI_STOP)
			{
				currentState = E_IDLE;
			}
		}

		break;

		case RUNNING:
		{
			if (current_input == EI_STOP)
			{
				currentState = E_IDLE;
			}
			else if (current_input == EI_WALK)
			{
				currentState = E_WALK;
			}
			else if (current_input == EI_DIE)
			{
				currentState = E_DEATH;
			}
			else if (current_input == EI_ATTACK)
			{
				currentState = E_BASIC_ATTACK;
			}
		}
		break;
		}

		if (previousState != currentState)
		{

		}

		previousState = currentState;

	}
	current_input = EI_NULL;

	return currentState;
}

void Entity::handleInput()
{
	if (!inputBlocked)
	{
		if (attributes->getLife() <= 0)
		{
			current_input = EI_DIE;
		}
		if (currentState != E_DEATH)
		{			
			//Do things
			if (player)
			{
				if (worldPosition.DistanceNoSqrt(player->getWorldPosition()) <= visionRadius * visionRadius)//Range vision
				{
					//comprobar si estas a rang d'atac
					if (worldPosition.DistanceNoSqrt(player->getWorldPosition()) < targetRadius * targetRadius)
					{
						current_input = EI_ATTACK;
					}
					else
					{
						target = app->game->player->getWorldPosition();
						target = app->map->WorldToMap(target.x, target.y);
						getNewPath(target);
						current_input = EI_WALK;
					}
				}
			}
		}
	}
}


bool Entity::mouseHover()
{
	bool ret = false;
	iPoint pos = app->input->getMouseWorldPosition();

	if (collider)
	{
		if (pos.x >= collider->rect.x && pos.x <= collider->rect.x + collider->rect.w &&
			pos.y >= collider->rect.y && pos.y <= collider->rect.y + collider->rect.h)
		{
			ret = true;
		}
	}

	return ret;
}