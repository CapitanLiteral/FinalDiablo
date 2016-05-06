#ifndef _HUDBLOOD_H_
#define _HUDBLOOD_H_

#include "hudElement.h"

class Player;
class GuiLabel;
enum TextColor;
class hudBlood : public hudElement
{
public:
	//Constructor
	hudBlood();

	//Destructor
	~hudBlood();

	//Called before fist frame
	bool start();

	//Called before each loop iteration
	bool preUpdate();

	//Called each frame
	bool update(float dt);

	//Called after each loop iteration
	bool postUpdate();

	//Called before quitting
	bool cleanUp();

	//Utils
	void IncreaseBlood(int blood);
	void SetBlood(int blood);

public:

	int			blood_current;
	Player*	player;

	GuiLabel*	blood_label;
};

#endif