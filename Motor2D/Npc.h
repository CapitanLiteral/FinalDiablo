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

class NpcCounselor : public Entity
{
public:

	//Constructor
	NpcCounselor(const iPoint &p, uint ID);
	~NpcCounselor();

	bool entityUpdate(float dt);

	bool playerInRange();

	void drawDebug();
	void loadGui();

public:
	GuiImage* startingImage = NULL;
	float	playerRange;
};


class NpcHealer : public Entity
{
public:

	//Constructor
	NpcHealer(const iPoint &p, uint ID);
	~NpcHealer();

	bool entityUpdate(float dt);

	bool playerInRange();
	void draw();
	void drawDebug();
	void loadGui();

public:
	GuiImage* startingImage = NULL;
	float	playerRange;
};

class NpcGossip : public Entity
{
public:

	//Constructor
	NpcGossip(const iPoint &p, uint ID);
	~NpcGossip();

	bool entityUpdate(float dt);
	bool playerInRange();

	void drawDebug();
	void loadGui();

public:
	GuiImage* startingImage = NULL;
	float	playerRange;
};



#endif