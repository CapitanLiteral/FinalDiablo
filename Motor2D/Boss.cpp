#include "Boss.h"
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
#include "Audio.h"
#include "p2Log.h"
#include "Gui.h"

Boss::Boss(iPoint pos)
{
	setWorldPosition(pos);

	type = BOSS;
	entityAnim = app->game->em->getBossAnimation();

	visionRadius = 200.0f;

	currentState = E_IDLE;
	currentDirection = E_DOWN;

	currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;

	imageSprite = new Sprite(app->game->em->getBossTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	colliderOffset.Set(25, 100);
	colliderSize.Set(50, 100);

	collider = app->collision->addCollider({ worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y, colliderSize.x, colliderSize.y }, COLLIDER_ENEMY, app->game->em);

	collider->entityLinked = this;

	alive = true;

	attributes = new Attributes(*app->game->em->getGriswoldAttributBuilder());
}


Boss::~Boss()
{
}

bool Boss::entityUpdate(float dt)
{
	bool ret = true;

	handleInput();

	updateAction();

	checkMouseForBar();

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
		switch (currentState)
		{
		case E_IDLE:
			break;
		case E_WALK:
			updateMovement(dt);
			break;
		case E_BASIC_ATTACK:
			int i = (rand() % 2);
			switch (i)
			{
			case 0:
				if (player)
					app->particleManager->createCross2Emisor(worldPosition.x, worldPosition.y, player);
				break;
			case 1:
				if (player)
					app->particleManager->createRadialEmisor(worldPosition.x, worldPosition.y, player);
				break;
			}
			timerSpell.start();
			break;
		}
	}
	else
	{
		collider->to_delete = true;
	}

	return ret;
}

void Boss::handleInput()
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
				if (worldPosition.DistanceNoSqrt(player->getWorldPosition()) <= visionRadius * visionRadius)//Range vision just follow you
				{
					int i = rand() % 2;
					switch (i)
					{
					case 0:
						if (timerChase.ReadSec() >= timeMarginChase)
						{
							setInitVelocity();
							setDirection();
							current_input = EI_WALK;
						}
						break;
					case 1:
						if (timerSpell.ReadSec() >= timeSpell)
						{
							current_input = EI_ATTACK;
						}
						break;
					}
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

entityState Boss::updateAction()
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
			else if (current_input == EI_ATTACK)
			{
				currentState = E_BASIC_ATTACK;
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
			else if (current_input == EI_WALK)
			{
				currentState = E_WALK;
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

void Boss::setInitVelocity()
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

void Boss::move(float dt)
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

bool Boss::isTargetReached()
{
	fPoint vel;

	vel.x = target.x - worldPosition.x;
	vel.y = target.y - worldPosition.y;

	if (vel.getModule() <= targetRadius)
	{
		timerChase.start();
		return true;
	}
	return false;
}

void Boss::updateVelocity(float dt)
{
	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());
}

void Boss::updateMovement(float dt)
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