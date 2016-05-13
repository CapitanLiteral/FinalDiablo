#include "Boss.h"
#include "App.h"
#include "Render.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Collision.h"
#include "Attributes.h"

Boss::Boss(iPoint pos)
{
	setWorldPosition(pos);

	type = BOSS;
	entityAnim = app->game->em->getBossAnimation();

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
