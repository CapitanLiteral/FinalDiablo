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

/*
bool Boss::entityUpdate(float dt)
{
	bool ret = true;

	if (app->input->getKey(SDL_SCANCODE_0) == KEY_DOWN)
		app->audio->PlayFx(fxPlayerGetHit);

	handleInput();

	if (attributes->getLife() <= 0)
	{
		current_input = EI_DIE;
		if (!died)
		{
			//player->attributes->addExp(100);
		}
	}

	if (mouseHover() && getCollider()->type == COLLIDER_ENEMY){
		lifeBar->Activate();
	}
	if (lifeBar->active == true){
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
				player->attributes->damage(attributes, 0);
				app->audio->PlayFx(fxPlayerGetHit);
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
*/