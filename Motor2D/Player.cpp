#include "Player.h"
#include "Attributes.h"
#include "Collision.h"
#include "App.h"

Player::Player()
{
	AttributeBuilder builder;
	attributes = new Attributes(builder);
	
	worldPosition.x = 0;
	worldPosition.y = 0;
	mapPosition.x = 0;
	mapPosition.y = 0;

	colliderOffset.SetToZero();
	colliderSize.SetToZero();

}

bool Player::awake(pugi::xml_node &config)
{
	return true;
}

bool Player::start()
{
	SDL_Rect rect = { worldPosition.x - colliderOffset.x, 
					  worldPosition.y - colliderOffset.y,	// Position
					  colliderSize.x, colliderSize.y};		// Size
	collider = app->collision->addCollider(rect, COLLIDER_PLAYER, this);

	return true;
}

bool Player::update(float dt)
{
	return true;
}

Player::~Player()
{
	RELEASE(attributes);
}



bool Player::Alive()
{
	return alive;
}

// Getters & Setters
iPoint Player::getMapPosition() const
{
	return worldPosition;
}

iPoint Player::getWorldPosition() const
{
	return mapPosition;
}

Sprite* Player::getSprite() const
{
	return sprite;
}

void Player::setMapPosition(iPoint tile)
{

}

void Player::setWorldPosition(iPoint coords)
{

}

//Nonsense things
fPoint Player::getPivotPosition()
{
	fPoint ret{0, 0};

	return ret;
}
