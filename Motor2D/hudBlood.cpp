#include "hudBlood.h"
#include "Player.h"
#include "App.h"
#include "Game.h"
#include "Gui.h"

//NOTE: PROVISIONAL 
#include "Fonts.h"
hudBlood::hudBlood()
{
	active = true;
}

hudBlood::~hudBlood()
{

}

bool hudBlood::start()
{
	player = app->game->player;
	blood_current = 0;

	
	blood_label = app->gui->addGuiLabel("0", NULL, { 10, 5 }, NULL,FONT_WHITE, this);
	Hud_gui_elements.push_back(blood_label);

	return true;
}

bool hudBlood::preUpdate()
{
	return true;
}

bool hudBlood::update(float dt)
{
	return true;
}

bool hudBlood::postUpdate()
{
	return true;
}

bool hudBlood::cleanUp()
{
	for (int i = 0; i < Hud_gui_elements.size(); i++)
	{
		for (list<GuiElement*>::iterator item2 = app->gui->gui_elements.begin(); item2 != app->gui->gui_elements.end(); item2++)
		{
			if ((*item2) == Hud_gui_elements[i])
			{
				RELEASE(*item2);
				app->gui->gui_elements.erase(item2);
				break;
			}
		}
	}

	Hud_gui_elements.clear();

	return true;
	return true;
}

void hudBlood::IncreaseBlood(int blood)
{
	blood_current += blood;
	p2SString text;
	text.create("%i", blood_current);
	blood_label->SetText(text);
}

void hudBlood::SetBlood(int blood)
{
	blood_current = blood;
	p2SString text;
	text.create("%i", blood_current);
	blood_label->SetText(text);
}