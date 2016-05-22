#ifndef __WOLF_H__
#define __WOLF_H__

#include "Entity.h"
#include "Timer.h"

class Wolf : public Entity
{
public:
	Wolf(iPoint pos);
	~Wolf();

	bool entityUpdate(float dt);

private:
	entityState updateAction();
	void handleInput();

	//Movement
	void setInitVelocity();
	void move(float dt);
	void updateVelocity(float dt);

	void updateMovement(float dt);

	bool isInWalkableTile(iPoint coords);

	Timer timerChangeDir;
	uint32 changeDir = 1;

};

#endif