#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "p2Point.h"
#include <map>
#include "Animation.h"

#include "SDL\include\SDL.h"

class Player;
class Paladin;
class Entity;
enum entityState;
enum entityDirection;

class EntityManager : public Module
{
public:
	EntityManager();

	virtual ~EntityManager();


	bool awake(pugi::xml_node&);

	bool start();

	bool preUpdate();

	bool update(float dt);

	bool postUpdate();

	bool cleanUp();

	Entity* getEntity(uint id);

	bool remove(uint _id);

	//Entity* createEntity(iPoint pos, const char* textureName, SDL_Rect& section, SDL_Rect& collider, entityType _type);

	Paladin* createPaladin(iPoint pos);

	SDL_Texture* getPaladinTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getPaladinAnimation();

	SDL_Texture* getWolfTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getWolfAnimation();

	SDL_Texture* getGriswoldTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getGriswoldAnimation();

	uint getEntityAtPositionId(iPoint position);

	uint getEntityOnMouseId();

	Entity* getEntityAtPosition(iPoint position);

	Entity* getEntityOnMouse();


	std::map<uint, Entity*>		activeEntities;
	std::map<uint, Entity*>		inactiveEntities;

private:

	uint nextId = 1;

	Player* player = NULL;

	bool doLogic;

	float updateMsCycle; // 1/fps logic
	float acummulatedTime;

private:
	void sortEntities();

	bool loadEnemiesAnimations();

	//Paladin
	SDL_Texture* paladinTexture = NULL;
	std::map<std::pair<entityState, entityDirection>, Animation>	paladinAnim;

	//Wolf
	SDL_Texture* wolfTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	wolfAnim;

	//Griswold texture
	SDL_Texture* griswoldTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	griswoldAnim;

};

#endif