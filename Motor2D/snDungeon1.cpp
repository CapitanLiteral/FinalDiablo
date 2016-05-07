#include "snDungeon1.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Game.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Input.h"
#include "SceneManager.h"
#include "snWin.h"
#include "Audio.h"
#include "snOutdoor1.h"
#include "Player.h"
#include "EntPortal.h"
#include "EntEnemy.h"



// Constructor
snDungeon1::snDungeon1()
{

}

// Destructor
snDungeon1::~snDungeon1()
{

}

// Called before render is available
bool snDungeon1::awake(pugi::xml_node& conf)
{
	return true;
}


// Called the first frame
bool snDungeon1::start()
{
	win = false;

	//app->audio->PlayMusic("audio/music/town1.ogg", 0);

	if (debug == NULL)
		debug = app->tex->Load("maps/mini_path.png");

	//Map
	if (app->map->Load("map_dungeon.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->game->player->SetPosition({0, 500});

	addEnemies();

	return true;
}


// preUpdate
bool snDungeon1::preUpdate()
{
	return true;
}


// update
bool snDungeon1::update(float dt)
{
	//Map
	app->map->draw();

	if (app->debug)
	{
		//Entities for debug
		if (app->input->getKey(SDL_SCANCODE_P) == KEY_DOWN && debug)
		{
			iPoint p;
			p = app->input->getMouseWorldPosition();
			p.x += app->map->data.tile_width / 2;
			p.y += app->map->data.tile_height / 2;

			//int a = rand() % 2;
			//if (a == 0)
			//app->game->em->add(p, ENEMY);
			//app->game->em->add(p, ENEMY_CRAWLER);
			EntPortal* portal = (EntPortal*)app->game->em->add(p, PORTAL);
			if (portal)
				portal->destiny = app->sm->outdoor1;

			p = app->map->WorldToMap(p.x, p.y);
			int i = 0;
		}

		if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			iPoint p;
			p = app->input->getMouseWorldPosition();
			p.x += app->map->data.tile_width / 2;
			p.y += app->map->data.tile_height / 2;

			//int a = rand() % 2;
			//if (a == 0)
			//app->game->em->add(p, ENEMY);
			//app->game->em->add(p, ENEMY_CRAWLER);
			app->game->em->addEnemy(p, ENEMY_CRAWLER);
			p = app->map->WorldToMap(p.x, p.y);
			int i = 0;
		}

		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			app->render->camera.x -= floor(CAM_SPEED * 5 * dt);
		}

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			app->render->camera.x += floor(CAM_SPEED * 5 * dt);
		}

		if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			app->render->camera.y -= floor(CAM_SPEED * 5 * dt);
		}

		if (app->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			app->render->camera.y += floor(CAM_SPEED * 5 * dt);
		}
	}

	return true;
}


// postUpdate
bool snDungeon1::postUpdate()
{
	//NOTE: In progress
	if (app->game->player->TeleportReady())
	{
		Scene* destiny = app->game->player->getDestiny();
		app->game->player->ResetTeleport();
		app->sm->fadeToBlack(destiny);
	}

	if (win)
	{
		app->sm->fadeToBlack(app->sm->win);
	}
	return true;
}


// Called before quitting
bool snDungeon1::cleanUp()
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
void snDungeon1::OnEvent(GuiElement* element, GUI_Event even)
{

}


//Load/unLoad, called when the scene changes
bool snDungeon1::Load()
{
	start();
	return true;
}

bool snDungeon1::unLoad()
{
	cleanUp();
	app->map->cleanUp();
	app->pathfinding->cleanUp();
	return true;
}

//adding Enemies
void snDungeon1::addEnemies()
{
	iPoint tile_pos;
	Entity* to_add = NULL;

	tile_pos = { -140, 417 };
	to_add = app->game->em->add(tile_pos, PORTAL);
	entity_list.push_back(to_add);
	EntPortal* portal = (EntPortal*)to_add;
	if (portal)
		portal->SetDestiny(portal->destiny = app->sm->outdoor1);

	tile_pos = { 182, 80 };
	for (int i = 0; i < 12; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x -= 4;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		tile_pos.y -= 4;
	}

	tile_pos = { 159, 92 };
	for (int i = 0; i < 6; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		tile_pos.y -= 4;
	}

	tile_pos = { 105, 82 };
	for (int i = 0; i < 6; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		tile_pos.y -= 3;
	}

	tile_pos = { 204, 47 };
	for (int i = 0; i < 7; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 2;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		tile_pos.y -= 3;
	}

	tile_pos = { 52, 108 };
	for (int i = 0; i < 4; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		tile_pos.y -= 4;
	}

	tile_pos = { 131,	35 };
	for (int i = 0; i < 13; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 1;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		tile_pos.y -= 2;
	}

	tile_pos = { 127, 160 };
	for (int i = 0; i < 13; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 2;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
		
		tile_pos.y -= 3;
	}

	tile_pos = { 204, 141 };
	for (int i = 0; i < 8; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 1;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
		tile_pos.y -= 2;
	}

	tile_pos = { 204, 222 };
	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
			tile_pos.y -= 3;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
	}

	tile_pos = { 96, 222 };
	for (int i = 0; i < 8; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
			tile_pos.y -= 3;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
	}

	tile_pos = { 100, 205 };
	for (int i = 0; i < 8; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
			tile_pos.y += 3;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
	}

	tile_pos = { 18, 94 };
	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
		}

		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);
		
		tile_pos.y += 2;
	}

	tile_pos = { 19, 120 };
	for (int i = 0; i < 8; i++)
	{

		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);

		tile_pos.y += 2;
	}

	tile_pos = { 69, 22 };
	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
			tile_pos.y += 3;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
	}

	tile_pos = { 19, 216 };
	for (int i = 0; i < 12; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 2;
			tile_pos.y += 2;
		}
		to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
		if (to_add != NULL)
			entity_list.push_back(to_add);

		tile_pos.x -= 1;
		tile_pos.y -= 3;
	}

	tile_pos = { 93, 138 };
	app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_COUNCIL);

	tile_pos = { 93, 149 };
	for (int i = 0; i < 6; i++)
	{
		if (i % 2 == 1)
		{
			tile_pos.x += 3;
			tile_pos.y -= 3;
		}
		else
		{
			to_add = app->game->em->addEnemy(app->map->MapToWorld(tile_pos.x, tile_pos.y), ENEMY_CRAWLER);
			if (to_add != NULL)
				entity_list.push_back(to_add);
		}
	}
}
