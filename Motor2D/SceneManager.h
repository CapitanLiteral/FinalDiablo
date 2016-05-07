#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include <list>
#include "Render.h"
#include "Timer.h"
#include "Gui.h"

using namespace std;

class Scene;
class snIntro;
class snOutdoor1;
class snDungeon1;
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
	bool fadeToBlack(Scene* new_scene, float time = 2.0f);

public:
	snIntro*	intro = NULL;
	snOutdoor1*	outdoor1 = NULL;
	snDungeon1* dungeon1 = NULL;
	snWin*		win = NULL;

private:
	list<Scene*>	scenes;
	Scene*		current_scene = NULL;
	Scene*		next_scene = NULL;

	//FADER

	

	bool fadeIn = false;
	bool fadeOut = false;
	float fadeTime;

	SDL_Texture* blackTexture = NULL;
	Sprite black;

	Fader* image = NULL;

	Timer fadeTimer;
};

#endif // __SCENEMANAGER_H__

