#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "Input.h"
#include "Gui.h"
#include "Audio.h"
#include "Animation.h"


#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )


Gui::Gui() : Module()
{
	name.create("gui");
	focus = NULL;
}

// Destructor
Gui::~Gui()
{}

// Called before render is available
bool Gui::awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	debug = false;

	return ret;
}

// Called before the first frame
bool Gui::start()
{
	mouse_clicked == false;
	atlas = app->tex->Load(atlas_file_name.c_str());
	//Disables the cursor
	SDL_ShowCursor(SDL_DISABLE);
	//Mouse--------
	mouse = new GuiMouseImage({ mouse_x, mouse_y }, { 189, 98, 33, 26 }, NULL, this);
	dragged_item = NULL;
	//-------------
	return true;
}

bool Gui::preUpdate()
{
	mouse_hovering = false;
	mouse->update();

	//---------------
	if (app->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	GuiElement* hover_element = FindSelectedElement();

	if (hover_element && hover_element->focusable && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		focus = hover_element;

	if (dragged_item)
	{
		dragged_item->Move();
		if (hover_element == NULL && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			dragged_item->inventory->items.remove(dragged_item);
			RELEASE(dragged_item);
			mouse_hovering = true;
		}
	}

	
	list<GuiElement*>::iterator item;

	for (item = gui_elements.begin(); item != gui_elements.end(); item++)
	{
		if (((*item)->interactable || &(*item._Ptr->_Myval) == hover_element) && (*item)->active)
		{
			(*item)->CheckEvent(hover_element, focus, mouse_hovering);
			
		}
	}

	for (item = gui_elements.begin(); item != gui_elements.end(); item++)
	{
		if ((*item)->active)
			(*item)->update(hover_element, focus);
	}
	

	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		mouse_clicked = true;
	}

	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		mouse_clicked = true;
	}
	else if (mouse_clicked == true)
	{
		mouse_clicked = false;
	}
	
	return true;
}

// Called after all updates
bool Gui::postUpdate()
{
	list<GuiElement*>::iterator item = gui_elements.begin();
	for (; item != gui_elements.end(); item++)
	{
		if ((*item)->active)
		{
			(*item)->draw();

			if(app->debug) (*item)->drawDebug();
		}
	}

	if (dragged_item)
	{
		dragged_item->draw();
		if (app->debug) dragged_item->drawDebug();
	}
	else
	{
		mouse->draw();
	}

	return true;
}

// Called before quitting
bool Gui::cleanUp()
{
	gui_elements.clear();

	return true;
}

// const getter for atlas
SDL_Texture* Gui::getAtlas() const
{
	return atlas;
}


//Creators
GuiImage* Gui::addGuiImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list)
{
	GuiImage* image = new GuiImage(p, r, par, list);
	if (image->parent != NULL)image->parent->addChild(image);
	gui_elements.push_back(image);
	return image;
}

Fader* Gui::addFader(iPoint p, SDL_Rect r, GuiElement* par, Module* list, SDL_Texture* tex, int alpha)
{
	Fader* fade = new Fader(p, r, par, list, tex, alpha);
	gui_elements.push_back(fade);
	return fade;
}

GuiAnimation* Gui::addGuiAnimation(iPoint p, SDL_Rect r, GuiElement* par, Module* list, SDL_Texture* tex, Animation* anim)
{
	GuiAnimation* guiAnimation = new GuiAnimation(p, r, par, list, tex, anim);
	gui_elements.push_back(guiAnimation);
	return guiAnimation;
}

GuiLabel* Gui::addGuiLabel(std::string t, _TTF_Font* f, iPoint p, GuiElement* par,TextColor color, Module* list)
{
	GuiLabel* label;
	
	if (f)
		label = new GuiLabel(t, f, p,color, par, list);
	else
		label = new GuiLabel(t, app->font->default, p,color, par, list);

	gui_elements.push_back(label);
	if (label->parent != NULL)label->parent->addChild(label);
	return label;
}
GuiLabel* Gui::addGuiLabel(std::string t, _TTF_Font* f, iPoint p, GuiElement* par, Module* list)
{
	GuiLabel* label;
	TextColor color = FONT_BLACK;
	if (f)
		label = new GuiLabel(t, f, p, color, par, list);
	else
		label = new GuiLabel(t, app->font->default, p, color, par, list);

	gui_elements.push_back(label);
	if (label->parent != NULL)label->parent->addChild(label);
	return label;
}
GuiImage* Gui::addGuiImageWithLabel(iPoint p, SDL_Rect r, std::string t, _TTF_Font* f, iPoint i, GuiElement* par, Module* list)
{
	GuiImage* image = new GuiImage(p, r, par, list);
	if (image->parent != NULL)image->parent->addChild(image);
	gui_elements.push_back(image);
	i.x = i.x + image->getScreenPosition().x;
	i.y = i.y + image->getScreenPosition().y - r.h;
	GuiLabel* label = addGuiLabel(t, f,i,  NULL, FONT_WHITE, list);
	
	image->SetLabel(label);
	gui_elements.push_back(label);
	return image;
}

GuiInputBox* Gui::addGuiInputBox(std::string t, _TTF_Font* f, iPoint p, int width, SDL_Rect r, iPoint offset, GuiElement* par, Module* list)
{
	GuiInputBox* input = new GuiInputBox(t, f, p, width, r, offset, par, list);
	gui_elements.push_back(input);
	if (input->parent != NULL)input->parent->addChild(input);
	return input;
}

GuiButton* Gui::addGuiButton(iPoint p, SDL_Rect idle_r1, SDL_Rect hover_r1, SDL_Rect click_r1, std::string t, _TTF_Font* f, Module* list, GuiElement* parent)
{
	GuiButton* button = new GuiButton(p, idle_r1, hover_r1, click_r1, t, f, list, parent);
	gui_elements.push_back(button);
	if (button->parent != NULL)button->parent->addChild(button);
	return button;

}

GuiElement* Gui::FindSelectedElement()
{
	list<GuiElement*>::reverse_iterator item = gui_elements.rbegin();

	for (; item != gui_elements.rend(); item++)
	{
		if ((*item)->CheckCollision(app->input->getMousePosition()) && (*item)->active)
		{
			return (*item);
		}
	}
	return NULL;
}

//EXERCISE 1
GuiSlider* Gui::addGuiSlider(iPoint p, SDL_Rect tex_1, SDL_Rect tex_2, int width, int thumb_h, iPoint offset, float value, GuiElement* par, Module* list)
{
	GuiSlider* slider = new GuiSlider(p, tex_1, tex_2, width, thumb_h, offset, value, par, list);
	gui_elements.push_back(slider);
	if (par != NULL)par->addChild(slider);
	return slider;
}


GuiInventory* Gui::addGuiInventory(iPoint p, SDL_Rect r, int col, int rows, int slot_w, int slot_h, GuiElement* par, Module* list)
{
	GuiInventory* inventory = new GuiInventory(p, r, col, rows, slot_w, slot_h, par, list);
	gui_elements.push_back(inventory);
	return inventory;
}

bool Gui::clearUI()
{
	list<GuiElement*>::reverse_iterator item = gui_elements.rbegin();

	for (; item != gui_elements.rend(); item++)
	{
		GuiElement* tmp = *item;
		RELEASE(tmp);
	}

	gui_elements.clear();

	return true;
}