#include "Textures.h"
#include "Render.h"
#include "App.h"
#include "Entity.h"
#include "Input.h"
#include "Map.h"


Entity::Entity()
{
	
}

Entity::~Entity()
{
}

bool Entity::entityUpdate(float internDT)
{
	bool ret = true;

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

}
void Entity::setId(int id)
{
	this->id = id;
}

//Draw------------------------------------

void Entity::draw()
{

}

void Entity::drawDebug()
{

}


//Movement methods
vector<iPoint> Entity::getNewPath(iPoint target)
{
	vector<iPoint> ret;

	return ret;
}

void Entity::getNewTarget()
{

}

void Entity::setTarget(iPoint target)
{

}

void Entity::updateVelocity(float dt)
{

}

void Entity::updateMovement(float dt)
{

}

bool Entity::isTargetReached()
{
	bool ret = true;

	return ret;
}

void Entity::move(float dt)
{

}

void Entity::setMovement(int x, int y)
{

}

bool Entity::isInDestiny()
{
	bool ret = false;

	return ret;
}

void Entity::setDirection()
{

}

void Entity::setDirection(fPoint pos)
{

}

entityState Entity::updateAction()
{
	return currentState;
}

void Entity::handleInput()
{

}