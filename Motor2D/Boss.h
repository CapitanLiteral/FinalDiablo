#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Timer.h"

class Boss :
	public Entity
{
public:
	Boss(iPoint pos);
	~Boss();

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

	Timer timerChase;
	float timeMarginChase = 1.0f;

	Timer timerSpell;
	float timeSpell = 3.0f;
};

#endif  //__BOSS_H__