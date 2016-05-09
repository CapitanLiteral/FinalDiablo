#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Hud.h"
#include "EntityManager.h"


Game::Game() : Module()
{
	//Init modules
	player	= new Player();
	em		= new EntityManager();
	hud		= new Hud();

	//add modules to the list
	addModule(player);
	addModule(em);
	addModule(hud);
}

//Destructor
Game::~Game()
{
	list<Module*>::reverse_iterator item = game_modules.rbegin();

	while (item != game_modules.rend())
	{
		RELEASE(*item);
		++item;
	}

	game_modules.clear();
}

// Called before render is available
bool Game::awake(pugi::xml_node& conf)
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		(*item)->awake(conf);
	}
	return true;
}

// Called before the first frame
bool Game::start()
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		(*item)->start();
	}

	return true;
}

//preUpdate
bool Game::preUpdate()
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		(*item)->preUpdate();
	}

	return true;
}

//update
bool Game::update(float dt)
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		if (!pause)
			(*item)->update(dt);

		(*item)->draw();
	}


	return true;
}

//postUpdate
bool Game::postUpdate()
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		(*item)->postUpdate();
	}

	return true;
}

// Called before quitting
bool Game::cleanUp()
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		(*item)->cleanUp();
	}

	return true;
}

void Game::draw()
{
	list<Module*>::iterator item = game_modules.begin();

	for (; item != game_modules.end(); item++)
	{
		(*item)->draw();
	}
}

void Game::addModule(Module* module)
{
	module->Init();
	game_modules.push_back(module);
}