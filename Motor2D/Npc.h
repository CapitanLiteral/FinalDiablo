#ifndef __ENT_NPC_H__
#define __ENT_NPC_H__

#include "Entity.h"
#include "p2Point.h"
#include "EntityManager.h"
#include "Timer.h"

class Collider;
class Sprite;
class GuiLabel;
class GuiImage;
enum entityType;

class Npc : public Entity
{
public:

	//Constructor
	Npc(const iPoint &p, uint ID);
	Npc::~Npc();

	//Drawing methods
	void draw();
	void drawDebug();

	bool playerInRange();

	Sprite* getSprite() const;

public:

	float	playerRange;

	SDL_Texture* texture;


};

class NpcCounselor : public Npc
{
public:

	//Constructor
	NpcCounselor(const iPoint &p, uint ID);
	~NpcCounselor();

	bool update(float dt);

	void loadGui();

public:
	SDL_Texture* counselorAtlas = NULL;
	GuiImage* startingImage = NULL;
};


class NpcHealer : public Npc
{
public:

	//Constructor
	NpcHealer(const iPoint &p, uint ID);
	~NpcHealer();

	bool update(float dt);

	void loadGui();

public:
	GuiImage* startingImage = NULL;
};

class NpcGossip : public Npc
{
public:

	//Constructor
	NpcGossip(const iPoint &p, uint ID);
	~NpcGossip();

	bool update(float dt);

public:

};



#endif