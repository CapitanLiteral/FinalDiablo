#include "snOutdoor1.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Input.h"
#include "SceneManager.h"
#include "snIntro.h"
#include "Map.h"
#include "Gui.h"
#include "Textures.h"
#include "Player.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Pathfinding.h"
#include "Entity.h"
#include "Game.h"
#include "Act1.h"
#include "EntPortal.h"
#include "EntEnemy.h"

// quit log en no debug
#include "p2Log.h"

using namespace std;

// Constructor
snOutdoor1::snOutdoor1() : Scene()
{
	name.create("outdoor1");
}

// Destructor
snOutdoor1::~snOutdoor1()
{}

// Called before render is available
bool snOutdoor1::awake(pugi::xml_node& conf)
{
	return true;
}

// Called the first frame
bool snOutdoor1::start()
{
	//Music
	//NOTE : deactivated for debugging
	//app->audio->PlayMusic("audio/music/town1.ogg", 0);

	if (debug == NULL)
		debug = app->tex->Load("maps/mini_path.png");
	
	//Map
	if(app->map->Load("map_swamp.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->game->player->SetPosition({ 0, 0 });

	//NOTE: PORTAL, provisional
	iPoint tile_pos = { -100, 282 };
	Entity* to_add = app->game->em->add(tile_pos, PORTAL);
	entity_list.push_back(to_add);
	EntPortal* portal = (EntPortal*)to_add;
	if (portal)
		portal->SetDestiny(portal->destiny = app->sm->act1);

	//NOTE: Test Sprite

	/*
	SDL_Rect position = { 2, 2, 0, 0 };
	pos_cow = &position;
	SDL_Rect section = { 0, 0, 96, 120 };
	sect_cow = &section;
	
	sprite_cow = new Sprite(NULL, sect_cow, pos_cow);
	sprite_cow->texture = app->tex->Load("textures/cow.png");
	sprite_cow->positionMap.x = -100;
	sprite_cow->positionMap.y = 100;
	sprite_cow->positionMap.w = 0;
	sprite_cow->positionMap.h = 0;
	sprite_cow->y = 0;

	sprite_cow->sectionTexture.x = 0;
	sprite_cow->sectionTexture.y = 0;
	sprite_cow->sectionTexture.w = 0;
	sprite_cow->sectionTexture.h = 0;

	app->render->addSpriteToList(sprite_cow);


	SDL_Rect positione = { 2, 2, 0, 0 };
	pos_enemy = &positione;
	SDL_Rect sectione = { 0, 0, 96, 120 };
	sect_enemy = &sectione;

	enemy = new Sprite(NULL, sect_enemy, pos_enemy);
	enemy->texture = app->tex->Load("textures/enemy1.png");
	enemy->positionMap.x = -100;
	enemy->positionMap.y = 150;
	enemy->positionMap.w = 0;
	enemy->positionMap.h = 0;

	enemy->sectionTexture.x = 0;
	enemy->sectionTexture.y = 0;
	enemy->sectionTexture.w = 0;
	enemy->sectionTexture.h = 0;
	enemy->y = -1;

	app->render->addSpriteToList(enemy);
	
	SDL_Rect positionee = { 2, 2, 0, 0 };
	pos_enemy2 = &positionee;
	SDL_Rect sectionee = { 0, 0, 96, 120 };
	sect_enemy2 = &sectionee;

	enemy2 = new Sprite(NULL, sect_enemy2, pos_enemy2);
	enemy2->texture = app->tex->Load("textures/enemy1.png");
	enemy2->positionMap.x = -20;
	enemy2->positionMap.y = 130;
	enemy2->positionMap.w = 0;
	enemy2->positionMap.h = 0;

	enemy2->sectionTexture.x = 0;
	enemy2->sectionTexture.y = 0;
	enemy2->sectionTexture.w = 0;
	enemy2->sectionTexture.h = 0;
	enemy2->y = 1;

	app->render->addSpriteToList(enemy2);
	*/
	

	
	return true;
}

// preUpdate
bool snOutdoor1::preUpdate()
{
	//NOTE: just to test the pathfinding
	// debug pathfing ------------------
	/*static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	app->input->getMousePosition(x, y);
	iPoint p = app->render->ScreenToWorld(x, y);
	p = app->map->WorldToMap(p.x, p.y);

	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			app->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}*/
	//
	return true;
}

// update
bool snOutdoor1::update(float dt)
{	
	//NOTE: Debug feature so we can test entities
	//add Entity
	if (app->input->getKey(SDL_SCANCODE_P) == KEY_DOWN && app->debug)
	{
		iPoint p;
		p = app->input->getMouseWorldPosition();
		p.x += app->map->data.tile_width/2;
		p.y += app->map->data.tile_height/2;

		//int a = rand() % 2;
		//if (a == 0)
			//app->game->em->add(p, ENEMY);
		//if (a == 1)
			EntPortal* ent = (EntPortal*)app->game->em->add(p, PORTAL);
			if (ent)
				ent->SetDestiny(app->sm->act1);
			
	}

	else if (app->input->getKey(SDL_SCANCODE_Q) == KEY_DOWN && app->debug)
	{
		iPoint p;
		p = app->input->getMouseWorldPosition();
		p.x += app->map->data.tile_width / 2;
		p.y += app->map->data.tile_height / 2;

		Entity* ent = app->game->em->addEnemy(p, ENEMY_WOLF);

	}

	else if (app->input->getKey(SDL_SCANCODE_W) == KEY_DOWN && app->debug)
	{
		iPoint p;
		p = app->input->getMouseWorldPosition();
		p.x += app->map->data.tile_width / 2;
		p.y += app->map->data.tile_height / 2;

		Entity* ent = app->game->em->addEnemy(p, ENEMY_CRAWLER);
	}

	else if (app->input->getKey(SDL_SCANCODE_E) == KEY_DOWN && app->debug)
	{
		iPoint p;
		p = app->input->getMouseWorldPosition();
		p.x += app->map->data.tile_width / 2;
		p.y += app->map->data.tile_height / 2;

		Entity* ent = app->game->em->addEnemy(p, ENEMY_COUNCIL);
	}

	//Map
	app->map->draw();
	
	//Player
	
	//app->game->player->draw();
	
	//Camera
	//Free movement only avaliable on debug mode
	
	/*if (app->debug)
	{
		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			app->render->camera.x -= floor(CAM_SPEED*dt);
		}

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			app->render->camera.x += floor(CAM_SPEED*dt);
		}

		if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			app->render->camera.y -= floor(CAM_SPEED*dt);
		}

		if (app->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			app->render->camera.y += floor(CAM_SPEED*dt);
		}
	}*/

	//Pathfinding debug
	//NOTE: uncomment for testing pathfinding
	int x, y;
	app->input->getMousePosition(x, y);
	iPoint p = app->render->ScreenToWorld(x, y);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->getTileBlit(p.x, p.y);

	app->render->Blit(debug, p.x, p.y);
	/*
	if (app->input->getKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		sprite_cow->positionMap.x = sprite_cow->positionMap.x -= floor(CAM_SPEED*dt);
	}

	if (app->input->getKey(SDL_SCANCODE_H) == KEY_REPEAT)
	{
		sprite_cow->positionMap.x += floor(CAM_SPEED*dt);
	}

	if (app->input->getKey(SDL_SCANCODE_T) == KEY_REPEAT)
	{
		sprite_cow->positionMap.y -= floor(CAM_SPEED*dt);
	}

	if (app->input->getKey(SDL_SCANCODE_G) == KEY_REPEAT)
	{
		sprite_cow->positionMap.y += floor(CAM_SPEED*dt);
	}
	*/



	/*
	
	const vector<iPoint>* path = app->pathfinding->getLastPath();

	for (uint i = 0; i < path->size(); ++i)
	{
		iPoint pos = app->map->getTileBlit(path->at(i).x, path->at(i).y);
		
		app->render->Blit(debug, pos.x, pos.y);
	}
	*/
	
	return true;
}

// postUpdate
bool snOutdoor1::postUpdate()
{
	//NOTE: In progress
	if (app->game->player->TeleportReady())
	{
		Scene* destiny = app->game->player->getDestiny();
		app->game->player->ResetTeleport();
		app->sm->ChangeScene(destiny);
	}

	return true;
}

// Called before quitting
bool snOutdoor1::cleanUp()
{
	// remove all entities
	list<Entity*>::iterator item;
	item = entity_list.begin();

	while (item != entity_list.end())
	{
		app->game->em->remove(item._Ptr->_Myval->id);
		item++;
	}
	entity_list.clear();

	return true;
}

// Called on certain event
void snOutdoor1::OnEvent(GuiElement* element, GUI_Event even)
{

}

//Load
bool snOutdoor1::Load()
{
	start();
	return true;
}

//unLoad
bool snOutdoor1::unLoad()
{
	cleanUp();
	app->map->cleanUp();
	app->pathfinding->cleanUp();
	return true;
}

