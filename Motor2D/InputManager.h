#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "Module.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "GuiElements.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>
#include <string>

enum INPUT_TYPE
{
	DOWN,
	UP,
	REPEAT
};

struct ShortCut
{
	ShortCut() : active(false)
	{}

	ShortCut(INPUT_TYPE _type, string _name, string _command) : active(false)
	{
		type = _type;
		name = _name;
		command = _command;
	}

	INPUT_TYPE	 type;
	bool		 active = false;
	bool		 ready_to_change = false;
	std::string		 name;
	std::string		 command;

	GuiImage*	 decor = nullptr;
	GuiLabel*	 command_label = nullptr;
	GuiLabel*	 shortcut_label = nullptr;
	GuiImage*	 trigger = nullptr;
};

enum ShortCutID
{
	Sid_CHARACTER = 0,
	Sid_INVENTORY,
	Sid_TREE,
	Sid_MENU,
	Sid_POTION_1,
	Sid_POTION_2,
	Sid_POTION_3,
	Sid_POTION_4,
	Sid_MOVE_UP,
	Sid_MOVE_DOWN,
	Sid_MOVE_LEFT,
	Sid_MOVE_RIGHT,
	Sid_VOL_UP,
	Sid_VOL_DOWN
};

class InputManager: public Module
{
public:

	InputManager();

	// Destructor
	virtual ~InputManager();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Called before all Updates
	bool preUpdate();

	bool update(float dt);

	// Called after all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	bool loadShortcutsInfo(GuiElement* parent);

	void OnEvent(GuiElement* gui, GUI_Event event);

	//Shortcuts list
	std::list<ShortCut*>			shortcuts_list;

	//Check for shortcut state
	bool CheckShortcut(ShortCutID id = Sid_CHARACTER);

	//Guiding labels for input changes
	GuiLabel*		chooseKey = NULL;
	GuiLabel*		keyRepeated = NULL;

private:

	//Refresh commands once have been changed
	void ChangeShortcutCommand(ShortCut* shortcut);

	//Shortcuts xml file path
	std::string		inputs_file_path;
};

#endif // __INPUT_MANAGER_H__