#ifndef __APP_H__
#define __APP_H__
//CHANGE
#include "p2List.h"
#include <list>
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"

using namespace std;

// Modules
class Window;
class Input;
class InputManager;
class Render;
class Textures;
class Audio;
class FileSystem;
class SceneManager;
class Map;
class PathFinding;
class Fonts;
class Gui;
class Game;
class Collision;
class ParticleManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool awake();

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool update();

	// Called before quitting
	bool cleanUp();

	// add a new module to handle
	void addModule(Module* module);

	

	// Exposing some properties for reading
	int getArgc() const;
	const char* getArgv(int index) const;
	const char* getTitle() const;
	const char* getOrganization() const;
	float getDT() const;

	void loadGame(const char* file);
	void saveGame(const char* file) const;
	void getSaveGames(list<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node loadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void prepareUpdate();

	// Call modules before each loop iteration
	void finishUpdate();

	// Call modules before each loop iteration
	bool preUpdate();

	// Call modules on each loop iteration
	bool doUpdate();

	// Call modules after each loop iteration
	bool postUpdate();

	// Load / Save
	bool loadGameNow();
	bool savegameNow() const;

public:

	// Modules
	Window*				win = NULL;
	Input*				input = NULL;
	InputManager*		inputManager = NULL;
	Render*				render = NULL;
	Textures*			tex = NULL;
	Audio*				audio = NULL;
	SceneManager*		sm = NULL;
	FileSystem*			fs = NULL;
	Map*				map = NULL;
	PathFinding*		pathfinding = NULL;
	Fonts*				font = NULL;
	Gui*				gui = NULL;
	Game*				game = NULL;
	Collision*			collision = NULL;
	ParticleManager*	particleManager = NULL;

	//NOTE: Now the debug mode is global form the app, ask Ric
	bool                debug = false;

private:

	list<Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	p2SString			load_game;
	mutable p2SString	save_game;

	PerfTimer			ptimer;
	uint64				frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					capped_ms = -1;
};

extern App* app; // No student is asking me about that ... odd :-S

#endif