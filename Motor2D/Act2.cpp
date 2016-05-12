#include "Act2.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Game.h"
//#include "EntityManager.h"
//#include "Entity.h"
#include "Input.h"
#include "SceneManager.h"
#include "snWin.h"
#include "Audio.h"
#include "Player.h"
#include "EntityManager.h"
#include "Act1.h"
#include "Entity.h"

Act2::Act2()
{
}


Act2::~Act2()
{
}

bool Act2::awake(pugi::xml_node& conf)
{
	//propFileName = conf.child("propAtlas").attribute("file").as_string();

	return true;
}


bool Act2::start()
{

	win = false;

	//app->audio->PlayMusic("audio/music/town1.ogg", 0);

	if (debug == NULL)
		debug = app->tex->Load("maps/mini_path.png");

	//Map
	if (app->map->Load("map_act2.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->render->sprites.clear(); //memory leak falta el RELEASE //Maybe ERROR, watch out
	app->render->sprites.push_back(app->game->player->getSprite()); //Maybe ERROR, watch out
	app->render->sprites.push_back(app->sm->act1->counselor->getSprite());
	app->render->sprites.push_back(app->sm->act1->healer->getSprite());
	app->game->player->setWorldPosition({ 0, 1200 });
	app->game->player->setStartingWorldPosition({ 0, 1200 });
	app->sm->act1->counselor->setWorldPosition({ 40, 1200 });
	app->sm->act1->counselor->setColliderPosition({ 16, 1120 });
	app->sm->act1->healer->setWorldPosition({ 150, 1200 });
	app->sm->act1->healer->setColliderPosition({ 130, 1130 });
	app->game->em->createNpc({210, 1200 }, NPC_GOSSIP);

	return true;
}

// preUpdate
bool Act2::preUpdate()
{
	return true;
}


// update
bool Act2::update(float dt)
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
			//EntPortal* portal = (EntPortal*)app->game->em->add(p, PORTAL); //Maybe ERROR, watch out // Descomentar lo de portal per canviar d'escena amb un trigger o algo
			//if (portal)
			//	portal->destiny = app->sm->outdoor1; 

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
			//app->game->em->addEnemy(p, ENEMY_CRAWLER);
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

	if (app->input->getKey(SDL_SCANCODE_L) == KEY_UP)
	{
		app->game->player->attributes->levelUp();
	}
	return true;
}

// postUpdate
bool Act2::postUpdate()
{
	//NOTE: In progress
	//if (app->game->player->TeleportReady())
	//{
	//	Scene* destiny = app->game->player->getDestiny();
	//	app->game->player->ResetTeleport();
	//	app->sm->ChangeScene(destiny);
	//}

	//if (win)
	//{
	//	app->sm->ChangeScene(app->sm->win);
	//}
	return true;
}


// Called before quitting
bool Act2::cleanUp()
{
	// remove all entities
	list<Entity*>::iterator item;
	item = entity_list.begin();

	while (item != entity_list.end())
	{
		//app->game->em->remove(item._Ptr->_Myval->id);
		item++;
	}
	entity_list.clear();
	return true;
}


// Called on certain event
void Act2::OnEvent(GuiElement* element, GUI_Event even)
{

}


//Load/unLoad, called when the scene changes
bool Act2::Load()
{
	start();
	return true;
}

bool Act2::unLoad()
{
	cleanUp();
	app->map->cleanUp();
	app->pathfinding->cleanUp();
	return true;
}