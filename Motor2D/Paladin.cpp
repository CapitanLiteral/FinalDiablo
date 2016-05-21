#include "Paladin.h"

#include "App.h"
#include "Render.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Collision.h"
#include "Attributes.h"
#include "Gui.h"


Paladin::Paladin(iPoint pos) : Entity()
{
	setWorldPosition(pos);

	type = PALADIN;

	visionRadius = 120.0f;
	targetRadius = 30.0f;

	entityAnim = app->game->em->getPaladinAnimation();

	currentState = E_IDLE;
	currentDirection = E_DOWN;

	currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;

	imageSprite = new Sprite(app->game->em->getPaladinTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	colliderOffset.Set(25, 70);
	colliderSize.Set(50, 75);

	collider = app->collision->addCollider({worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y, colliderSize.x, colliderSize.y}, COLLIDER_ENEMY, app->game->em);

	collider->entityLinked = this;

	alive = true;

	attributes = new Attributes(*app->game->em->getPaladinAttributBuilder());
}

Paladin::~Paladin()
{

}

bool Paladin::entityUpdate(float dt)
{
	bool ret = true;

	//if (app->input->getKey(SDL_SCANCODE_0) == KEY_DOWN)
		//app->audio->PlayFx(fxPlayerGetHit);

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

	//LOG("currentState: %d", currentState);
	if (currentState != E_DEATH)
	{
		switch (currentState)
		{
		case E_IDLE:
			break;
		case E_WALK:
			updateMovement(dt);
			break;
		case E_SKILL:
			break;
		case E_BASIC_ATTACK:
			if (currentAnimation->isOver() && player != NULL)
			{
				player->attributes->damage(attributes, 0);
				//app->audio->PlayFx(fxPlayerGetHit);
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

void Paladin::handleInput()
{
	if (!inputBlocked)
	{
		if (attributes->getLife() <= 0)
		{
			current_input = EI_DIE;
		}
		if (currentState != E_DEATH)
		{
			if (player)
			{
				if (worldPosition.DistanceNoSqrt(player->getWorldPosition()) <= visionRadius * visionRadius && timer.ReadSec() >= timeMargin)//Range vision just follow you
				{
					setInitVelocity();
					setDirection();
					current_input = EI_WALK;
				}
				else
				{
					current_input = EI_STOP;
				}
			}
		}
	}
	inputBlocked = false;
}

entityState Paladin::updateAction()
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

void Paladin::setInitVelocity()
{
	target = player->getWorldPosition();

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

void Paladin::move(float dt)
{
	fPoint vel = velocity * dt;

	worldPosition.x += vel.x;
	worldPosition.y += vel.y;

	moveCollider();

	movement = !isTargetReached();

	if (movement)
	{
		updateVelocity(dt);

		if (app->debug)
		{
			app->render->DrawLine(worldPosition.x, worldPosition.y, target.x, target.y, 0, 0, 255);

			app->render->DrawLine(worldPosition.x, worldPosition.y, vel.x * 50 + target.x, vel.y * 50 + target.y, 0, 255, 255);
		}
	}
	else
	{
		current_input = EI_STOP;
	}
}

bool Paladin::isTargetReached()
{
	fPoint vel;

	vel.x = target.x - worldPosition.x;
	vel.y = target.y - worldPosition.y;

	if (vel.getModule() <= targetRadius)
	{
		timer.start();
		return true;
	}
	return false;
}

void Paladin::updateVelocity(float dt)
{
	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());
}

void Paladin::updateMovement(float dt)
{
	if (movement)
	{
		if (!isTargetReached())
		{
			updateVelocity(dt);
			move(dt);
		}
	}
}