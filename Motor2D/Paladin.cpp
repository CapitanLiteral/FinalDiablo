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


Paladin::Paladin() : Entity()
{
	type = PALADIN;
	entityAnim = app->game->em->getPaladinAnimation();

	currentState = E_IDLE;
	direction = E_DOWN;

	currentAnimation = &entityAnim->find({ currentState, direction })->second;

	imageSprite = new Sprite(app->game->em->getPaladinTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	colliderOffset.Set(25, 100);
	colliderSize.Set(50, 100);

	collider = app->collision->addCollider({worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y, colliderSize.x, colliderSize.y}, COLLIDER_ENEMY, app->game->em);

	alive = true;

	attributes = new Attributes(*app->game->em->getPaladinAttributBuilder());
}

Paladin::~Paladin()
{

}
