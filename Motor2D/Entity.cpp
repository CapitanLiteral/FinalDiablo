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

Entity::Entity()
{
	player = app->game->player;

	fxPlayerGetHit = app->game->em->fxPlayerGetHit;

	pDebug = app->tex->Load("maps/mini_path.png");

	if (pDebug == NULL)
	{
		LOG("Mini path entity not loaded correctly");
	}
	else
	{
		LOG("Mini path entity loaded correctly");
	}
	lifeBarRect = { 320, 725, 102, 18 };
	lifeBar = app->gui->addGuiImage({ 300, 0 }, { 320, 725, 102, 18 }, NULL, NULL);/**/
	lifeBar->Desactivate();
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
	return true;
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
	else if (currentState == E_DEATH && !died)
	{
		currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;
		died = true;
	}
	else if (currentState == E_BASIC_ATTACK && !died)
	{
		currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;
	}
	else if (currentState == E_IDLE && !died)
	{
		currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;
	}
	else if (currentState == E_IDLE && !died)
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
}


//Movement methods
void Entity::setDirection()
{
	float angle = velocity.getAngle();

	entityDirection dir;

	if (angle < 22.5 && angle > -22.5)
		dir = E_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = E_DOWN_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = E_DOWN;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = E_DOWN_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = E_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = E_UP_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = E_UP;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = E_UP_RIGHT;

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
	return currentState;
}

void Entity::handleInput()
{
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

void Entity::moveCollider()
{
	if (collider)
		collider->SetPos(worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y);
}

void Entity::checkMouseForBar()
{
	if (mouseHover() && getCollider()->type == COLLIDER_ENEMY)
	{
		lifeBar->Activate();
	}
	if (lifeBar->active == true)
	{
		SDL_Rect rect;
		float dif;
		float entityLife = attributes->getLife();
		if (entityLife > 0.0f)
		{
			rect = lifeBarRect;
			dif = attributes->getMaxLife() - entityLife;
			//dif *= rect.w;
			//	dif /= entityLife;
			rect.w -= dif;

			//rect.w -= int(dif);

			lifeBar->SetTextureRect(rect);
		}
		else
		{
			lifeBar->SetTextureRect({ 0, 0, 0, 0 });
		}
	}

	if (!mouseHover()){
		lifeBar->Desactivate();
	}
}