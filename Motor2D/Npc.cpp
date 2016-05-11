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

//Constructor
Npc::Npc(const iPoint &p, uint ID) : Entity()
{
	worldPosition = p;
}

Npc::~Npc(){

}

void Npc::draw()
{
	//iPoint pos = getBlitPosition();

	if (imageSprite)
	{	//Uncomment for draw animations
		/*SDL_Rect current_sprite = current_animation->getCurrentFrame();
		fPoint p = getPivotPosition();
		iPoint pos(p.x, p.y);
		sprite->updateSprite(tex, pos, sprite_pivot, current_sprite);*/

		iPoint pos = getWorldPosition();
		//sprite->updateSprite(tex, pos, sprite_pivot, sprite_rect);
		imageSprite->DrawSprite();

	}

}

void Npc::drawDebug()
{
	iPoint p_pos = getWorldPosition();
	//SDL_Rect rect = getPlayerRect();

	app->render->DrawQuad({ p_pos.x, p_pos.y, 3, 3 }, 255, 0, 0, 255, false);

//	app->render->DrawQuad(rect, 255, 0, 0, 255, false);

	app->render->DrawCircle(p_pos.x, p_pos.y, playerRange, 0, 0, 255);
}


bool Npc::playerInRange()
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


//Constructor
NpcCounselor::NpcCounselor(const iPoint &p, uint ID) : Npc(p, ID)
{
	type = entityType::NPC_COUNCELOUR;
	texture = app->tex->Load("images/aidanIdle.png");
	loadGui();
	setWorldPosition(p);
	spriteRect = { 39, 32, 34, 74 };
	spritePivot = { 17, 74 };
	playerRange = 70.0f;

	//collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);

	//Sprite creation
	iPoint pos = getWorldPosition();
	//SDL_Rect current_sprite = current_animation->getCurrentFrame();
	imageSprite = new Sprite(texture, pos, spritePivot, spriteRect);
	app->render->addSpriteToList(imageSprite);

}

NpcCounselor::~NpcCounselor(){

}

bool NpcCounselor::update(float dt)
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
	counselorAtlas = app->tex->Load("gui/AidanAtlas.png");
	iPoint p = getWorldPosition();
	p.x -= 10;
	p.y -= 10;
	startingImage = app->gui->addGuiImage(p, {}, NULL, NULL);
}

//Constructor
NpcHealer::NpcHealer(const iPoint &p, uint ID) : Npc(p, ID)
{
	texture = app->tex->Load("images/atmaIdle.png");
	type = entityType::NPC_HEALER;

	setWorldPosition(p);
	spriteRect = { 27, 35, 28, 66 };
	spritePivot = { 15, 66 };
	colliderOffset = { 15, 66 };
	playerRange = 70.0f;
	loadGui();
//	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);

	iPoint pos = getWorldPosition();
	imageSprite = new Sprite(texture, pos, spritePivot, spriteRect);
	app->render->addSpriteToList(imageSprite);

}

NpcHealer::~NpcHealer(){

}

//update
bool NpcHealer::update(float dt)
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
NpcGossip::NpcGossip(const iPoint &p, uint ID) : Npc(p, ID)
{
	texture = app->tex->Load("images/alkorIdle.png");
	type = entityType::NPC_GOSSIP;

	setWorldPosition(p);
	spriteRect = { 20, 35, 25, 68 };
	spritePivot = { 12, 68 };
	colliderOffset = { 15, 68 };
	playerRange = 70.0f;

	//collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);

	//Sprite creation

	//Sprite creation
	iPoint pos = getWorldPosition();
	//SDL_Rect current_sprite = current_animation->getCurrentFrame();
	imageSprite = new Sprite(texture, pos, spritePivot, spriteRect);
	app->render->addSpriteToList(imageSprite);

}

NpcGossip::~NpcGossip(){

}

//update
bool NpcGossip::update(float dt)
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

