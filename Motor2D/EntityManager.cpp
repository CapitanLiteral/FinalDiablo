#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Npc.h"
#include "Paladin.h"
#include "Wolf.h"
#include "Griswold.h"
#include "FileSystem.h"
#include "Attributes.h"

#include "App.h"
#include "Render.h"
//#include "PathFinding.h"
#include "p2Log.h"
#include "Textures.h"
#include "Input.h"
#include "Collision.h"

EntityManager::EntityManager() : Module()
{
	LOG("EntityManager: Creating.");
	name.create("EntityManager");
}

EntityManager::~EntityManager()
{
	LOG("EntityManager: Destroying.");
	activeEntities.clear();
	inactiveEntities.clear();
}

bool EntityManager::awake(pugi::xml_node& config)
{
	LOG("EntityManager: Awake.");
	bool ret = true;

	return ret;
}

bool EntityManager::start()
{
	LOG("EntityManager: Start.");
	bool ret = true;

	doLogic = false;
	updateMsCycle = 1 / 50;
	acummulatedTime = 0.0f;

	ret = loadEnemiesAnimations();
	ret = loadNpcAnimations();

	paladinTexture = app->tex->Load("images/Paladin.png");
	wolfTexture = app->tex->Load("images/Wolf.png");
	griswoldTexture = app->tex->Load("images/Griswold.png");
	counselorTexture = app->tex->Load("images/aidanIdle.png");
	healerTexture = app->tex->Load("images/atmaIdle.png");
	gossipTexture = app->tex->Load("images/alkorIdle.png");


	return ret;
}

bool EntityManager::preUpdate()
{
	bool ret = true;

	if (app->debug)
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
		{
			iPoint point = app->input->getMouseWorldPosition();

			createPaladin(point);
		}
	}

	return ret;
}

bool EntityManager::update(float dt)
{
	bool ret = true;

	//TODO: limitador de fps logics
	acummulatedTime += dt;
	if (acummulatedTime >= updateMsCycle)
		doLogic = true;

	std::map<uint, Entity*>::iterator tmp = activeEntities.begin();

	for (; tmp != activeEntities.end(); ++tmp)
	{
		if (doLogic /*&& app->render->isInsideCameraZone(tmp->second->getCollider()->rect)*/)
		{
			tmp->second->entityUpdate(dt);
			acummulatedTime = 0.0f;
		}
	}

	return ret;
}

bool EntityManager::postUpdate()
{
	bool ret = true;

	std::map<uint, Entity*>::iterator tmp = activeEntities.begin();

	for (; tmp != activeEntities.end(); ++tmp)
		tmp->second->entityPostUpdate();

	return ret;
}

bool EntityManager::cleanUp()
{
	LOG("EntityManager: CleanUp.");
	bool ret = true;

	std::map<uint, Entity*>::iterator tmp = activeEntities.begin();

	for (; tmp != activeEntities.end(); ++tmp)
		RELEASE(tmp->second);

	for (tmp = inactiveEntities.begin(); tmp != inactiveEntities.end(); ++tmp)
		RELEASE(tmp->second);

	activeEntities.clear();
	inactiveEntities.clear();

	return ret;
}

bool EntityManager::loadEnemiesAnimations()
{
	bool ret = true;

	//############################
	//###     EnemyPaladin     ###
	//############################

	pugi::xml_document	anim_file;
	pugi::xml_node		anim;
	char* buff;
	int size = app->fs->Load("animations/paladin_animations.xml", &buff);
	pugi::xml_parse_result result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations").child("PALADIN");

	if (ret == true)
	{
		for (pugi::xml_node action = anim.child("IDLE"); action != NULL; action = action.next_sibling())
		{
			for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
			{
				std::pair<entityState, entityDirection> p;
				int state = action.child("name").attribute("value").as_int();
				p.first = (entityState)state;

				int di = dir.first_child().attribute("name").as_int();
				p.second = (entityDirection)di;

				Animation anims;
				int x = dir.first_child().attribute("x").as_int();
				int y = dir.first_child().attribute("y").as_int();
				int w = dir.first_child().attribute("w").as_int();
				int h = dir.first_child().attribute("h").as_int();
				int fN = dir.first_child().attribute("frameNumber").as_int();
				int margin = dir.first_child().attribute("margin").as_int();
				bool loop = action.child("loop").attribute("value").as_bool();
				int pivotX = dir.first_child().attribute("pivot_x").as_int();
				int pivotY = dir.first_child().attribute("pivot_y").as_int();
				float animSpeed = action.child("speed").attribute("value").as_float();
				anims.setAnimation(x, y, w, h, fN, margin);
				anims.loop = loop;
				anims.speed = animSpeed;
				anims.pivot.x = pivotX;
				anims.pivot.y = pivotY;

				iPoint piv;
				paladinAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
				paladinAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
				piv = paladinAnim.find({ p.first, p.second })->second.pivot;

			}
		}
	}

	//############################
	//###      EnemyWolf       ###
	//############################

	size = app->fs->Load("animations/wolf_animations.xml", &buff);
	result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations").child("WOLF");

	if (ret == true)
	{
		for (pugi::xml_node action = anim.child("IDLE"); action != NULL; action = action.next_sibling())
		{
			for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
			{
				std::pair<entityState, entityDirection> p;
				int state = action.child("name").attribute("value").as_int();
				p.first = (entityState)state;

				int di = dir.first_child().attribute("name").as_int();
				p.second = (entityDirection)di;

				Animation anims;
				int x = dir.first_child().attribute("x").as_int();
				int y = dir.first_child().attribute("y").as_int();
				int w = dir.first_child().attribute("w").as_int();
				int h = dir.first_child().attribute("h").as_int();
				int fN = dir.first_child().attribute("frameNumber").as_int();
				int margin = dir.first_child().attribute("margin").as_int();
				bool loop = action.child("loop").attribute("value").as_bool();
				int pivotX = dir.first_child().attribute("pivot_x").as_int();
				int pivotY = dir.first_child().attribute("pivot_y").as_int();
				float animSpeed = action.child("speed").attribute("value").as_float();
				anims.setAnimation(x, y, w, h, fN, margin);
				anims.loop = loop;
				anims.speed = animSpeed;
				anims.pivot.x = pivotX;
				anims.pivot.y = pivotY;

				iPoint piv;
				wolfAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
				wolfAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
				piv = wolfAnim.find({ p.first, p.second })->second.pivot;

			}
		}
	}

	//############################
	//###    EnemyGriswold     ###
	//############################

	size = app->fs->Load("animations/griswold_animations.xml", &buff);
	result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations").child("GRISWOLD");

	if (ret == true)
	{
		for (pugi::xml_node action = anim.child("IDLE"); action != NULL; action = action.next_sibling())
		{
			for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
			{
				std::pair<entityState, entityDirection> p;
				int state = action.child("name").attribute("value").as_int();
				p.first = (entityState)state;

				int di = dir.first_child().attribute("name").as_int();
				p.second = (entityDirection)di;

				Animation anims;
				int x = dir.first_child().attribute("x").as_int();
				int y = dir.first_child().attribute("y").as_int();
				int w = dir.first_child().attribute("w").as_int();
				int h = dir.first_child().attribute("h").as_int();
				int fN = dir.first_child().attribute("frameNumber").as_int();
				int margin = dir.first_child().attribute("margin").as_int();
				bool loop = action.child("loop").attribute("value").as_bool();
				int pivotX = dir.first_child().attribute("pivot_x").as_int();
				int pivotY = dir.first_child().attribute("pivot_y").as_int();
				float animSpeed = action.child("speed").attribute("value").as_float();
				anims.setAnimation(x, y, w, h, fN, margin);
				anims.loop = loop;
				anims.speed = animSpeed;
				anims.pivot.x = pivotX;
				anims.pivot.y = pivotY;

				iPoint piv;
				griswoldAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
				griswoldAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
				piv = griswoldAnim.find({ p.first, p.second })->second.pivot;

			}
		}
	}

	return ret;
}

bool EntityManager::loadNpcAnimations()
{
	bool ret = true;
	//############################
	//###     NpcCounselor     ###
	//############################

	pugi::xml_document	anim_file;
	pugi::xml_node		anim;
	char* buff;
	int size = app->fs->Load("animations/counselor_animations.xml", &buff);
	pugi::xml_parse_result result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations");

	if (ret == true)
	{
		for (pugi::xml_node action = anim.child("IDLE"); action != NULL; action = action.next_sibling())
		{
			for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
			{
				std::pair<entityState, entityDirection> p;
				int state = action.child("name").attribute("value").as_int();
				p.first = (entityState)state;

				int di = dir.first_child().attribute("name").as_int();
				p.second = (entityDirection)di;

				Animation anims;
				int x = dir.first_child().attribute("x").as_int();
				int y = dir.first_child().attribute("y").as_int();
				int w = dir.first_child().attribute("w").as_int();
				int h = dir.first_child().attribute("h").as_int();
				int fN = dir.first_child().attribute("frameNumber").as_int();
				int margin = dir.first_child().attribute("margin").as_int();
				bool loop = action.child("loop").attribute("value").as_bool();
				int pivotX = dir.first_child().attribute("pivot_x").as_int();
				int pivotY = dir.first_child().attribute("pivot_y").as_int();
				float animSpeed = action.child("speed").attribute("value").as_float();
				anims.setAnimation(x, y, w, h, fN, margin);
				anims.loop = loop;
				anims.speed = animSpeed;
				anims.pivot.x = pivotX;
				anims.pivot.y = pivotY;

				iPoint piv;
				counselorAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
				counselorAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
				piv = counselorAnim.find({ p.first, p.second })->second.pivot;

			}
		}
	}

	//############################
	//###     NpcHealer        ###
	//############################

	size = app->fs->Load("animations/healer_animations.xml", &buff);
	result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations");

	if (ret == true)
	{
		for (pugi::xml_node action = anim.child("IDLE"); action != NULL; action = action.next_sibling())
		{
			for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
			{
				std::pair<entityState, entityDirection> p;
				int state = action.child("name").attribute("value").as_int();
				p.first = (entityState)state;

				int di = dir.first_child().attribute("name").as_int();
				p.second = (entityDirection)di;

				Animation anims;
				int x = dir.first_child().attribute("x").as_int();
				int y = dir.first_child().attribute("y").as_int();
				int w = dir.first_child().attribute("w").as_int();
				int h = dir.first_child().attribute("h").as_int();
				int fN = dir.first_child().attribute("frameNumber").as_int();
				int margin = dir.first_child().attribute("margin").as_int();
				bool loop = action.child("loop").attribute("value").as_bool();
				int pivotX = dir.first_child().attribute("pivot_x").as_int();
				int pivotY = dir.first_child().attribute("pivot_y").as_int();
				float animSpeed = action.child("speed").attribute("value").as_float();
				anims.setAnimation(x, y, w, h, fN, margin);
				anims.loop = loop;
				anims.speed = animSpeed;
				anims.pivot.x = pivotX;
				anims.pivot.y = pivotY;

				iPoint piv;
				healerAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
				healerAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
				piv = healerAnim.find({ p.first, p.second })->second.pivot;

			}
		}
	}

	//############################
	//###     NpcGossip        ###
	//############################

	size = app->fs->Load("animations/gossip_animations.xml", &buff);
	result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations");

	if (ret == true)
	{
		for (pugi::xml_node action = anim.child("IDLE"); action != NULL; action = action.next_sibling())
		{
			for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
			{
				std::pair<entityState, entityDirection> p;
				int state = action.child("name").attribute("value").as_int();
				p.first = (entityState)state;

				int di = dir.first_child().attribute("name").as_int();
				p.second = (entityDirection)di;

				Animation anims;
				int x = dir.first_child().attribute("x").as_int();
				int y = dir.first_child().attribute("y").as_int();
				int w = dir.first_child().attribute("w").as_int();
				int h = dir.first_child().attribute("h").as_int();
				int fN = dir.first_child().attribute("frameNumber").as_int();
				int margin = dir.first_child().attribute("margin").as_int();
				bool loop = action.child("loop").attribute("value").as_bool();
				int pivotX = dir.first_child().attribute("pivot_x").as_int();
				int pivotY = dir.first_child().attribute("pivot_y").as_int();
				float animSpeed = action.child("speed").attribute("value").as_float();
				anims.setAnimation(x, y, w, h, fN, margin);
				anims.loop = loop;
				anims.speed = animSpeed;
				anims.pivot.x = pivotX;
				anims.pivot.y = pivotY;

				iPoint piv;
				gossipAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
				gossipAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
				piv = gossipAnim.find({ p.first, p.second })->second.pivot;

			}
		}
	}
	return ret;
}
bool EntityManager::remove(uint id)
{
	bool ret = true;

	if (activeEntities.erase(id) > 0)
	{
		Entity* tmp = getEntity(id);
		inactiveEntities.insert(std::pair<uint, Entity*>(id, tmp));
	}
	else
		ret = false;

	return ret;
}

Entity* EntityManager::getEntity(uint id)
{
	Entity* ret = NULL;

	std::map<uint, Entity*>::iterator tmp = activeEntities.find(id);

	return (tmp != activeEntities.end() ? tmp->second : NULL);

	return ret;
}

void EntityManager::sortEntities()
{

}

Paladin* EntityManager::createPaladin(iPoint pos)
{
	Paladin* ret = NULL;

	ret = new Paladin(pos);

	ret->setId(nextId);

	activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));

	++nextId;

	return ret;
}

Wolf* EntityManager::createWolf(iPoint pos)
{
	Wolf* ret = NULL;

	ret = new Wolf(pos);

	ret->setWorldPosition(pos);
	ret->setId(nextId);

	activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));

	++nextId;

	return ret;
}

Griswold* EntityManager::createGriswold(iPoint pos)
{
	Griswold* ret = NULL;

	ret = new Griswold(pos);

	ret->setWorldPosition(pos);
	ret->setId(nextId);

	activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));

	++nextId;

	return ret;
}

Entity* EntityManager::createNpc(iPoint position, entityType type)
{
	switch (type)
	{
	case (NPC_COUNSELOR):
		{
			 NpcCounselor* ret = new NpcCounselor(position, nextId);
			 activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));
			 nextId++;
			 return ret;
			break;
		}
	case (NPC_HEALER) :
	{
		NpcHealer* ret = new NpcHealer(position, nextId);
		activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));
		nextId++;
		return ret;
		break;
	}
	case (NPC_GOSSIP) :
	{
		NpcGossip* ret = new NpcGossip(position, nextId);
		activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));
		nextId++;
		return ret;
		break;
	}
	}
	return NULL;
}

SDL_Texture* EntityManager::getPaladinTexture()
{
	return paladinTexture;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getPaladinAnimation()
{
	return &paladinAnim;
}

AttributeBuilder* EntityManager::getPaladinAttributBuilder()
{
	return &paladinAttributeBuilder;
}

SDL_Texture* EntityManager::getWolfTexture()
{
	return wolfTexture;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getWolfAnimation()
{
	return &wolfAnim;
}

AttributeBuilder* EntityManager::getWolfAttributBuilder()
{
	return &wolfAttributeBuilder;
}

SDL_Texture* EntityManager::getGriswoldTexture()
{
	return griswoldTexture;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getGriswoldAnimation()
{
	return &griswoldAnim;
}

AttributeBuilder* EntityManager::getGriswoldAttributBuilder()
{
	return &griswoldAttributeBuilder;
}

SDL_Texture* EntityManager::getCounselorTexture()
{
	return counselorTexture;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getCounselorAnimation()
{
	return &counselorAnim;
}

SDL_Texture* EntityManager::getHealerTexture()
{
	return healerTexture;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getHealerAnimation()
{
	return &healerAnim;
}

SDL_Texture* EntityManager::getGossipTexture()
{
	return gossipTexture;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getGossipAnimation()
{
	return &gossipAnim;
}

uint EntityManager::getEntityAtPositionId(iPoint position)
{
	uint ret = NULL;

	iPoint pos = app->render->ScreenToWorld(position.x, position.y);

	for (std::map<uint, Entity*>::iterator iterator = activeEntities.begin();
		iterator != activeEntities.end(); 
		iterator++)
	{
		if (iterator->second->getCollider())
		{
			SDL_Rect rect = iterator->second->getCollider()->rect;
			if (rect.x <= pos.x	&& rect.x + rect.w >= pos.x
				&& rect.y <= pos.y && 	rect.y + rect.h >= pos.y)
			{
				ret = (*iterator).first;
			}
		}
	}

	return ret;
}

uint EntityManager::getEntityOnMouseId()
{
	iPoint pos = app->input->getMouseWorldPosition();

	return getEntityAtPositionId(pos);
}

Entity* EntityManager::getEntityAtPosition(iPoint position)
{
	Entity* ret = NULL;

	for (std::map<uint, Entity*>::iterator iterator = activeEntities.begin();
		iterator != activeEntities.end();
		iterator++)
	{
		if (iterator->second->getCollider())
		{
			SDL_Rect rect = iterator->second->getCollider()->rect;
			if (rect.x <= position.x	&& rect.x + rect.w >= position.x
				&& rect.y <= position.y && 	rect.y + rect.h >= position.y)
			{
				ret = (*iterator).second;
			}
		}
	}

	return ret;
}

Entity* EntityManager::getEntityOnMouse()
{
	iPoint pos = app->input->getMouseWorldPosition();

	return getEntityAtPosition(pos);
}

void EntityManager::setEnemiesAttributes()
{
	//TODO: Should load all attributes from an xml

	//############################
	//###     EnemyPaladin     ###
	//############################

	

	//############################
	//###      EnemyWolf       ###
	//############################

	

	//############################
	//###    EnemyGriswold     ###
	//############################

	
}