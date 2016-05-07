#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include <list>

using namespace std;

class Scene;
class snIntro;
class snOutdoor1;
class Act1;
class snWin;

class SceneManager : public Module
{
public:
	SceneManager();

	//Destructor
	~SceneManager();

	// Called before render is available
	bool awake(pugi::xml_node& conf);

	// Called before the first frame
	bool start();

	//preUpdate
	bool preUpdate();

	//update
	bool update(float dt);

	//postUpdate
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Methods
	void addScene(Scene* scene);
	bool ChangeScene(Scene* new_scene);

public:
	snIntro*	intro = NULL;
	snOutdoor1*	outdoor1 = NULL;
	Act1*		act1 = NULL;
	snWin*		win = NULL;

private:
	list<Scene*>	scenes;
	Scene*		current_scene = NULL;
};

#endif // __SCENEMANAGER_H__

