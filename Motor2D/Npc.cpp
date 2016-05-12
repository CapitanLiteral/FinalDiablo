#include "Npc.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "Textures.h"
#include "Player.h"
#include "Game.h"
#include "Input.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "p2Point.h"
#include "snWin.h"
#include "Act1.h"
#include "Collision.h"
#include "Attributes.h"
//Provisional?
#include "Audio.h"
#include "Animation.h"

//Constructor
NpcCounselor::NpcCounselor(const iPoint &p, uint ID)
{
	type = entityType::NPC_COUNSELOR;
	loadGui();
	setWorldPosition(p);
	playerRange = 70.0f;

	colliderOffset.Set(25, 75);
	colliderSize.Set(45, 80);

	readed = true;
	readyForSecondZone = false;

	SDL_Rect rect = { worldPosition.x - colliderOffset.x,
			worldPosition.y - colliderOffset.y,	// Position
			colliderSize.x, colliderSize.y };		// Size
	
	collider = app->collision->addCollider(rect, COLLIDER_NPC, app->game->em);

	entityAnim = app->game->em->getCounselorAnimation();
	currentAnimation = &entityAnim.find({ E_IDLE, E_DOWN })->second;
	imageSprite = new Sprite(app->game->em->getCounselorTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

}

NpcCounselor::~NpcCounselor(){

}

bool NpcCounselor::entityUpdate(float dt)
{
	if (playerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && mouseHover())
		{
			if (app->game->player->attributes->getLevel() == 1  || readed)
			{
				introductionImage->Activate();
				readed = false;
			}
			else if (app->game->player->attributes->getLevel() < 5)
			{
				secondImage->Activate();
			}
			else if (app->game->player->attributes->getLevel() == 5)
			{
				readyToGoImage->Activate();
				readyForSecondZone = true;
			}
			else if (readyForSecondZone)
			{
				//Pasar a la siguiente escena
			}
			else if (app->game->player->attributes->getLevel() == 5 && readyForSecondZone)
			{
				secondIntroductionImage->Activate();
			}
			else if (app->game->player->attributes->getLevel() < 10)
			{
				secondSecondaryImage->Activate();
			}
			else if (app->game->player->attributes->getLevel() == 10)
			{
				finalFightImage->Activate();
				//set position al centre on hi han ordas fe penya forta
			}
		}
	}
	else{
		introductionImage->Desactivate();
		secondImage->Desactivate();
		readyToGoImage->Desactivate();
		secondIntroductionImage->Desactivate();
		secondSecondaryImage->Desactivate();
		finalFightImage->Desactivate();
	}

	return true;
}

void NpcCounselor::draw()
{
	imageSprite->updateSprite(worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->getCurrentFrame());
}

void NpcCounselor::drawDebug()
{
	iPoint position = getWorldPosition();

	app->render->DrawCircle(position.x, position.y, playerRange, 0, 0, 255);
}

bool NpcCounselor::playerInRange()
{
	iPoint target_player = app->game->player->getWorldPosition();

	iPoint dist;

	dist.x = target_player.x - getWorldPosition().x;
	dist.y = target_player.y - getWorldPosition().y;

	float ret = dist.getModule();
	ret = ret;
	float range = sqrt(dist.x*dist.x + dist.y*dist.y);

	if (playerRange > ret)
	{
		return true;
	}

	return false;
}

void NpcCounselor::loadGui(){
	iPoint p = { 0, 0 };
	introductionImage = app->gui->addGuiImage(p, { 1024, 704, 409, 157 }, NULL, NULL);
	introductionImage->Desactivate();
	secondImage = app->gui->addGuiImage(p, { 1472, 704, 111, 86 }, NULL, NULL);
	secondImage->Desactivate();
	readyToGoImage = app->gui->addGuiImage(p, {1216,896,111,86}, NULL, NULL);
	readyToGoImage->Desactivate();
	secondIntroductionImage = app->gui->addGuiImage(p, {1345,895,111,85}, NULL, NULL);
	secondIntroductionImage->Desactivate();
	secondSecondaryImage = app->gui->addGuiImage(p, {1472,832,111,85}, NULL, NULL);
	secondSecondaryImage->Desactivate();
	finalFightImage = app->gui->addGuiImage(p, {1472,960,111,85}, NULL, NULL);
	finalFightImage->Desactivate();
}

//Constructor
NpcHealer::NpcHealer(const iPoint &p, uint ID)
{
	type = entityType::NPC_HEALER;
	loadGui();
	setWorldPosition(p);
	playerRange = 70.0f;
	
	colliderOffset.Set(25, 75);
	colliderSize.Set(45, 80);

	currentState = E_IDLE;
	currentDirection = E_DOWN;

	SDL_Rect rect = { worldPosition.x - colliderOffset.x,
		worldPosition.y - colliderOffset.y,	// Position
		colliderSize.x, colliderSize.y };		// Size
	
	collider = app->collision->addCollider(rect, COLLIDER_NPC, app->game->em);

	entityAnim = app->game->em->getHealerAnimation();
	currentAnimation = &entityAnim.find({ currentState, currentDirection })->second;
	imageSprite = new Sprite(app->game->em->getHealerTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	readed = 0;
}

NpcHealer::~NpcHealer(){

}

//update
bool NpcHealer::entityUpdate(float dt)
{
	if (playerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && mouseHover())
		{
			if (readed < 3)
			{
				introductionImage->Activate();
				readed++;
			}
			else
			{
				num = 1 + rand()&(101 - 1);
				if (num <=25)
				{
					goodDayImage->Activate();
					introductionImage->Desactivate();
					helloImage->Desactivate();
					changeImage->Desactivate();
					lordImage->Desactivate();
				}
				else if (25<num && num<=50)
				{
					helloImage->Activate();
					introductionImage->Desactivate();
					goodDayImage->Desactivate();
					changeImage->Desactivate();
					lordImage->Desactivate();
				}
				else if (50<num && num <=75)
				{
					changeImage->Activate();
					introductionImage->Desactivate();
					goodDayImage->Desactivate();
					helloImage->Desactivate();
					lordImage->Desactivate();
				}
				else if (75<num)
				{
					lordImage->Activate();
					introductionImage->Desactivate();
					goodDayImage->Desactivate();
					helloImage->Desactivate();
					changeImage->Desactivate();
				}
			}
		}
	}
	else{
		introductionImage->Desactivate();
		goodDayImage->Desactivate();
		helloImage->Desactivate();
		changeImage->Desactivate();
		lordImage->Desactivate();
	}

	return true;
}

void NpcHealer::draw()
{
	imageSprite->updateSprite(worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->getCurrentFrame());
}

void NpcHealer::drawDebug()
{
	iPoint position = getWorldPosition();

	app->render->DrawCircle(position.x, position.y, playerRange, 0, 0, 255);
}

bool NpcHealer::playerInRange()
{
	iPoint target_player = app->game->player->getWorldPosition();

	fPoint dist;

	dist.x = target_player.x - getWorldPosition().x;
	dist.y = target_player.y - getWorldPosition().y;

	float ret = dist.getModule();
	ret = ret;

	if (playerRange > ret)
	{
		return true;
	}

	return false;
}

void NpcHealer::loadGui(){

	iPoint p = { 0, 0 };
	introductionImage = app->gui->addGuiImage(p, { 1600, 0, 409, 157 }, NULL, NULL);
	introductionImage->Desactivate();
	goodDayImage = app->gui->addGuiImage(p, { 1600, 192, 111, 40 },NULL,NULL);
	goodDayImage->Desactivate();
	helloImage = app->gui->addGuiImage(p, {1600,256,111,40}, NULL, NULL);
	helloImage->Desactivate();
	changeImage = app->gui->addGuiImage(p, {1728,192,111,40}, NULL, NULL);
	changeImage->Desactivate();
	lordImage = app->gui->addGuiImage(p, {1728,256,111,40}, NULL, NULL);
	lordImage->Desactivate();
}

NpcGossip::NpcGossip(const iPoint &p, uint ID)
{
	type = entityType::NPC_GOSSIP;
	setWorldPosition(p);
	playerRange = 70.0f;
	loadGui();
	colliderOffset.Set(25, 75);
	colliderSize.Set(45, 80);

	SDL_Rect rect = { worldPosition.x - colliderOffset.x,
		worldPosition.y - colliderOffset.y,	// Position
		colliderSize.x, colliderSize.y };		// Size
	collider = app->collision->addCollider(rect, COLLIDER_NPC, app->game->em);

	entityAnim = app->game->em->getGossipAnimation();
	currentAnimation = &entityAnim.find({ E_IDLE, E_DOWN })->second;
	imageSprite = new Sprite(app->game->em->getGossipTexture(), worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->PeekCurrentFrame());
	app->render->addSpriteToList(imageSprite);

	readed = 0;
}

NpcGossip::~NpcGossip(){

}

bool NpcGossip::entityUpdate(float dt)
{
	if (playerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && mouseHover())
		{
			if (readed < 3)
			{
				introductionImage->Activate();
				readed++;
			}
			else
			{
				num = 1 + rand()&(101 - 1);
				if (num <= 25)
				{
					goodDayImage->Activate();
					introductionImage->Desactivate();
					helloImage->Desactivate();
					changeImage->Desactivate();
					lordImage->Desactivate();
				}
				else if (25<num && num <= 50)
				{
					helloImage->Activate();
					introductionImage->Desactivate();
					goodDayImage->Desactivate();
					changeImage->Desactivate();
					lordImage->Desactivate();
				}
				else if (50<num && num <= 75)
				{
					changeImage->Activate();
					introductionImage->Desactivate();
					goodDayImage->Desactivate();
					helloImage->Desactivate();
					lordImage->Desactivate();
				}
				else if (75<num)
				{
					lordImage->Activate();
					introductionImage->Desactivate();
					goodDayImage->Desactivate();
					helloImage->Desactivate();
					changeImage->Desactivate();
				}
			}
		}
	}
	else{
		introductionImage->Desactivate();
		goodDayImage->Desactivate();
		helloImage->Desactivate();
		changeImage->Desactivate();
		lordImage->Desactivate();
	}

	return true;
}

void NpcGossip::draw()
{
	imageSprite->updateSprite(worldPosition, currentAnimation->pivot, (SDL_Rect)currentAnimation->getCurrentFrame());
}

void NpcGossip::drawDebug()
{
	iPoint position = getWorldPosition();

	app->render->DrawCircle(position.x, position.y, playerRange, 0, 0, 255);
}

bool NpcGossip::playerInRange()
{
	iPoint target_player = app->game->player->getWorldPosition();

	fPoint dist;

	dist.x = target_player.x - getWorldPosition().x;
	dist.y = target_player.y - getWorldPosition().y;

	float ret = dist.getModule();
	ret = ret;

	if (playerRange > ret)
	{
		return true;
	}

	return false;
}

void NpcGossip::loadGui(){

	iPoint p = { 0, 0 };
	introductionImage = app->gui->addGuiImage(p, { 1600, 320, 409, 157 }, NULL, NULL);
	introductionImage->Desactivate();
	goodDayImage = app->gui->addGuiImage(p, { 1600, 192, 111, 40 }, NULL, NULL);
	goodDayImage->Desactivate();
	helloImage = app->gui->addGuiImage(p, { 1600, 256, 111, 40 }, NULL, NULL);
	helloImage->Desactivate();
	changeImage = app->gui->addGuiImage(p, { 1728, 192, 111, 40 }, NULL, NULL);
	changeImage->Desactivate();
	lordImage = app->gui->addGuiImage(p, { 1728, 256, 111, 40 }, NULL, NULL);
	lordImage->Desactivate();
}
