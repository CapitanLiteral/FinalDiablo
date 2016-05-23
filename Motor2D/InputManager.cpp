#include "InputManager.h"
#include "App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Gui.h"
#include "FileSystem.h"
#include "Input.h"
#include "Fonts.h"
#include "Audio.h"
#include "SDL\include\SDL.h"

InputManager::InputManager() : Module()
{
	name = "inputManager";
}

// Destructor
InputManager::~InputManager()
{}

// Called when before render is available
bool InputManager::awake(pugi::xml_node& conf)
{
	bool ret = true;

	//Loading shortcuts path xml
	inputs_file_path = conf.child("shortcuts_path").attribute("value").as_string();

	return ret;
}

// Called before all Updates
bool InputManager::preUpdate()
{
	bool ret = true;

	//DOWN
	//Code with down queues here
	if (!app->input->down_queue.empty())
	{
		for (int i = 0; i < app->input->down_queue.size(); i++)
		{
			list<ShortCut*>::iterator it = shortcuts_list.begin();
			while (it != shortcuts_list.end())
			{
				if (app->input->down_queue.front() + i == (*it)->command && (*it)->type == DOWN)
					(*it)->active = true;
				++it;
			}
		}
	}
	//UP
	//Code with up queues here
	if (!app->input->up_queue.empty())
	{
		for (int i = 0; i < app->input->up_queue.size(); i++)
		{
			list<ShortCut*>::iterator it = shortcuts_list.begin();
			while (it != shortcuts_list.end())
			{
				if (app->input->up_queue.front() + i == (*it)->command && (*it)->type == UP)
					(*it)->active = false;
				++it;
			}
		}
	}
	//REPEAT
	//Code with repeat queues here
	if (!app->input->repeat_queue.empty())
	{
		for (int i = 0; i < app->input->repeat_queue.size(); i++)
		{
			list<ShortCut*>::iterator it = shortcuts_list.begin();
			while (it != shortcuts_list.end())
			{
				if (app->input->repeat_queue.front() + i == (*it)->command && (*it)->type == REPEAT)
					(*it)->active = true;
				++it;
			}
		}
	}

	return ret;
}

bool InputManager::update(float dt)
{
	bool ret = true;

	list<ShortCut*>::iterator it = shortcuts_list.begin();
	while (it != shortcuts_list.end())
	{
		if ((*it)->ready_to_change)
		{
			static SDL_Event event;

			// COMPLETELY STOPS THE GAME...
			// no time to change it
			SDL_WaitEvent(&event);

			if (event.type == SDL_KEYDOWN)
			{
				std::string code = SDL_GetScancodeName(event.key.keysym.scancode);
				bool keyAlreadyAssigned = false;
				list<ShortCut*>::iterator it2 = shortcuts_list.begin();

				while (it2 != shortcuts_list.end())
				{
					if (strcmp((*it2)->command.c_str(), code.c_str()) == 0)
					{
						chooseKey->active = false;
						keyRepeated->active = true;
						keyAlreadyAssigned = true;
					}
					it2++;
				}

				if (!keyAlreadyAssigned)
				{
					(*it)->command = code;
					ChangeShortcutCommand((*it));
					chooseKey->active = false;
					keyRepeated->active = false;
					(*it)->ready_to_change = false;

					list<ShortCut*>::iterator it = shortcuts_list.begin();
					while (it != shortcuts_list.end())
					{
						(*it)->active = false;
						++it;
					}
				}
				
			}

			return ret;
		}

		++it;
	}

	if (chooseKey && keyRepeated)
	{
		chooseKey->active = false;
		keyRepeated->active = false;
	}

	return ret;
}

// Called after all Updates
bool InputManager::postUpdate()
{
	bool ret = true;

	list<ShortCut*>::iterator it = shortcuts_list.begin();

	while (it != shortcuts_list.end())
	{
		(*it)->active = false;
		++it;
	}

	return ret;
}

// Called before quitting
bool InputManager::cleanUp()
{
	bool ret = true;

	shortcuts_list.clear();

	return ret;
}

bool InputManager::loadShortcutsInfo(GuiElement* parent)
{
	bool ret = true;

	//Loading shortcuts info

	pugi::xml_document	inputs_data;
	pugi::xml_node		node;

	char* buf;
	int size = app->fs->Load(inputs_file_path.c_str(), &buf);
	pugi::xml_parse_result result = inputs_data.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
	{
		LOG("Could not load xml file %s. PUGI error: &s", inputs_file_path.c_str(), result.description());
		return false;
	}
	else
		node = inputs_data.child("inputs_data");

	//Pop-up position
	iPoint pos = parent->getLocalPosition();

	int count = 2;
	int spacing;

	for (node = node.child("shortcut"); node && ret; node = node.next_sibling("shortcut"))
	{
		ShortCut* shortcut = new ShortCut();

		std::string type_tmp = node.child("TYPE").attribute("value").as_string();
		if (type_tmp == "UP") shortcut->type = UP;
		else if (type_tmp == "DOWN") shortcut->type = DOWN;
		else if (type_tmp == "REPEAT") shortcut->type = REPEAT;

		shortcut->name = node.child("name").attribute("value").as_string();
		shortcut->command = node.child("command").attribute("value").as_string();

		spacing = 63 - ((int(shortcut->command.size()) - 1) * 5);

		if (count < 6)
		{
			shortcut->shortcut_label = app->gui->addGuiLabel(shortcut->name.data(), app->font->smallFont, { 10, (30 * count) }, parent, TextColor::FONT_WHITE, this);
			shortcut->decor = app->gui->addGuiImage({ 150, (30 * count) - 5 }, { 320, 960, 133, 25 }, parent, this);
			shortcut->command_label = app->gui->addGuiLabel(shortcut->command.data(), app->font->smallFont, { 150 + spacing, (30 * count) }, parent, TextColor::FONT_BLACK, this);
			shortcut->trigger = app->gui->addGuiImage({ 220, (30 * count) - 5 }, { 1380, 550, 133, 25 }, parent, this);
		}
		else if (count < 10)
		{
			shortcut->shortcut_label = app->gui->addGuiLabel(shortcut->name.data(), app->font->smallFont, { 10, (30 * count) + 25 }, parent, TextColor::FONT_WHITE, this);
			shortcut->decor = app->gui->addGuiImage({ 150, (30 * count) + 20 }, { 320, 960, 133, 25 }, parent, this);
			shortcut->command_label = app->gui->addGuiLabel(shortcut->command.data(), app->font->smallFont, { 150 + spacing, (30 * count) + 25 }, parent, TextColor::FONT_BLACK, this);
			shortcut->trigger = app->gui->addGuiImage({ 220, (30 * count) + 20 }, { 1380, 550, 133, 25 }, parent, this);
		}
		else if (count < 14)
		{
			shortcut->shortcut_label = app->gui->addGuiLabel(shortcut->name.data(), app->font->smallFont, { 310, (30 * (count - 8)) }, parent, TextColor::FONT_WHITE, this);
			shortcut->decor = app->gui->addGuiImage({ 450, (30 * (count - 8)) - 5 }, { 320, 960, 133, 25 }, parent, this);
			shortcut->command_label = app->gui->addGuiLabel(shortcut->command.data(), app->font->smallFont, { 450 + spacing, (30 * (count - 8)) }, parent, TextColor::FONT_BLACK, this);
			shortcut->trigger = app->gui->addGuiImage({ 450, (30 * (count - 8)) - 5 }, { 1380, 550, 133, 25 }, parent, this);
		}
		else
		{
			shortcut->shortcut_label = app->gui->addGuiLabel(shortcut->name.data(), app->font->smallFont, { 310, (30 * (count - 8)) + 25 }, parent, TextColor::FONT_WHITE, this);
			shortcut->decor = app->gui->addGuiImage({ 450, (30 * (count - 8)) + 20 }, { 320, 960, 133, 25 }, parent, this);
			shortcut->command_label = app->gui->addGuiLabel(shortcut->command.data(), app->font->smallFont, { 450 + spacing, (30 * (count - 8)) + 25 }, parent, TextColor::FONT_BLACK, this);
			shortcut->trigger = app->gui->addGuiImage({ 450, (30 * (count - 8)) + 20 }, { 1380, 550, 133, 25 }, parent, this);
		}
		

		shortcut->trigger->interactable = true;
		shortcuts_list.push_back(shortcut);

		count++;
	}

	app->gui->addGuiLabel("Options Menu", app->font->bigAssFont, { 190, 5 }, parent, TextColor::FONT_WHITE, this);
	chooseKey = app->gui->addGuiLabel("Choose Key", app->font->bigAssFont, { 350, 275 }, parent, TextColor::FONT_WHITE, this);
	keyRepeated = app->gui->addGuiLabel("Invalid Key", app->font->bigAssFont, { 350, 275 }, parent, TextColor::FONT_RED, this);

	return ret;
}

void InputManager::OnEvent(GuiElement* gui, GUI_Event event)
{
	if (event == EVENT_MOUSE_LEFTCLICK_DOWN)
	{
		list<ShortCut*>::iterator it = shortcuts_list.begin();
		while (it != shortcuts_list.end())
		{
			if (gui == (*it)->trigger)
			{
				(*it)->ready_to_change = true;
				chooseKey->active = true;
				keyRepeated->active = false;
				break;
			}

			++it;
		}
	}
}

bool InputManager::CheckShortcut(ShortCutID id)
{
	bool ret = false;

	int i = 0;
	std::list<ShortCut*>::iterator it = shortcuts_list.begin();

	while (it != shortcuts_list.end())
	{
		if (i == int(id))
		{
			ret = (*it)->active;
			break;
		}

		i++;
		it++;
	}

	return ret;
}

void InputManager::ChangeShortcutCommand(ShortCut* shortcut)
{
	shortcut->command_label->SetText(shortcut->command, app->font->smallFont);

	iPoint posLabel = shortcut->command_label->getLocalPosition();
	iPoint posDecor = shortcut->decor->getLocalPosition();
	posLabel.x = posDecor.x + 63 - ((int(shortcut->command.size()) - 1) * 5);
	shortcut->command_label->SetLocalPosition(posLabel);

	shortcut->ready_to_change = false;
}