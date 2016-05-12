#ifndef _SNWIN_H_
#define _SNWIN_H_

#include "Scene.h"
#include "Timer.h"
#include <list>

using namespace std;

class Entity;
class GuiImage;
class GuiElement;
class GuiButton;

class snWin : public Scene
{
public:
	// Constructor
	snWin();

	// Destructor
	~snWin();

	bool start();
	bool update(float dt);

	// Called on certain event
	void OnEvent(GuiElement* element, GUI_Event even);

	//Load/unLoad, called when the scene changes
	bool Load();
	bool unLoad();

	//adding Enemies
	void addEnemies();

private:


public:
	//NOTE: provisional.
	Timer ret;

	GuiButton* backMenuButton = NULL;
	GuiImage* background = NULL;


};

#endif _SNWIN_