#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneManager.h"
#include "FileSystem.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Fonts.h"
#include "Gui.h"
#include "App.h"
#include "Game.h"
#include "Collision.h"
#include "ParticleManager.h"

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_start(ptimer);

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	sm = new SceneManager();
	fs = new FileSystem();
	map = new Map();
	pathfinding = new PathFinding();
	font = new Fonts();
	gui = new Gui();
	game = new Game();
	collision = new Collision();
	particleManager = new ParticleManager();

	// Ordered for awake / start / update
	// Reverse order of cleanUp
	addModule(fs);
	addModule(input);
	addModule(win);
	addModule(tex);
	addModule(audio);
	addModule(map);
	addModule(pathfinding);
	addModule(font);
	addModule(particleManager);
	addModule(gui);

	// scene manager
	addModule(sm);

	// collision
	addModule(collision);

	// game last 
	addModule(game);

	// render last to swap buffer
	addModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// release modules
	list<Module*>::reverse_iterator item = modules.rbegin(); 

	while(item != modules.rend())
	{
		RELEASE(*item);
		++item;
	}

	modules.clear();
}

void App::addModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool App::awake()
{
	PERF_start(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = loadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if(cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	if(ret == true)
	{
		list<Module*>::iterator item;
		item = modules.begin();
		
		
		while (item != modules.end() && ret == true)
		{
			ret = (*item)->awake(config.child((*item)->name.getString()));
			++item;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::start()
{
	PERF_start(ptimer);
	bool ret = true;

	list<Module*>::iterator item;
	item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if ((*item) != game)
		{
			ret = (*item)->start();
		} 
		else
			(*item)->active = false;

		++item;
	}

	startup_time.start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::update()
{
	bool ret = true;
	prepareUpdate();

	if(input->getWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = preUpdate();

	if(ret == true)
		ret = doUpdate();

	if(ret == true)
		ret = postUpdate();

	finishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node App::loadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = app->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void App::prepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.start();
}

// ---------------------------------------------
void App::finishUpdate()
{
	if(want_to_save == true)
		savegameNow();

	if(want_to_load == true)
		loadGameNow();

	// Activating debug mode for all the Game

	if (input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	// Framerate calculations --
	
	if(last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;


	static char title[256];

	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
			avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
		//app->win->SetTitle(title);
	

	if(capped_ms > 0 && last_frame_ms < capped_ms)
	{
		PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool App::preUpdate()
{
	bool ret = true;

	list<Module*>::iterator item;
	item = modules.begin();
	Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->preUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::doUpdate()
{
	bool ret = true;
	list<Module*>::iterator item;
	item = modules.begin();
	Module* pModule = NULL;
	
	for(item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::postUpdate()
{
	bool ret = true;
	list<Module*>::iterator item;
	Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->postUpdate();
	}

	return ret;
}

// Called before quitting
bool App::cleanUp()
{
	PERF_start(ptimer);
	bool ret = true;

	list<Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->cleanUp();
		++item;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int App::getArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::getArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::getTitle() const
{
	return title.getString();
}

// ---------------------------------------
float App::getDT() const
{
	return dt;
}

// ---------------------------------------
const char* App::getOrganization() const
{
	return organization.getString();
}

// Load / Save
void App::loadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "getSaveGames" list
	want_to_load = true;
	load_game.create("%s%s", fs->getSaveDirectory(), file);
}

// ---------------------------------------
void App::saveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "getSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.create(file);
}

// ---------------------------------------
void App::getSaveGames(list<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool App::loadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.getString(), &buffer);

	if(size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if(result != NULL)
		{
			LOG("Loading new Game State from %s...", load_game.getString());

			root = data.child("game_state");

			list<Module*>::iterator item = modules.begin();
			ret = true;

			while(item != modules.end() && ret == true)
			{
				ret = (*item)->Load(root.child((*item)->name.getString()));
				++item;
			}

			data.reset();
			if(ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (item != modules.end()) ? (*item)->name.getString() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.getString(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", load_game.getString());

	want_to_load = false;
	return ret;
}

bool App::savegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.getString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	list<Module*>::const_iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->name.getString()));
		++item;
	}

	if(ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		fs->Save(save_game.getString(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", save_game.getString());
	}
	else
		LOG("Save process halted from an error in module %s", (item != modules.end()) ? (*item)->name.getString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}