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
//Provisional?
#include "Audio.h"
#include "Animation.h"

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


//Constructor
NpcCounselor::NpcCounselor(const iPoint &p, uint ID)
{
	type = entityType::NPC_COUNSELOUR;
	loadGui();
	setWorldPosition(p);
	playerRange = 70.0f;

	SDL_Rect rect = { worldPosition.x - colliderOffset.x,
			worldPosition.y - colliderOffset.y,	// Position
			colliderSize.x, colliderSize.y };		// Size
	collider = app->collision->addCollider(rect, COLLIDER_NPC, app->game->em);

	entityAnim = app->game->em->getCounselorAnimation();
	//currentAnimation = entityAnim->find({ IDLE, E_DOWN })->second;
	imageSprite = new Sprite(app->game->em->getCounselorTexture(), worldPosition, /*currentAnimation->pivot*/iPoint{ 0, 0 }, SDL_Rect{ 0, 0, 0, 0 });
	app->render->addSpriteToList(imageSprite);

}

NpcCounselor::~NpcCounselor(){

}

bool NpcCounselor::entityUpdate(float dt)
{
	if (playerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN){
			//if (Entity* ent = app->game->em->getEntityOnMouse())
			//{
			//	//Able GUI NPC
			//	playerInRange();
			//}
		}
	}
	else{
		//Disable NPC GUI
	}

	return true;
}

void NpcCounselor::loadGui(){
	
}

//Constructor
NpcHealer::NpcHealer(const iPoint &p, uint ID)
{
	type = entityType::NPC_COUNSELOUR;
	loadGui();
	setWorldPosition(p);
	playerRange = 70.0f;
	
	colliderOffset.Set(41, 94);
	colliderSize.Set(64, 128);

	SDL_Rect rect = { worldPosition.x - colliderOffset.x,
		worldPosition.y - colliderOffset.y,	// Position
		colliderSize.x, colliderSize.y };		// Size
	
	collider = app->collision->addCollider(rect, COLLIDER_NPC, app->game->em);

	entityAnim = app->game->em->getHealerAnimation();
	imageSprite = new Sprite(app->game->em->getHealerTexture(), worldPosition,/* currentAnimation->pivot*/ iPoint{ 41, 94 }, SDL_Rect{ 0, 0, 0, 0 });
	app->render->addSpriteToList(imageSprite);

}

NpcHealer::~NpcHealer(){

}

void NpcHealer::draw()
{
	imageSprite->updateSprite(worldPosition, iPoint{ 41, 94 }, SDL_Rect{0,0,64,128});
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
//update
bool NpcHealer::entityUpdate(float dt)
{
	if (playerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN){
		//	if (Entity* ent = app->game->em->getEntityOnMouse())
			//{
		//		startingImage->Activate();
		//	}
		}
	}
	else{
		startingImage->Desactivate();
	}

	return true;
}


void NpcHealer::loadGui(){

	iPoint p = { 0, 0 };
	startingImage = app->gui->addGuiImage(p, { 1457, 697, 409, 156 }, NULL, NULL);
	startingImage->Desactivate();
}

//Constructor
NpcGossip::NpcGossip(const iPoint &p, uint ID)
{
	type = entityType::NPC_COUNSELOUR;
	setWorldPosition(p);
	playerRange = 70.0f;

	SDL_Rect rect = { worldPosition.x - colliderOffset.x,
		worldPosition.y - colliderOffset.y,	// Position
		colliderSize.x, colliderSize.y };		// Size
	collider = app->collision->addCollider(rect, COLLIDER_NPC, app->game->em);

	entityAnim = app->game->em->getGossipAnimation();
	imageSprite = new Sprite(app->game->em->getGossipTexture(), worldPosition,/* currentAnimation->pivot*/ iPoint{ 0, 0 }, SDL_Rect{ 0, 0, 0, 0 });
	app->render->addSpriteToList(imageSprite);

}

NpcGossip::~NpcGossip(){

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
//update
bool NpcGossip::entityUpdate(float dt)
{
	if (playerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN){
			//if (Entity* ent = app->game->em->getEntityOnMouse())
			//{
			//	//GUI NPC
			//}
		}
	}
	else{
		//Disable NPC GUI
	}

	return true;
}

void NpcGossip::loadGui(){

	iPoint p = { 0, 0 };
	startingImage = app->gui->addGuiImage(p, { 1457, 697, 409, 156 }, NULL, NULL);
	startingImage->Desactivate();
}
