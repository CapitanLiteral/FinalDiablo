#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

Input::Input() : Module()
{
	name.create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::preUpdate()
{
	static SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//Clean queues
	while (!down_queue.empty())		down_queue.pop();
	while (!up_queue.empty())		up_queue.pop();
	while (!repeat_queue.empty())	repeat_queue.pop();


	for (int i = 0; i < MAX_KEYS; ++i)
	{
		//Translate key number to scancode enum (ex: 14 is SDL_SCANCODE_K)
		SDL_Scancode code = (SDL_Scancode)i;

		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
				down_queue.push(SDL_GetScancodeName(code));
			}
			else
			{
				keyboard[i] = KEY_REPEAT;
				repeat_queue.push(SDL_GetScancodeName(code));
			}

		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
				up_queue.push(SDL_GetScancodeName(code));
			}

			else
			{
				keyboard[i] = KEY_IDLE;
			}

		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
			{	int scale = app->win->getScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
			}
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;

			case SDL_TEXTINPUT:
				input_text.insert(cursor_pos,event.text.text);
				cursor_pos += strlen(event.text.text);
			break;

			case SDL_KEYDOWN:
				if (enable_input)
				{
					//Ask ric why do i need to use the virtual keys
					switch (event.key.keysym.sym)
					{
					case SDLK_BACKSPACE:
						input_text.erase(input_text.at(cursor_pos - 1));
						if (cursor_pos > 0)
							cursor_pos--;
					break;
					case SDLK_DELETE:
						input_text.erase(input_text.at(cursor_pos));
					break;
					case SDLK_RIGHT:
						if (cursor_pos < input_text.size())
							cursor_pos++;
					break;
					case SDLK_LEFT:
						if (cursor_pos >0)
							cursor_pos--;
					break;
					case SDLK_HOME:
						cursor_pos = 0;
					break;
					case SDLK_END:
						cursor_pos = input_text.size();
					break;

					}
				}
		}
	}

	return true;
}

// Called before quitting
bool Input::cleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool Input::getWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

iPoint Input::getMousePosition()
{
	return{ mouse_x, mouse_y};
}

void Input::getMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

iPoint Input::getMouseWorldPosition()
{
	return{ mouse_x - app->render->camera.x, mouse_y - app->render->camera.y };
}

void Input::getMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
	mouse_motion_x = mouse_motion_y = 0;
}

iPoint Input::getMouseMotion()
{
	
	iPoint ret = { mouse_motion_x, mouse_motion_y };
	mouse_motion_x = mouse_motion_y = 0;

	return ret;
	
}

void Input::startInput(std::string edit_input, int pos)
{
	SDL_StartTextInput();
	//so it can have lots of input boxes
	input_text = edit_input;
	cursor_pos = pos;
	enable_input = true;
}
void Input::StopInput()
{
	enable_input = false;
	SDL_StopTextInput();
}
std::string Input::getInput(int& cursor)
{
	cursor = cursor_pos;
	return input_text;
}
