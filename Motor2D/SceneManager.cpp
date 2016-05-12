#include "SceneManager.h"

#include "App.h"
#include "snIntro.h"
#include "Act1.h"	
#include "Act2.h"
#include "snWin.h"
#include "Render.h"
#include "Textures.h"

#include "Game.h"


SceneManager::SceneManager() : Module()
{
	name.create("scene_manager");
	intro = new snIntro();
	//outdoor1 = new snOutdoor1();
	act1 = new Act1();
	act2 = new Act2();
	win = new snWin();

	
	addScene(intro);
	//addScene(outdoor1);
	addScene(act1);
	addScene(act2);
	addScene(win);
}

SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::awake(pugi::xml_node& conf)
{
	current_scene = intro;
	filePropsVillage = conf.child("propAtlas").attribute("file").as_string();
	return true;
}

// Called before the first frame
bool SceneManager::start()
{
	bool ret;
	
	black = Sprite(blackTexture, {1,1});
	black.y = INT_MAX - 1;
	black.setAlpha(0);

	ret = current_scene->start();

	blackTexture = app->tex->Load("textures/Black.png");
	image = app->gui->addFader({ 0, 0 }, { 0, 0, 640, 600 }, NULL, this, blackTexture, 80);
	image->active = false;

	return ret;
}

//preUpdate
bool SceneManager::preUpdate()
{
	return current_scene->preUpdate();
}

//update
bool SceneManager::update(float dt)
{
	//current_scene->update(dt);

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
	bool ret = current_scene->postUpdate();

	if (fadeIn || fadeOut)
		app->render->DrawSprite(&black);

	return ret;
}

// Called before quitting
bool SceneManager::cleanUp()
{
	bool ret = current_scene->cleanUp();

	list<Scene*>::reverse_iterator item = scenes.rbegin();

	while (item != scenes.rend())
	{
		RELEASE(*item);
		++item;
	}

	scenes.clear();

	return ret;
}

void SceneManager::addScene(Scene* scene)
{
	scenes.push_back(scene);
}

// Changes the current scene
bool SceneManager::ChangeScene(Scene* new_scene)
{
	bool ret;
	
	// unload current scene
	if (!(ret = (current_scene->unLoad()))) return ret;

	// start game if scene was intro
	if (current_scene == intro && new_scene != win)
	{
		if (!(ret = (app->game->start()))) return ret;
	}

	if ((new_scene == intro && current_scene != win) || new_scene == win)
	{
		if (!(ret = (app->game->cleanUp()))) return ret;
	}
	
	current_scene = new_scene;
	ret = current_scene->Load();

	return ret;
}

bool SceneManager::fadeToBlack(Scene* new_scene, float time)
{
	bool ret = true;
	bool a = !(fadeIn || fadeOut);
	if (!(fadeIn || fadeOut))
	{
		next_scene = new_scene;
		fadeTime = time;
		fadeIn = true;
		fadeTimer.start();
	}
	
	return ret;
}

std::string SceneManager::getFilePropsVillage()
{
	return filePropsVillage;
}
