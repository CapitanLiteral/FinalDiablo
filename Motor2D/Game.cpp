#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Hud.h"
#include "EntityManager.h"
#include "Attributes.h"
#include "Gui.h"


Game::Game() : Module()
{
	//Init modules
	player	= new Player();
	em		= new EntityManager();
	hud		= new Hud();

	player->attributes->setHud(hud);
	player->attributes->setReferences(&player->worldPosition.x, &player->worldPosition.y);
	hud->playerAtt = player->attributes;
}

//Destructor
Game::~Game()
{
	RELEASE(hud);
	RELEASE(em);
	RELEASE(player);
}

// Called before render is available
bool Game::awake(pugi::xml_node& conf)
{
	player->awake(conf);
	em->awake(conf);
	hud->awake(conf);
	return true;
}

// Called before the first frame
bool Game::start()
{
	active = true;

	app->gui->clearUI();

	player->start();
	em->start();
	hud->start();

	return true;
}

//preUpdate
bool Game::preUpdate()
{
	player->preUpdate();
	em->preUpdate();
	hud->preUpdate();

	return true;
}

//update
bool Game::update(float dt)
{
	player->attributes->update();

	player->update(dt);
	em->update(dt);
	hud->update(dt);

	player->draw();

	return true;
}

//postUpdate
bool Game::postUpdate()
{
	player->postUpdate();
	em->postUpdate();
	hud->postUpdate();

	return true;
}

// Called before quitting
bool Game::cleanUp()
{
	active = false;

	app->gui->clearUI();

	hud->cleanUp();
	em->cleanUp();
	player->cleanUp();

	return true;
}