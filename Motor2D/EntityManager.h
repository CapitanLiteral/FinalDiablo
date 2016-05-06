#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include <map>
#include <vector>

enum ENTITY_TYPE;
enum ENEMY_TYPE;
class Entity;
class GuiLabel;
using namespace std;

class EntityManager : public Module
{

public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();

	// update
	bool update(float dt);

	// Called each loop iteration
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	Entity* add(iPoint &pos, ENTITY_TYPE type);
	Entity* addEnemy(iPoint &pos, ENEMY_TYPE type);
	bool remove(uint id);
	Entity* getEntity(uint id);

	Entity* entityOnMouse();
	Entity* entityOnCoords(iPoint &pos);

private:
	map<uint, Entity*> activeEntities;
	map<uint, Entity*> inactiveEntities;
	//selection
	//selection_ordered
	uint nextID;
	//uchar filter;

	//SDL_Rect selector;

	void drawAll();
	//void SelectAll(uchar filter);
	//SelectAvailable
	//void calculateSelector
	//void sortEntities

public:
	//Wolf textures

	//Maybe ERROR, watch out // Aixo no ho toco que s'ha d'acabar canviant.

	//Crawler textures
	SDL_Texture* crawler_idle;
	SDL_Texture* crawler_walk;
	SDL_Texture* crawler_attack;
	SDL_Texture* crawler_death;

	//Boss textures
	SDL_Texture* boss_idle;
	SDL_Texture* boss_walk;
	SDL_Texture* boss_attack;
	SDL_Texture* boss_death;

	//Portal texture
	SDL_Texture* portal_tex;

	//Sounds
	int crawler_attackfx;
	int crawler_gethitfx;
	int crawler_deathfx;

	int wolf_attackfx;
	int wolf_gethitfx;
	int wolf_deathfx;

	GuiLabel* enemy_name = NULL;
};

#endif // __EntityManager_H__
