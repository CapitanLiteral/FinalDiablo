#include "app.h"
#include "EntityManager.h"
#include "Input.h"
#include "EntEnemy.h"
#include "Pathfinding.h"
#include "p2Log.h"
#include "Map.h"

#include "Render.h"
#include "EntItem.h"
#include "EntPortal.h"
#include "EntEnemy.h"

#include "Textures.h"
#include "Collision.h"

#include "Gui.h"
//Provisional
#include "Fonts.h"
#include "Audio.h"

#include <algorithm>


EntityManager::EntityManager() : Module()
{
	name.create("EntityManager");
}

// Destructor
EntityManager::~EntityManager()
{
	nextID = 0;
}

// Called before render is available
bool EntityManager::awake(pugi::xml_node &node)
{
	return true;
}

// Called before the first frame
bool EntityManager::start()
{
	//Crawler
	crawler_idle = app->tex->Load("textures/crawler_idle.png");
	crawler_walk = app->tex->Load("textures/crawler_walk.png");
	crawler_death = app->tex->Load("textures/crawler_death.png");
	crawler_attack = app->tex->Load("textures/crawler_attack.png");

	//Boss
	boss_idle = app->tex->Load("textures/boss_idle.png");
	boss_walk = app->tex->Load("textures/boss_walk.png");
	boss_death = app->tex->Load("textures/boss_death.png");
	boss_attack = app->tex->Load("textures/boss_attack.png");

	//Portal
	portal_tex = app->tex->Load("textures/portal.png");
	
	enemy_name = app->gui->addGuiLabel(" ", NULL, { 260, 0 }, NULL, FONT_WHITE, this);
	enemy_name->Desactivate();

	//Sounds
	crawler_attackfx = app->audio->LoadFx("audio/fx/VileChildAttack.ogg");
	crawler_gethitfx = app->audio->LoadFx("audio/fx/VileChildgetHit.ogg");
	crawler_deathfx = app->audio->LoadFx("audio/fx/VileChildDeath.ogg");

	wolf_attackfx = app->audio->LoadFx("audio/fx/WolfAttack.ogg");
	wolf_gethitfx = app->audio->LoadFx("audio/fx/WolfgetHit.ogg");
	wolf_deathfx = app->audio->LoadFx("audio/fx/WolfDeath.ogg");

	return true;
}

// Called each loop iteration
bool EntityManager::preUpdate()
{
	// Clicking middle button, eliminates an entity
	if (app->input->getMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
	{
		iPoint pos; 
		pos = app->input->getMouseWorldPosition();
		const Entity* entity = entityOnMouse();

		if (entity != NULL)
		{
			remove(entity->id);
		}
	}


	return true;
}

//update
bool EntityManager::update(float dt)
{
	map<uint, Entity*>::iterator item = activeEntities.begin();
	for (; item != activeEntities.end(); ++item)
	{
		SDL_Rect cam = app->render->camera;
		fPoint pos = item->second->position;
		if (pos.x + 100 > -cam.x && pos.x - 100 < -cam.x + cam.w &&
			pos.y + 100 > -cam.y && pos.y - 100 < -cam.y + cam.h)
		{
			item->second->update(dt);
		}
	}

	// Entities drawing
	item = activeEntities.begin();
	for (; item != activeEntities.end(); ++item)
	{
		item->second->draw();
		if (app->debug)
		{
			item->second->drawDebug();
		}

	}

	if (Entity* ent = entityOnMouse())
	{
		if (ent->type == ENEMY)
		{
			((EntEnemy*)ent)->DrawHPbar();
			enemy_name->Activate();
			enemy_name->SetText(((EntEnemy*)ent)->name);
			enemy_name->Center(true, false);
		}
	}
	else
		enemy_name->Desactivate();

	return true;
}

// Called each loop iteration
bool EntityManager::postUpdate()
{

	//Checking if there's an entity under the mouse to do it's stuff
	// NOTE: put it as gui


	return true;
}

// Called before quitting
bool EntityManager::cleanUp()
{
	map<uint, Entity*>::iterator item = activeEntities.begin();
	for (; item != activeEntities.end(); item++)
		delete item->second;

	item = inactiveEntities.begin();
	for (; item != inactiveEntities.end(); item++)
		delete item->second;

	activeEntities.clear();
	inactiveEntities.clear();

	if (enemy_name)
		enemy_name->Desactivate();

	return true;
}

// add method

Entity* EntityManager::add(iPoint &pos, ENTITY_TYPE type)
{
	Entity* entity = NULL;
	iPoint tile_pos = app->map->WorldToMap(pos.x, pos.y);

	// Checking for another bricks already on the map_tile specified by argument pos.
	map<uint, Entity*>::iterator item = activeEntities.begin();

	for (; item != activeEntities.end(); item++)
	{
		if (entityOnCoords(pos) != NULL)
			return entity; // No entity is created!
	}

	if (app->pathfinding->IsWalkable(tile_pos))	// Can we add a new entity on that tile? i.e. Is that tile walkable?
	{
		switch (type)
		{
		//NOTE: to diferentiate the kinds of enemies, put ENEMY_TYPE enum, but don't use the one from the diferent kinds of entities
		case (ENEMY) :
			entity = new EntEnemyWolf(pos, ++nextID);
			break;
		case (ITEM_HEALTH) :
			entity = new itmPotionHP(pos, ++nextID);
			break;
		case(PORTAL) :
			entity = new EntPortal(pos, ++nextID);
		}

		// We add the new entity to the map of active entities. 
		activeEntities.insert(pair<uint, Entity*>(nextID, entity));
	}

	return entity;
}

Entity* EntityManager::addEnemy(iPoint &pos, ENEMY_TYPE type)
{
	Entity* entity = NULL;
	iPoint tile_pos = app->map->WorldToMap(pos.x, pos.y);

	// Checking for another bricks already on the map_tile specified by argument pos.
	map<uint, Entity*>::iterator item = activeEntities.begin();

	for (; item != activeEntities.end(); item++)
	{
		if (entityOnCoords(pos) != NULL)
			return entity; // No entity is created!
	}

	if (app->pathfinding->IsWalkable(tile_pos))	// Can we add a new entity on that tile? i.e. Is that tile walkable?
	{
		switch (type)
		{
			//NOTE: to diferentiate the kinds of enemies, put ENEMY_TYPE enum, but don't use the one from the diferent kinds of entities
		case (ENEMY_WOLF) :
			entity = new EntEnemyWolf(pos, ++nextID);
			break;

		case (ENEMY_CRAWLER) :
			entity = new EntEnemyCrawler(pos, ++nextID);
			break;

		case (ENEMY_COUNCIL) :
			entity = new EntEnemyCouncil(pos, ++nextID);
			break;
		}

		// We add the new entity to the map of active entities. 
		activeEntities.insert(pair<uint, Entity*>(nextID, entity));
	}

	return entity;
}

// remove an entity using its ID
bool EntityManager::remove(uint id)
{
	//NOTE: has to delete, not do this!
	//It's not destroyed? we'll see for later weeks
	Entity* e = getEntity(id);
	if (e && e->collider)
		e->collider->to_delete = true;

	if (e && e->sprite)
	{
		//NOTE: here, is a beautiful memory leak, if i uncomment this all the game goes fucked

		//RELEASE(e->sprite);
		app->render->sprites.remove(e->sprite);
		//RELEASE(e->sprite);
	}

	if (activeEntities.erase(id) > 0)
	{
		Entity* e = getEntity(id);
		//NOTE: has to delete, not do this!
		//It's not destroyed? we'll see for later weeks
		inactiveEntities.insert(pair<uint, Entity*>(id, e));

		return true;
	}
	return false;
}

// Return ID for the corresponding entity
Entity* EntityManager::getEntity(uint id)
{
	map<uint, Entity*>::iterator item = activeEntities.find(id);
	return (item != activeEntities.end() ? item->second : NULL);
}

// WhichEntityOnMouse: Returns an entity under the mouse cursor
Entity* EntityManager::entityOnMouse()
{
	iPoint p = app->input->getMouseWorldPosition();

	return entityOnCoords(p);
}

Entity* EntityManager::entityOnCoords(iPoint &pos)
{
	map<uint, Entity*>::reverse_iterator item = activeEntities.rbegin();
	for (; item != activeEntities.rend(); ++item)
	{
		//NOTE: Have to be specified to just Enemy....
		if (item->second->type == ENEMY)
			if (((EntEnemy*)item->second)->current_action == ENTITY_DEATH)
				continue;

			SDL_Rect rect = item->second->getPlayerRect();

		if (pos.x >= rect.x && pos.x <= rect.x + rect.w &&
			pos.y >= rect.y && pos.y <= rect.y + rect.h)
			return item->second;
	}
	return NULL;
}


