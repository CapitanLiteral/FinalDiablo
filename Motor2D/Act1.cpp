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

	//music act 1
	app->audio->PlayMusic("audio/music/town1.ogg");
	
	//fx walls
	hollyFireFx = app->audio->LoadFx("audio/fx/infernoloop.wav");
	
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
	
	createHollyFire();

	app->game->player->setWorldPosition({ -2700, 1800 });
	app->game->player->setStartingWorldPosition({ -2700, 1800 });
	
	counselor = app->game->em->createNpc({-2480,1800}, NPC_COUNSELOR);
	//healer = app->game->em->createNpc({ 1440, 2265 }, NPC_HEALER);
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
{/*
	remove all entities
	list<Entity*>::iterator item;
	item = entity_list.begin();

	while (item != entity_list.end())
	{
		app->game->em->remove((*item)->getId());
		item++;
	}
	entity_list.clear();
	app->game->em->cleanUp();*/
	
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

void Act1::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_HOLLYFIRE || c1->type == COLLIDER_HOLLYFIRE && c2->type == COLLIDER_PLAYER)
	{
		if (c1->type == COLLIDER_PLAYER)
		{	
			float dmg = app->game->player->attributes->getMaxLife()*0.6*app->getDT();
			app->game->player->attributes->addLife(-dmg);
			//fx sound
			//app->audio->PlayFx(hollyFireFx, 0);

		}
		else if (c2->type == COLLIDER_PLAYER)
		{
			float dmg = app->game->player->attributes->getMaxLife()*0.6*app->getDT();
			app->game->player->attributes->addLife(-dmg);
			//fx sound
			//app->audio->PlayFx(hollyFireFx, 0);
		}
	}

}

void Act1::createHollyFire()
{
	//ROAD 1
	//hollyFire
	hFire.anim.setAnimation(0, 0, 25, 94, 3, 2);
	hFire.active = true;
	hFire.anim.speed = 0.15f;
	hFire.anim.loop = true;
	hFire.anim.pivot.Set(0, 0);
	hFire.life = 0;
	hFire.texture = app->tex->Load("images/hollyFire.png");

	hollyFire = app->particleManager->createParticle(hFire, -2300, 1900, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire1 = app->particleManager->createParticle(hFire, -2250, 1930, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire2 = app->particleManager->createParticle(hFire, -2200, 1960, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	
	//ROAD 2
	hollyFire3 = app->particleManager->createParticle(hFire, -1460, 1900, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire4 = app->particleManager->createParticle(hFire, -1410, 1880, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire5 = app->particleManager->createParticle(hFire, -1360, 1860, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire6 = app->particleManager->createParticle(hFire, -1310, 1840, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire7 = app->particleManager->createParticle(hFire, -1260, 1820, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire8 = app->particleManager->createParticle(hFire, -1210, 1800, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	
	//ROAD 3
	hollyFire9 = app->particleManager->createParticle(hFire, -710, 1800, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire10 = app->particleManager->createParticle(hFire, -660, 1820, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire11 = app->particleManager->createParticle(hFire, -610, 1840, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire12 = app->particleManager->createParticle(hFire, -560, 1860, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire13 = app->particleManager->createParticle(hFire, -510, 1880, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire14 = app->particleManager->createParticle(hFire, -460, 1900, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire15 = app->particleManager->createParticle(hFire, -410, 1920, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	
	//ROAD4
	hollyFire16 = app->particleManager->createParticle(hFire, -010, 1980, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire17 = app->particleManager->createParticle(hFire, 40, 1960, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire18 = app->particleManager->createParticle(hFire, 90, 1940, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire19 = app->particleManager->createParticle(hFire, 140, 1920, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire20 = app->particleManager->createParticle(hFire, 190, 1900, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire21 = app->particleManager->createParticle(hFire, 240, 1880, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire22 = app->particleManager->createParticle(hFire, 290, 1860, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire23 = app->particleManager->createParticle(hFire, 340, 1840, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	
	//ROAD5
	hollyFire24 = app->particleManager->createParticle(hFire, 740, 1540, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire25 = app->particleManager->createParticle(hFire, 790, 1520, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire26 = app->particleManager->createParticle(hFire, 840, 1500, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire27 = app->particleManager->createParticle(hFire, 890, 1480, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire28 = app->particleManager->createParticle(hFire, 940, 1460, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire29 = app->particleManager->createParticle(hFire, 990, 1440, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire30 = app->particleManager->createParticle(hFire, 1040, 1420, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	
	//ROAD6
	hollyFire31 = app->particleManager->createParticle(hFire, 1790, 1400, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire32 = app->particleManager->createParticle(hFire, 1840, 1420, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire33 = app->particleManager->createParticle(hFire, 1890, 1440, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire34 = app->particleManager->createParticle(hFire, 1940, 1460, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire35 = app->particleManager->createParticle(hFire, 1990, 1480, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));
	hollyFire36 = app->particleManager->createParticle(hFire, 2040, 1500, INT_MAX, { 0, -34 }, { 25, 50 }, COLLIDER_HOLLYFIRE, this, true, app->tex->Load("images/hollyFire.png"));

}