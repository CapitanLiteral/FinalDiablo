#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "p2Point.h"
#include <map>
#include "Animation.h"

#include "SDL\include\SDL.h"

class Player;
class Paladin;
class Wolf;
class Griswold;
class Npc;
class Entity;
enum entityType;
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

	Wolf* createWolf(iPoint pos);

	Griswold* createGriswold(iPoint pos);

	Entity* createNpc(iPoint position, entityType type);

	SDL_Texture* getPaladinTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getPaladinAnimation();

	SDL_Texture* getWolfTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getWolfAnimation();

	SDL_Texture* getGriswoldTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getGriswoldAnimation();

	SDL_Texture* getCounselorTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getCounselorAnimation();

	SDL_Texture* getHealerTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getHealerAnimation();

	SDL_Texture* getGossipTexture();
	std::map<std::pair<entityState, entityDirection>, Animation>* getGossipAnimation();

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
	bool loadNpcAnimations();

	//Paladin
	SDL_Texture* paladinTexture = NULL;
	std::map<std::pair<entityState, entityDirection>, Animation>	paladinAnim;

	//Wolf
	SDL_Texture* wolfTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	wolfAnim;

	//Griswold texture
	SDL_Texture* griswoldTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	griswoldAnim;

	//Counselor texture
	SDL_Texture* counselorTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	counselorAnim;

	//Healer texture
	SDL_Texture* healerTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	healerAnim;
	
	//Gossip texture
	SDL_Texture* gossipTexture;
	std::map<std::pair<entityState, entityDirection>, Animation>	gossipAnim;
};

#endif