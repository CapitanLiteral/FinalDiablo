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

	NpcCounselor(const iPoint &p, uint ID);
	~NpcCounselor();

	bool entityUpdate(float dt);
	
	void draw();
	void drawDebug();
	
	bool playerInRange();

	void loadGui();

public:
	GuiImage* introductionImage = NULL;
	GuiImage* secondImage = NULL;
	GuiImage* readyToGoImage = NULL;
	GuiImage* secondIntroductionImage = NULL;
	GuiImage* secondSecondaryImage = NULL;
	GuiImage* finalFightImage = NULL;
	bool readed;
	bool readyForSecondZone;
	float	playerRange;
	Timer ret;
	Timer ret1;
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
	GuiImage* introductionImage = NULL;
	GuiImage* goodDayImage = NULL;
	GuiImage* helloImage = NULL;
	GuiImage* changeImage = NULL;
	GuiImage* lordImage = NULL;
	float	playerRange;
	uint readed;
	uint num;
};

class NpcGossip : public Entity
{
public:

	//Constructor
	NpcGossip(const iPoint &p, uint ID);
	~NpcGossip();

	bool entityUpdate(float dt);
	bool playerInRange();
	void draw();
	void drawDebug();
	void loadGui();

public:
	GuiImage* introductionImage = NULL;
	GuiImage* goodDayImage = NULL;
	GuiImage* helloImage = NULL;
	GuiImage* changeImage = NULL;
	GuiImage* lordImage = NULL;
	float	playerRange;
	uint readed;
	uint num;
};



#endif