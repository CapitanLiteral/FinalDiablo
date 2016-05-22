#include "Wolf.h"

#include "App.h"
#include "Render.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Collision.h"
#include "Attributes.h"
#include "Map.h"

Wolf::Wolf(iPoint pos) : Entity()
{
	setWorldPosition(pos);

	movement = true;

	type = WOLF;
	entityAnim = app->game->em->getWolfAnimation();

	currentState = E_IDLE;
	currentDirection = E_DOWN;

	currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;

	imageSprite = new Sprite(app->game->em->getWolfTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	colliderOffset.Set(25, 100);
	colliderSize.Set(50, 100);

	collider = app->collision->addCollider({ worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y, colliderSize.x, colliderSize.y }, COLLIDER_ENEMY, app->game->em);

	collider->entityLinked = this;

	alive = true;

	attributes = new Attributes(*app->game->em->getWolfAttributBuilder());
}


Wolf::~Wolf()
{
}


bool Wolf::entityUpdate(float dt)
{
	bool ret = true;

	handleInput();

	updateAction();

	if (attributes->getLife() <= 0)
	{
		current_input = EI_DIE;
		if (!died)
		{
			//player->attributes->addExp(100);
		}
	}

	if (currentState != E_DEATH)
	{
		updateMovement(dt);
	}
	else
	{
		collider->to_delete = true;
	}

	return ret;
}

entityState Wolf::updateAction()
{
	if (current_input != EI_NULL)
	{
		switch (currentState)
		{
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

		if (previousState != currentState)
		{
			setDirection();
		}

		previousState = currentState;

		}
		current_input = EI_NULL;
	}
	return currentState;
}

void Wolf::handleInput()
{
	if (!inputBlocked)
	{
		if (attributes->getLife() <= 0)
		{
			movement = false;
			current_input = EI_DIE;
		}
		if (currentState != E_DEATH)
		{
			if (timerChangeDir.ReadSec() >= changeDir)
			{
				iPoint displace;
				do
				{
					displace.x = (rand() % 160) - 80;
					displace.y = (rand() % 160) - 80;

				} while (!isInWalkableTile(worldPosition + displace));
				target = worldPosition + displace;
				setInitVelocity();
			}
		}
		inputBlocked = false;
	}
}

void Wolf::setInitVelocity()
{
	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());

	float angle = velocity.getAngle();
	float escalar = 0.02;

	if (angle < 22.5 && angle > -22.5)
		target.x = target.x + (angle * escalar); //RIGHT
	else if (angle >= 22.5 && angle <= 67.5)
	{
		target.x = target.x + (angle * escalar); //DOWN_RIGHT
		target.y = target.y + (angle * escalar);
	}
	else if (angle > 67.5 && angle < 112.5)
		target.y = target.y + (angle * escalar); //DOWN
	else if (angle >= 112.5 && angle <= 157.5)
	{
		target.x = target.x + (angle * escalar);//DOWN_LEFT
		target.y = target.y + (angle * escalar);
	}
	else if (angle > 157.5 || angle < -157.5)
		target.x = target.x + (angle * escalar);//LEFT
	else if (angle >= -157.5 && angle <= -112.5)
	{
		target.x = target.x + (angle * escalar);//UP_LEFT
		target.y = target.y + (angle * escalar);
	}
	else if (angle > -112.5 && angle < -67.5)
		target.y = target.y + (angle * escalar);//UP
	else if (angle >= -67.5 && angle <= -22.5)
	{
		target.x = target.x + (angle * escalar);//UP_RIGHT
		target.y = target.y + (angle * escalar);
	}

	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());

	movement = true;
}

void Wolf::move(float dt)
{
	fPoint vel = velocity * dt;

	worldPosition.x += vel.x;
	worldPosition.y += vel.y;

	moveCollider();

		

	if (app->debug)
	{
		app->render->DrawLine(worldPosition.x, worldPosition.y, target.x, target.y, 0, 0, 255);

		app->render->DrawLine(worldPosition.x, worldPosition.y, vel.x * 50 + target.x, vel.y * 50 + target.y, 0, 255, 255);
	}
}

void Wolf::updateVelocity(float dt)
{
	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());

}

void Wolf::updateMovement(float dt)
{
	updateVelocity(dt);
	setDirection();
	move(dt);
}

bool Wolf::isInWalkableTile(iPoint coords)
{
	bool ret = true;

	iPoint tile = app->map->WorldToMap(coords.x, coords.y);

	MapLayer* walkability = NULL;
	std::list<MapLayer*>::iterator it = app->map->data.layers.begin();
	while (it != app->map->data.layers.end())
	{
		if ((*it)->name == "Navigation")
		{
			walkability = *it;
			break;
		}
		it++;
	}
	if (!((walkability != NULL)))
	{
		LOG("Couldn't find 'Walkable' layer");
	}

	if (walkability->get(tile.x, tile.y) == 2)
		ret = false;

	return ret;
}