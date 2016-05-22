#ifndef __WOLF_H__
#define __WOLF_H__

#include "Entity.h"
#include "Timer.h"

class Wolf : public Entity
{
public:
	Wolf(iPoint pos, std::vector<iPoint> points);//CARE: THIS IS TMP(vector)
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
	bool isTargetReached();

	Timer timerChangeDir;
	float changeDir = 1.5f;

	std::vector<iPoint> possiblePoints;//CARE: THIS IS TMP

};

#endif