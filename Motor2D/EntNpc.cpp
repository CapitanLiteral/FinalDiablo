#include "EntNpc.h"
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


//EntEnemy
//----------------------------

//Constructor
EntNpc::EntNpc(const iPoint &p, uint ID) : EntStatic(p, ID)
{
	worldPosition = p;
	type = NPC;
}

EntNpc::~EntNpc(){

}
//Drawing methods
void EntNpc::draw()
{
	//iPoint pos = getBlitPosition();

	if (sprite)
	{	//Uncomment for draw animations
		/*SDL_Rect current_sprite = current_animation->getCurrentFrame();
		fPoint p = getPivotPosition();
		iPoint pos(p.x, p.y);
		sprite->updateSprite(tex, pos, sprite_pivot, current_sprite);*/

		fPoint p = getPivotPosition();
		iPoint pos(p.x, p.y);
		sprite->updateSprite(tex, pos, sprite_pivot, sprite_rect);

	}

}

void EntNpc::drawDebug()
{
	iPoint t_pos = getTilePosition();
	fPoint p_pos = getPivotPosition();
	SDL_Rect rect = getPlayerRect();

	app->render->DrawQuad({ p_pos.x, p_pos.y, 3, 3 }, 255, 0, 0, 255, false);

	app->render->DrawQuad(rect, 255, 0, 0, 255, false);

	app->render->DrawCircle(p_pos.x, p_pos.y, playerRange, 0, 0, 255);
}


bool EntNpc::PlayerInRange()
{
	iPoint target_player = app->game->player->getWorldPosition();

	fPoint dist;

	dist.x = target_player.x - position.x;
	dist.y = target_player.y - position.y;

	float ret = dist.getModule();
	ret = ret;

	if (playerRange > ret)
	{
		return true;
	}

	return false;
}
iPoint EntNpc::getMapPosition() const{
	return mapPosition;
}
iPoint EntNpc::getWorldPosition() const{
	return worldPosition;
}

void EntNpc::setMapPosition(iPoint tile){
	mapPosition = tile;
}

void EntNpc::setWorldPosition(iPoint coords){
	worldPosition = coords;
}

Collider* EntNpc::getCollider() const{
	return collider;
}

iPoint EntNpc::getColliderOffset() const{
	return colliderOffset;
}

iPoint EntNpc::getColliderSize() const{
	return colliderSize;
}


//Constructor
EntCounselor::EntCounselor(const iPoint &p, uint ID) : EntNpc(p, ID)
{
	npcType = NPC_COUNSELOR;
	tex = idle_tex = app->tex->Load("images/aidanIdle.png");
	loadGui();
	//SetAnimations();
	//current_animation_set = idle;
	setWorldPosition(p);
	sprite_rect = collider_rect = {39,32,34,74};
	sprite_pivot = {17,74};
	colliderOffset = { 17, 74 };
	playerRange = 70.0f;

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);

	//Sprite creation
	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	//SDL_Rect current_sprite = current_animation->getCurrentFrame();
	SDL_Rect current_sprite = sprite_rect;
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}
EntCounselor::~EntCounselor(){

}

//update
bool EntCounselor::update(float dt)
{
	if (PlayerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN){
			if (Entity* ent = app->game->em->entityOnMouse())
			{
				//Able GUI NPC
				PlayerInRange();
			}
		}
	}
	else{
		//Disable NPC GUI
	}

	return true;
}

void EntCounselor::SetAnimations()
{
	//NOTE: this should not go here
	collider_rect.w = sprite_rect.w = sprite_dim.x = 34;
	collider_rect.h = sprite_rect.h = sprite_dim.y = 74;

	sprite_pivot = { collider_rect.w / 2, collider_rect.h};
	//sprite_pivot.y += 5;



	//NOTE: Do it like a player, no number, variables
	//Idle
	for (int i = 1; i < 8; i++)
	{
		Animation tmp;
		tmp.SetFrames((39) * i, 32, 34, 74, 16, 4);
		tmp.speed = 0.2f;

		idle.push_back(tmp);
	}
}

void EntCounselor::loadGui(){
	counselorAtlas = app->tex->Load("gui/AidanAtlas.png");
	iPoint p = getWorldPosition();
	p.x -= 10;
	p.y -= 10;
	startingImage = app->gui->addGuiImage(p, {}, NULL, NULL);
}
//-----------------------------

//EntEnemyCrawler
//----------------------------

//Constructor
EntHealer::EntHealer(const iPoint &p, uint ID) : EntNpc(p, ID)
{
	tex = idle_tex = app->tex->Load("images/atmaIdle.png");

	//SetAnimations();
	//current_animation_set = idle;
	setWorldPosition(p);
	npcType = NPC_HEALER;
	sprite_rect = collider_rect = { 27, 35, 28, 66 };
	sprite_pivot = { 15, 66 };
	colliderOffset = { 15, 66 };
	playerRange = 70.0f;
	loadGui();
	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);

	//Sprite creation

	//Sprite creation
	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	//SDL_Rect current_sprite = current_animation->getCurrentFrame();
	SDL_Rect current_sprite = sprite_rect;
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}

EntHealer::~EntHealer(){

}

//update
bool EntHealer::update(float dt)
{
	if (PlayerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN){
			if (Entity* ent = app->game->em->entityOnMouse())
			{
				startingImage->Activate();
			}
		}
	}
	else{
		startingImage->Desactivate();
	}

	return true;
}



void EntHealer::SetAnimations()
{
	//NOTE: this should not go here
	collider_rect.w = sprite_rect.w = sprite_dim.x = 78;
	collider_rect.h = sprite_rect.h = sprite_dim.y = 51;

	sprite_pivot = { collider_rect.w / 2, collider_rect.h - 20 };
	sprite_pivot.y += 5;



	//NOTE: Do it like a player, no number, variables
	//Idle
	for (int i = 0; i < 8; i++)
	{
		Animation tmp;
		tmp.SetFrames(0, (51 + 0) * i, 78, 51, 4, 0);
		tmp.speed = 0.2f;

		idle.push_back(tmp);
	}

}

void EntHealer::loadGui(){
	
	iPoint p = { 0, 0 };
	startingImage = app->gui->addGuiImage(p, {1457,697,409,156}, NULL, NULL);
	startingImage->Desactivate();
}
//-----------------------------

//EntEnemyBoss
//----------------------------

//Constructor
EntGossip::EntGossip(const iPoint &p, uint ID) : EntNpc(p, ID)
{
	tex = idle_tex = app->tex->Load("images/alkorIdle.png");

	//SetAnimations();
	//current_animation_set = idle;
	setWorldPosition(p);
	npcType = NPC_HEALER;
	sprite_rect = collider_rect = { 20, 35, 25, 68 };
	sprite_pivot = { 12, 68 };
	colliderOffset = { 15, 68 };
	playerRange = 70.0f;

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);

	//Sprite creation

	//Sprite creation
	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	//SDL_Rect current_sprite = current_animation->getCurrentFrame();
	SDL_Rect current_sprite = sprite_rect;
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

}

EntGossip::~EntGossip(){

}

//update
bool EntGossip::update(float dt)
{
	if (PlayerInRange())
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN){
			if (Entity* ent = app->game->em->entityOnMouse())
			{
				//GUI NPC
			}
		}
	}
	else{
		//Disable NPC GUI
	}

	return true;
}

void EntGossip::SetAnimations()
{
	//NOTE: this should not go here
	collider_rect.w = sprite_rect.w = sprite_dim.x = 77;
	collider_rect.h = sprite_rect.h = sprite_dim.y = 97;

	sprite_pivot = { collider_rect.w / 2, collider_rect.h - 20 };
	sprite_pivot.y += 5;



	//NOTE: Do it like a player, no number, variables
	//Idle
	for (int i = 0; i < 8; i++)
	{
		Animation tmp;
		tmp.SetFrames(0, (97 + 0) * i, 77, 97, 7, 0);
		tmp.speed = 0.2f;

		idle.push_back(tmp);
	}
}