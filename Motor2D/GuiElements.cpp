#include "GuiElements.h"
#include "App.h"
#include "Fonts.h"
#include "Render.h"
#include "Gui.h"
#include "Input.h"
#include "Textures.h"
#include "Animation.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )



/*
--------Constructors
*/

//May i change this in the future for the rect
GuiElement::GuiElement(iPoint p, GUI_Type t, GuiElement* par = NULL, Module* list = NULL) : type(t), parent(par), listener(list), mouseIn(false)
{
	local_rect = { p.x, p.y, tex_rect.w, tex_rect.h };
	interactable = false;
	focusable = false;
	draggable = false;
	mask = true;
}

GuiElement::GuiElement(iPoint p, SDL_Rect r, GUI_Type t, GuiElement* par, Module* list) 
	: parent(par), tex_rect(r), type(t), listener(list), mouseIn(false)
{
	local_rect = { p.x, p.y, tex_rect.w, tex_rect.h };
	interactable = false;
	focusable = false;
	draggable = false;
	mask = false;
}

GuiLabel::GuiLabel(std::string t, _TTF_Font* f, iPoint p, TextColor color, GuiElement* par, Module* list = NULL)
	: GuiElement(p, GUI_LABEL, par, list), text(t), font(f)
{
	// NOTE :Have to polish the texture sistem in the label
	tex = app->font->Print(text.c_str(),color,f);
	this->color = color;
	tex_rect = { 0, 0, 0, 0 };
	app->font->CalcSize(text.c_str(), tex_rect.w, tex_rect.h);
	SetLocalRect({ p.x, p.y, tex_rect.w, tex_rect.h});
}


GuiImage::GuiImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list)
	: GuiElement(p, r, GUI_IMAGE, par, list)
{}

//NOTE :I'm doing an especific constructor, have to change this
GuiInputBox::GuiInputBox(std::string t, _TTF_Font* f, iPoint p, int width, SDL_Rect r, iPoint offset, GuiElement* par, Module* list)
	: GuiElement(p, r, GUI_INPUTBOX, par, list), text(t, f, { 0, 0 },FONT_WHITE, this), image({ offset.x, offset.y }, r, this)
{
	SetLocalRect({ p.x, p.y, width, text.getLocalRect().h});
	
	image.Center(true, true);
	inputOn = false;
	init = false;
	password = false;
	cursor_pos = 0;

	app->font->CalcSize("A", cursor.x, cursor.y);
	cursor.x = 0;
}

GuiButton::GuiButton(iPoint p, SDL_Rect idle_r1, SDL_Rect hover_r1, SDL_Rect click_r1, std::string t, _TTF_Font* f, Module* list, GuiElement* parent)
	: GuiElement(p, idle_r1, GUI_BUTTON, parent, list),
	  button_image(p, idle_r1, this, NULL),
	  button_label(t, f, p,FONT_BLACK, this, NULL)
{
	button_image.Center(true, true);
	button_label.Center(true, true);

	interactable = true;

	idle_tex = idle_r1;
	hover_tex = hover_r1;
	click_tex = click_r1;
}

GuiMouseImage::GuiMouseImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list)
	: GuiElement(p, r, GUI_MOUSE_IMAGE, par, list), mouse_image({ p.x, p.y }, r, this, NULL)
{}
//-----

//draw functions
//MOUSE----------------------------
void GuiMouseImage::draw()
{
	iPoint p = getScreenPosition();

	app->render->Blit(app->gui->getAtlas(),
		p.x - app->render->camera.x,
		p.y - app->render->camera.y,
		&tex_rect);
}
//---------------------------------
void GuiImage::draw()
{
	iPoint p = getScreenPosition();
	
	app->render->Blit(app->gui->getAtlas(),
		p.x - app->render->camera.x,
		p.y - app->render->camera.y,
		&tex_rect);
}

void GuiLabel::draw()
{
	app->render->Blit(tex, getScreenPosition().x - app->render->camera.x, getScreenPosition().y - app->render->camera.y, NULL);
}

void GuiInputBox::draw()
{
	image.draw();
	text.draw();

	if (inputOn)
	{
		iPoint pos = getScreenPosition();
		app->render->DrawQuad({ pos.x + cursor.x - app->render->camera.x, pos.y - app->render->camera.y, CURSOR_WIDTH, cursor.y }, 255, 255, 255);
	}
}

void GuiButton::draw()
{
	button_image.draw();
	button_label.draw();
}
//

//update functions
//MOUSE-----------------
void GuiMouseImage::update()
{
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	
	this->SetLocalPosition({ mouse_x, mouse_y });
}

void GuiInputBox::update(GuiElement* hover, GuiElement* focus)
{
	bool focused = (focus == this);
	
	if (!init && focused)
	{
		text.SetText("");
		init = true;
	}

	if (inputOn != focused)
	{
		if (focused)
			app->input->startInput(text.text, cursor_pos);
		else
			app->input->StopInput();

		inputOn = focused;
	}


	if (inputOn)
	{
		int changed_cursor;
		std::string added_text = app->input->getInput(changed_cursor);
		
		if (added_text != text.text || changed_cursor != cursor_pos)
		{
			if (added_text != text.text)
			{
				text.SetText(added_text);
				if (listener)
					listener->OnEvent(this, EVENT_INPUT_CHANGE);
			}


			cursor_pos = changed_cursor;
			if (cursor_pos > 0)
			{
				
				std::string selection;
				selection.resize(added_text.size() * 2);
			
				selection = added_text.substr(0, cursor_pos);
				
				app->font->CalcSize(selection.c_str(), cursor.x, cursor.y);
			}
			else
			{
				cursor.x = 0;
			}
		}
	}

}

//GuiLabel Functions
void GuiLabel::SetText(std::string t)
{
	if (tex)
		SDL_DestroyTexture(tex);

	text = t.c_str();
	tex = app->font->Print(text.c_str(), color);
	
	uint w, h;
	app->tex->getSize(tex, w, h);
	SetSize(w, h);
}

//GuiElement Functions
iPoint GuiElement::getLocalPosition()
{
	return{ local_rect.x, local_rect.y };
}

iPoint GuiElement::getScreenPosition()
{
	if (parent)
	{
		iPoint ret;
		ret.x = local_rect.x + parent->getScreenPosition().x;
		ret.y = local_rect.y + parent->getScreenPosition().y;
		return ret;
	}

	return{ local_rect.x, local_rect.y };
}

void GuiElement::SetLabel(GuiElement* label)
{
	descriptionlabel = label;
	descriptionlabel->Desactivate();
}
SDL_Rect GuiElement::getScreenRect()
{
	return{ getScreenPosition().x, getScreenPosition().y, local_rect.w, local_rect.h };
}
SDL_Rect GuiElement::getLocalRect()
{
	return local_rect;
}

void GuiElement::SetLocalPosition(iPoint p)
{
	local_rect.x = p.x;
	local_rect.y = p.y;
}

void GuiElement::Center(bool x, bool y)
{
	if (x || y)
	{
		int fw, fh;
		if (parent)
		{
			fw = parent->getLocalRect().w;
			fh = parent->getLocalRect().h;
		}
		else
		{
			fw = app->render->camera.w;
			fh = app->render->camera.h;
		}
		int w, h;
		SDL_Rect rect = getLocalRect();
		if (x)
			w = fw / 2 - rect.w / 2;
		else
			w = rect.x;

		if (y)
			h = fh / 2 - rect.h / 2;
		else
			h = rect.y;

		SetLocalPosition({ w, h });
	}
}

bool GuiElement::CheckCollision(iPoint p)
{
	bool ret = false;
	if (p.x >  getScreenPosition().x &&
		p.x < (getScreenPosition().x + local_rect.w) &&
		p.y >  getScreenPosition().y &&
		p.y < (getScreenPosition().y + local_rect.h))
	{
		ret = true;
	}
	return ret;
}

bool GuiElement::CheckEvent(GuiElement* hover, GuiElement* focus, bool& interaction)
{
	bool inside = (hover == this);
	bool focused = (focus == this);
	

		if (inside != mouseIn)
		{
			if (listener)
			{
				if (inside)
				{
					listener->OnEvent(this, EVENT_MOUSE_ENTER);
				}
				else
					listener->OnEvent(this, EVENT_MOUSE_EXIT);
			}
			mouseIn = inside;
		}
		
		if (inside)
		{
			interaction = true;

			if (listener)
			{
				if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_DOWN);
				if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_UP);
				if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
					listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_DOWN);
				if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
					listener->OnEvent(this, EVENT_MOUSE_RIGHTCLICK_UP);
			}

			if (draggable && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				iPoint p = getLocalPosition();
				iPoint m = app->input->getMouseMotion();
				SetLocalPosition(p + m);
			}
		}

		if (focusIn != focused)
		{
			if (listener)
			{
				if (focused)
					listener->OnEvent(this, EVENT_FOCUS_DOWN);
				else
					listener->OnEvent(this, EVENT_FOCUS_UP);
			}
			focusIn = focused;
		}

		if (focused)
		{
			if (listener)
			{
				if(app->input->getKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_DOWN);
				if (app->input->getKey(SDL_SCANCODE_RETURN) == KEY_UP)
					listener->OnEvent(this, EVENT_MOUSE_LEFTCLICK_UP);
			}
		}
	
	return true;
}

void GuiElement::addChild(GuiElement* child)
{
	childs.push_back(child);
}

void GuiElement::removeChild(GuiElement* child)
{
	childs.remove(child);
	if (child->parent == this)
		child->parent = NULL;
}

void GuiElement::Activate()
{
	if (active){
		active = true;
		for (list<GuiElement*>::iterator item = childs.begin(); item != childs.end(); item++)
		{
			item._Ptr->_Myval->Activate();
		}
	}
}

void GuiElement::ActivateChilds()
{
	if (active){
	for (list<GuiElement*>::iterator item = childs.begin(); item != childs.end(); item++)
	{
		item._Ptr->_Myval->active = true;
	}
	}
}
void GuiElement::DesactivateChilds()
{
	for (list<GuiElement*>::iterator item = childs.begin(); item != childs.end(); item++)
	{
		item._Ptr->_Myval->active = false;
	}
}



void GuiElement::Desactivate()
{
	if (active){
		active = false;
		for (list<GuiElement*>::iterator item = childs.begin(); item != childs.end(); item++)
		{
			item._Ptr->_Myval->Desactivate();
		}
	}
}

void GuiElement::drawDebug()
{
	SDL_Rect rect = getScreenRect();
	rect.x -= app->render->camera.x;
	rect.y -= app->render->camera.y;

	app->render->DrawQuad(rect, 255, 0, 0, 1000, false);
}


GuiSlider::GuiSlider(iPoint p, SDL_Rect tex_1, SDL_Rect tex_2, int w, int thumb_h, iPoint offset, float value, GuiElement* par, Module* list)
: GuiElement(p, GUI_SLIDER, par, list), image({ offset.x, offset.y }, tex_1, this, NULL), thumb({ p.x + offset.x, thumb_h }, tex_2, this, NULL), width(w), thumb_pos(0), max_value(value)
{
	SetLocalRect({ p.x, p.y, width, tex_1.h });
	value = 0;
}

void GuiSlider::draw()
{
	image.draw();
	thumb.draw();
}

void GuiSlider::update(GuiElement* hover, GuiElement* focus)
{
	bool focused = (focus == this);
	
	int last_thumb = thumb_pos;
	

	if (thumb.CheckCollision(app->input->getMousePosition()) && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
			iPoint m = app->input->getMouseMotion();
			thumb_pos += m.x;
	}

	if (focused)
	{
		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			thumb_pos--;

		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			thumb_pos++;
	}

	
	if (clicking)
	{
		if (thumb.CheckCollision(app->input->getMousePosition()) == false)
		{
			if (app->input->getMousePosition().x > thumb.getScreenPosition().x)
				thumb_pos++;
			else
				thumb_pos--;
		}
	}

	if (last_thumb != thumb_pos)
	{
		if (thumb_pos < 0 || thumb_pos > width)
		{
			if (thumb_pos < 0)
				thumb_pos = 0;
			else
				thumb_pos = width;
		}
		thumb.SetLocalPosition({ thumb_pos + image.getLocalPosition().x, thumb.getLocalPosition().y });

		listener->OnEvent(this, EVENT_SLIDER_CHANGE);
	}
}


float GuiSlider::getValue()const
{
	float dvalue = max_value / width;
	return thumb_pos*dvalue;
}


Fader::Fader(iPoint p, SDL_Rect r, GuiElement* par, Module* list, SDL_Texture* tex, int alpha)
	: GuiElement(p, r, GUI_IMAGE, par, list), tex(tex), alpha(alpha)
{}

void Fader::setAlpha(int a)
{
	if (a >= 0 && a <= 255)
	{
		alpha = a;
	}
}

void Fader::draw()
{
	iPoint p = getScreenPosition();

	app->render->Blit(tex,
		p.x - app->render->camera.x,
		p.y - app->render->camera.y,
		&tex_rect, { 255, 255, 255, alpha });
}

//GUI ANIM

GuiAnimation::GuiAnimation(iPoint p, SDL_Rect r, GuiElement* par, Module* list, SDL_Texture* tex, Animation* anim)
	: GuiElement(p, r, GUI_IMAGE, par, list), tex(tex), anim(anim)
{}

void GuiAnimation::draw()
{
	iPoint p = getScreenPosition();

	app->render->Blit(tex,
		p.x - app->render->camera.x,
		p.y - app->render->camera.y,
		&anim->getCurrentFrame());
}