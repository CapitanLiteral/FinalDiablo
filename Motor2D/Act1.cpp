#include "Act1.h"
#include "Act2.h"
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
#include "Player.h"
#include "EntPortal.h"
#include "EntEnemy.h"
#include "Map.h"

Act1::Act1()
{
}


Act1::~Act1()
{
}

bool Act1::awake(pugi::xml_node& conf)
{
	//propFileName = conf.child("propAtlas").attribute("file").as_string();

	return true;
}


bool Act1::start()
{
	propFileName = app->sm->getFilePropsVillage();
	propAtlas = app->tex->Load(propFileName.c_str());
	win = false;

	//app->audio->PlayMusic("audio/music/town1.ogg", 0);

	if (debug == NULL)
		debug = app->tex->Load("maps/mini_path.png");

	//Map
	if (app->map->Load("map_act1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (app->map->CreateWalkabilityMap(w, h, &data))
			app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	app->game->player->SetPosition({ 450, 2400 });

	createProps();

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
			EntPortal* portal = (EntPortal*)app->game->em->add(p, PORTAL); //Maybe ERROR, watch out // Descomentar lo de portal per canviar d'escena amb un trigger o algo
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

	if (app->input->getKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		app->sm->fadeToBlack(app->sm->act2);
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
	

	return true;
}

// postUpdate
bool Act1::postUpdate()
{
	//NOTE: In progress
	if (app->game->player->TeleportReady())
	{
		Scene* destiny = app->game->player->getDestiny();
		app->game->player->ResetTeleport();
		app->sm->ChangeScene(destiny);
	}

	if (win)
	{
		app->sm->ChangeScene(app->sm->win);
	}
	return true;
}


// Called before quitting
bool Act1::cleanUp()
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

void Act1::createProps(){
	SDL_Rect propRect{ 3073, 1497, 215, 202 };
	iPoint p;
	iPoint piv;
	p.x =piv.x = 0;
	p.y = piv.y = 0;
	propProva = new Sprite(propAtlas, p, piv, propRect);
	
	propRect = { 1236, 872, 105, 58 };
	p.x = 595; piv.x = 105;
	p.y = 2468; piv.y = 58;
	fire = new Sprite(propAtlas, p, piv, propRect);
	fire->layer = SCENE;
	app->render->addSpriteToList(fire);
	
	propRect = { 1805, 793, 356, 256 };
	p.x = 2146; piv.x = 356;
	p.y = 2506; piv.y = 156;
	woodHouse = new Sprite(propAtlas, p, piv, propRect);
	woodHouse->layer = SCENE;
	app->render->addSpriteToList(woodHouse);
	
	propRect = { 2374, 838, 202, 109 };
	p.x = 2159; piv.x = 109;
	p.y = 2590; piv.y = 20;
	wood = new Sprite(propAtlas, p, piv, propRect);
	wood->layer = SCENE;
	app->render->addSpriteToList(wood);
	
	propRect = { 4644, 87, 140, 212 };
	p.x = 1920; piv.x = 140;
	p.y = 2832; piv.y = 212;
	woodUp = new Sprite(propAtlas, p, piv, propRect);
	woodUp->layer = SCENE;
	app->render->addSpriteToList(woodUp);
	
	propRect = { 1140, 391, 522, 315 };
	p.x = 122; piv.x = 522;
	p.y = 1615; piv.y = 315;
	carriage = new Sprite(propAtlas, p, piv, propRect);
	carriage->layer = SCENE;
	app->render->addSpriteToList(carriage);

	propRect = { 4569, 855, 106, 82 };
	p.x = 86; piv.x = 106;
	p.y = 1532; piv.y = 82;
	stuff = new Sprite(propAtlas, p, piv, propRect);
	stuff->layer = SCENE;
	app->render->addSpriteToList(stuff);

	propRect = { 33, 459, 288, 214 };
	p.x = -512; piv.x = 288;
	p.y = 1964; piv.y = 214;
	carriage1 = new Sprite(propAtlas, p, piv, propRect);
	carriage1->layer = SCENE;
	app->render->addSpriteToList(carriage1);

	propRect = { 656, 864, 112, 77 };
	p.x = -388; piv.x = 112;
	p.y = 2027; piv.y = 77;
	treeCut = new Sprite(propAtlas, p, piv, propRect);
	treeCut->layer = SCENE;
	app->render->addSpriteToList(treeCut);

	propRect = { 56, 691, 425, 289 };
	p.x = -1175; piv.x = 425;
	p.y = 2319; piv.y = 289;
	tent = new Sprite(propAtlas, p, piv, propRect);
	tent->layer = SCENE;
	app->render->addSpriteToList(tent);

	propRect = { 2374, 838, 202, 109 };
	p.x = -1378; piv.x = 202;
	p.y = 2479; piv.y = 109;
	wood1 = new Sprite(propAtlas, p, piv, propRect);
	wood1->layer = SCENE;
	app->render->addSpriteToList(wood1);

	propRect = { 2374, 838, 202, 109 };
	p.x = -1378; piv.x = 202;
	p.y = 2509; piv.y = 109;
	wood2 = new Sprite(propAtlas, p, piv, propRect);
	wood2->layer = SCENE;
	app->render->addSpriteToList(wood2);

	propRect = { 2374, 838, 202, 109 };
	p.x = -1378; piv.x = 202;
	p.y = 2579; piv.y = 109;
	wood3 = new Sprite(propAtlas, p, piv, propRect);
	wood3->layer = SCENE;
	app->render->addSpriteToList(wood3);

	propRect = { 4644, 87, 140, 212 };
	p.x = -960; piv.x = 140;
	p.y = 2612; piv.y = 212;
	woodUp1 = new Sprite(propAtlas, p, piv, propRect);
	woodUp1->layer = SCENE;
	app->render->addSpriteToList(woodUp1);

	propRect = { 4724, 770, 264, 168 };
	p.x = 1744; piv.x = 264;
	p.y = 2168; piv.y = 168;
	tent1 = new Sprite(propAtlas, p, piv, propRect);
	tent1->layer = SCENE;
	app->render->addSpriteToList(tent1);

	propRect = { 2339, 417, 490, 248 };
	p.x = 1290; piv.x = 490;
	p.y = 2008; piv.y = 248;
	carriage2 = new Sprite(propAtlas, p, piv, propRect);
	carriage2->layer = SCENE;
	app->render->addSpriteToList(carriage2);

	propRect = { 598, 458, 360, 238 };
	p.x = 830; piv.x = 360;
	p.y = 3488; piv.y = 238;
	carriage3 = new Sprite(propAtlas, p, piv, propRect);
	carriage3->layer = SCENE;
	app->render->addSpriteToList(carriage3);

	propRect = { 1762, 585, 151, 124 };
	p.x = -169; piv.x = 151;
	p.y = 2934; piv.y = 124;
	carriage4 = new Sprite(propAtlas, p, piv, propRect);
	carriage4->layer = SCENE;
	app->render->addSpriteToList(carriage4);
}