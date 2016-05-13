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
	entityAnim = app->game->em->getPaladinAnimation();

	currentState = E_IDLE;
	currentDirection = E_DOWN;

	currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;

	imageSprite = new Sprite(app->game->em->getPaladinTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	colliderOffset.Set(25, 70);
	colliderSize.Set(50, 75);

	collider = app->collision->addCollider({worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y, colliderSize.x, colliderSize.y}, COLLIDER_ENEMY, app->game->em);

	alive = true;

	attributes = new Attributes(*app->game->em->getPaladinAttributBuilder());
}

Paladin::~Paladin()
{

}
/*
bool Paladin::entityUpdate(float dt)
{
	if (mouseHover()){
		lifeBar->Activate();
	}
	if (lifeBar->active == true){
		SDL_Rect rect;
		float dif;
		float maxEntityLife = attributes->getMaxLife();
		if (attributes->getMaxLife() >= maxEntityLife)
		{
			lifeBar->SetTextureRect(lifeBarRect);
		}
		else if (attributes->getMaxLife() > 0.0f)
		{
			rect = lifeBarRect;

			dif = maxEntityLife - attributes->getMaxLife();
			dif *= rect.w;
			dif /= maxEntityLife;

			rect.w -= int(dif);

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
	return true;
}*/