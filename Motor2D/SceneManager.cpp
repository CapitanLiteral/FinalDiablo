#include "SceneManager.h"

#include "App.h"
#include "snIntro.h"
#include "snOutdoor1.h"
#include "Act1.h"	
#include "snWin.h"
#include "Render.h"
#include "Textures.h"

#include "Game.h"


SceneManager::SceneManager() : Module()
{
	intro = new snIntro();
	outdoor1 = new snOutdoor1();
	act1 = new Act1();
	win = new snWin();

	addScene(win);
	addScene(intro);
	addScene(outdoor1);
	addScene(act1);
}


SceneManager::~SceneManager()
{
}

// Called before render is available
bool SceneManager::awake(pugi::xml_node& conf)
{
	current_scene = intro;
	return true;
}

// Called before the first frame
bool SceneManager::start()
{
	
	
	black = Sprite(blackTexture, {1,1});
	black.y = INT_MAX - 1;
	black.setAlpha(0);

	current_scene->start();

	blackTexture = app->tex->Load("textures/Black.png");
	image = app->gui->addFader({ 0, 0 }, { 0, 0, 640, 600 }, NULL, this, blackTexture, 80);
	image->active = false;
	return true;
}

//preUpdate
bool SceneManager::preUpdate()
{
	current_scene->preUpdate();
	return true;
}

//update
bool SceneManager::update(float dt)
{
	current_scene->update(dt);

	bool ret = true;

	if (current_scene != nullptr)
	{
		current_scene->update(dt);
	}

	if (fadeIn)
	{
		if (fadeTimer.ReadSec() <= fadeTime)
		{
			image->setAlpha(int(fadeTimer.ReadSec() * 255) / fadeTime);
		}
		else
		{
			ChangeScene(next_scene);
			fadeIn = false;
			fadeOut = true;
			fadeTimer.start();
		}
	}
	else
	{
		if (fadeTimer.ReadSec() <= fadeTime)
		{
			image->setAlpha(255 - int(fadeTimer.ReadSec() * 255) / fadeTime);
		}
		else
		{
			fadeOut = false;
		}
	}

	if (fadeIn || fadeOut)
		app->render->addSpriteToList(&black);

	image->active = (fadeIn || fadeOut);

	return ret;
}

//postUpdate
bool SceneManager::postUpdate()
{
	current_scene->postUpdate();

	if (fadeIn || fadeOut)
		app->render->DrawSprite(&black);

	return true;
}

// Called before quitting
bool SceneManager::cleanUp()
{
	current_scene->cleanUp();


	list<Scene*>::reverse_iterator item = scenes.rbegin();

	while (item != scenes.rend())
	{
		RELEASE(*item);
		++item;
	}

	scenes.clear();

	return true;
}

void SceneManager::addScene(Scene* scene)
{
	scenes.push_back(scene);
}

// Changes the current scene
bool SceneManager::ChangeScene(Scene* new_scene)
{
	current_scene->unLoad();

	//WARNING: this is purely provisional for the 0.2 version, HAS TO BE CHANGED, the in-game system and all the modules that are activcated there
	if (current_scene == intro)
	{
		//NOTE: may have to be changed in the future
		app->game->Init();
		app->game->start();
	}

	if ((new_scene == intro && current_scene != win) || new_scene == win)
	{
		app->game->active = false;
		app->game->cleanUp();
	}
	//
	current_scene = new_scene;

	current_scene->Load();

	return true;
}

bool SceneManager::fadeToBlack(Scene* new_scene, float time)
{
	bool ret = true;

	if (!fadeIn || !fadeOut)
	{
		next_scene = new_scene;
		fadeTime = time;
		fadeIn = true;
		fadeTimer.start();
	}
	
	return ret;
}