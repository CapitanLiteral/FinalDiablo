#ifndef __PALADIN_H__
#define __PALADIN_H__

#include "Entity.h"

class GuiImage;

class Paladin : public Entity
{
public: 
	Paladin(iPoint pos);
	~Paladin();

	bool entityUpdate(float dt);

private:
	entityState updateAction();
	void handleInput();

	//Movement
	void setInitVelocity();
	void move(float dt);
	bool isTargetReached();
	void updateVelocity(float dt);

	void updateMovement(float dt);
};
#endif