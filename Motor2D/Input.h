#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "p2Point.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();

	// Called before quitting
	bool cleanUp();

	// Gather relevant win events
	bool getWindowEvent(EventWindow ev);

	void startInput(std::string edit_input, int pos);
	void StopInput();
	std::string getInput(int& cursor);

	// Check key states (includes mouse and joy buttons)
	KeyState getKey(int id) const
	{
		return keyboard[id];
	}

	KeyState getMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check if a certain window event happened
	bool getWindowEvent(int code);

	// get mouse / axis position
	//added for big utility
	
	//gets the world coordinates of the mouse positionb 
	//NOTE : used because there where problems with the map movement
	iPoint getMouseWorldPosition();

	//gets the screen coordinates of the mouse position
	iPoint getMousePosition();
	void getMousePosition(int &x, int &y);
	
	iPoint getMouseMotion();
	void getMouseMotion(int& x, int& y);

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	bool		enable_input;
	std::string   input_text;
	int			cursor_pos;
};

#endif // __INPUT_H__