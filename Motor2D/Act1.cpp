#include "Act1.h"
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
#include "Map.h"
#include "Npc.h"

Act1::Act1()
{
}


Act1::~Act1()
{
}

bool Act1::awake(pugi::xml_node& conf)
{
	return true;
}


bool Act1::start()
{
	wallsTexture = app->tex->Load(app->sm->getFileWallsTexture().c_str());

	win = false;

	
	app->audio->PlayMusic("audio/music/town1.ogg");

	if (debug == NULL)
		debug = app->tex->Load("maps/mini_path.png");

	//Map
	if (app->map->Load("new_map_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	SDL_Rect wallsRect{ 0, 0, 7360, 3680 };
	iPoint p, piv;
	p.x = -3520;
	piv.x = 0;
	p.y = 0;
	piv.y = 0;
	walls = new Sprite(wallsTexture, p, piv, wallsRect);
	app->render->addSpriteToList(walls);

	//hollyFire
	hFire.anim.setAnimation(0, 0, 25, 94, 3, 2);
	hFire.active = true;
	hFire.anim.speed = 0.15f;
	hFire.anim.loop = true;
	hFire.anim.pivot.Set(0, 0);
	hFire.life = 0;
	hFire.texture = app->tex->Load("images/hollyFire.png");

	hollyFire = app->particleManager->createParticle(hFire, -2700, 1800, INT_MAX, { 0, 0 }, { 25, 94 }, COLLIDER_PLAYER_PARTICLE, this, true, app->tex->Load("images/hollyFire.png"));
//	app->particleManager->createParticle(rageArround, worldPosition.x, worldPosition.y, rageDuration, { 0, 0 }, { 64, 64 }, COLLIDER_PLAYER_PARTICLE, this, true, particlesAtlas);
	//hollyFire->texture = app->tex->Load("images/LogoAnim.png");
	//hollyFire->anim.setAnimation(0, 0, 25, 94, 3, 2);
	//hollyFire->anim.speed = 0.2f;
	

	app->game->player->setWorldPosition({ -2700, 1800 });
	app->game->player->setStartingWorldPosition({ -2700, 1800 });

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//@@@@@              ENEMIES CREATION                @@@@@
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//############################
	//###       PALADINS       ###
	//############################
	
	//counselor = app->game->em->createNpc({515,2415}, NPC_COUNSELOR);
	//healer = app->game->em->createNpc({ 1440, 2265 }, NPC_HEALER);

	//createProps();
	//createEnemies();
	//app->audio->PlayMusic("audio/music/town1.ogg",0.5f);
	return true;
}

// preUpdate
bool Act1::preUpdate()
{
	return true;
}


// update
bool Act1::update(float dt)
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
	
	if (app->input->getKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		app->sm->changeScene(3);
	}
	if (app->input->getKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		app->sm->changeScene(4);
	}

	if (app->input->getKey(SDL_SCANCODE_L) == KEY_UP)
	{
		app->game->player->attributes->levelUp();
	}

	if (app->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		app->render->camera.y += 10;
		//app->render->renderZone.y += 10;
	}
	if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		app->render->camera.y -= 10;
		//app->render->renderZone.y -= 10;
	}

	if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		app->render->camera.x += 10;
		//app->render->renderZone.x += 10;
	}

	if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		app->render->camera.x -= 10;
		//app->render->renderZone.x -= 10;
	}
	
	
	}

	
	

	return true;
}


// postUpdate
bool Act1::postUpdate()
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
bool Act1::cleanUp()
{
	// remove all entities
	//list<Entity*>::iterator item;
	//item = entity_list.begin();

	//while (item != entity_list.end())
	//{
	//	app->game->em->remove((*item)->getId());
	//	item++;
	//}
	//entity_list.clear();
	//app->game->em->cleanUp();
	//app->audio->cleanUp();
	return true;
}


// Called on certain event
void Act1::OnEvent(GuiElement* element, GUI_Event even)
{

}


//Load/unLoad, called when the scene changes
bool Act1::Load()
{
	start();
	return true;
}

bool Act1::unLoad()
{
	cleanUp();
	app->map->cleanUp();
	app->pathfinding->cleanUp();
	return true;
}
