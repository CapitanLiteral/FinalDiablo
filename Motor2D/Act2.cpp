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




	if (debug == NULL)
		debug = app->tex->Load("maps/mini_path.png");
	
	//music act 2
	app->audio->PlayMusic("audio/music/caves.mp3");

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
	//app->render->sprites.push_back(app->sm->act1->counselor->getSprite());
	//app->render->sprites.push_back(app->sm->act1->healer->getSprite());
	app->game->player->setWorldPosition({ 0, 1200 });
	app->game->player->setStartingWorldPosition({ 0, 1200 });
	/*app->sm->act1->counselor->setWorldPosition({ 40, 1200 });
	app->sm->act1->counselor->setColliderPosition({ 16, 1120 });
	app->sm->act1->healer->setWorldPosition({ 150, 1200 });
	app->sm->act1->healer->setColliderPosition({ 130, 1130 });
	app->game->em->createNpc({210, 1200 }, NPC_GOSSIP);*/

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@              ENEMIES CREATION                @@@@@
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//############################
	//###       GRISWOLDS      ###
	//############################

	app->game->em->createGriswold({ -215, 1309 });
	app->game->em->createGriswold({ -363, 1416 });
	app->game->em->createGriswold({ -81, 1584 });

	app->game->em->createGriswold({ 217, 1895 });
	app->game->em->createGriswold({ 576, 2063 });
	app->game->em->createGriswold({ 913, 2246 });
	app->game->em->createGriswold({ 1175, 2369 });
	app->game->em->createGriswold({ 1456, 2239 });

	app->game->em->createGriswold({ 2300, 2516 });
	app->game->em->createGriswold({ 1889, 2706 });
	app->game->em->createGriswold({ 1613, 2840 });
	app->game->em->createGriswold({ 1397, 2969 });

	app->game->em->createGriswold({ 1688, 3230 });
	app->game->em->createGriswold({ 2138, 3537 });
	app->game->em->createGriswold({ 1873, 3640 });
	app->game->em->createGriswold({ 1565, 3768 });
	app->game->em->createGriswold({ 2694, 3533 });
	app->game->em->createGriswold({ 3220, 3542 });
	app->game->em->createGriswold({ 3569, 3490 });
	app->game->em->createGriswold({ 3890, 3321 });
	app->game->em->createGriswold({ 4186, 3141 });
	app->game->em->createGriswold({ 4483, 2995 });
	app->game->em->createGriswold({ 4790, 2773 });
	app->game->em->createGriswold({ 4950, 2854 });
	app->game->em->createGriswold({ 3997, 2890 });
	app->game->em->createGriswold({ 3676, 2714 });
	app->game->em->createGriswold({ 3959, 2392 });
	app->game->em->createGriswold({ 3574, 2261 });

	app->game->em->createGriswold({ 861, 2887 });
	app->game->em->createGriswold({ 517, 2690 });
	app->game->em->createGriswold({ 354, 2863 });
	app->game->em->createGriswold({ 233, 2975 });
	app->game->em->createGriswold({ 78, 3114 });
	app->game->em->createGriswold({ 105, 2843 });
	app->game->em->createGriswold({ 105, 2501 });
	app->game->em->createGriswold({ -332, 2522 });
	app->game->em->createGriswold({ -261, 2813 });
	app->game->em->createGriswold({ -704, 2516 });
	app->game->em->createGriswold({ -340, 2266 });
	app->game->em->createGriswold({ -796, 2343 });
	app->game->em->createGriswold({ -1125, 2270 });
	app->game->em->createGriswold({ -1619, 2000 });
	app->game->em->createGriswold({ -725, 3114 });

	app->game->em->createGriswold({ 240, 4208 });
	app->game->em->createGriswold({ -130, 4288 });
	app->game->em->createGriswold({ -444, 4300 });
	app->game->em->createGriswold({ -680, 4198 });
	app->game->em->createGriswold({ -991, 4042 });
	app->game->em->createGriswold({ -1335, 3862 });
	app->game->em->createGriswold({ -1719, 3575 });
	app->game->em->createGriswold({ -1434, 3412 });

	app->game->em->createGriswold({ -1116, 3197 });
	app->game->em->createGriswold({ -1430, 3101 });
	app->game->em->createGriswold({ -1739, 2964 });
	app->game->em->createGriswold({ -2111, 2797 });
	app->game->em->createGriswold({ -2377, 2677 });
	app->game->em->createGriswold({ -2524, 2508 });
	app->game->em->createGriswold({ -2607, 2792 });
	app->game->em->createGriswold({ -2756, 2652 });
	app->game->em->createGriswold({ -3038, 2849 });

	app->game->em->createGriswold({ -2956, 2166 });
	app->game->em->createGriswold({ -2679, 2001 });
	app->game->em->createGriswold({ -2367, 1868 });
	app->game->em->createGriswold({ -2068, 1743 });
	app->game->em->createGriswold({ -1773, 1936 });
	app->game->em->createGriswold({ -1782, 1540 });


	app->game->em->createBoss({ 2789, 1827});

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