#ifndef __GUIELEMENTS_H__
#define __GUIELEMENTS_H__

#include "Module.h"
#include "p2Point.h"
#include "Animation.h"

#include <list>
#include <string>
;
using namespace std;
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct _TTF_Font;
enum TextColor;
enum GUI_Type
{
	GUI_LABEL,
	GUI_IMAGE,
	GUI_BUTTON,
	GUI_INPUTBOX,
	GUI_SLIDER,
	GUI_MOUSE_IMAGE,
	GUI_ITEM,
	GUI_INVENTORY,
	GUI_SLOT
};

enum GUI_Event
{
	EVENT_MOUSE_LEFTCLICK_DOWN,
	EVENT_MOUSE_LEFTCLICK_UP,
	EVENT_MOUSE_RIGHTCLICK_DOWN,
	EVENT_MOUSE_RIGHTCLICK_UP,
	EVENT_MOUSE_ENTER,
	EVENT_MOUSE_EXIT,
	EVENT_FOCUS_DOWN,
	EVENT_FOCUS_UP,
	EVENT_INPUT_CHANGE,
	EVENT_SLIDER_CHANGE,
};

class GuiElement
{
public:
	GuiElement(iPoint p, GUI_Type t, GuiElement* par, Module* list);
	GuiElement(iPoint p, SDL_Rect r, GUI_Type t, GuiElement* par, Module* list);
	~GuiElement(){}

	virtual void draw(){}
	virtual void update(GuiElement* hover, GuiElement* focus){}

	//Utils
	bool CheckCollision(iPoint p);
	bool CheckEvent(GuiElement* hover, GuiElement* focus, bool& interaction);
	void drawDebug();
	void Center(bool x, bool y);

	//Activate and desactivate
	//NOTE: a bit useles...
	void Desactivate();
	void Activate();
	void ActivateChilds();
	void DesactivateChilds();
	//
	//getters
	iPoint getLocalPosition();
	iPoint getScreenPosition();
	SDL_Rect getScreenRect();
	SDL_Rect getLocalRect();
	SDL_Rect getTextureRect(){ return tex_rect; };
	
	void SetLocalPosition(iPoint p);
	void SetSize(int w, int h){ local_rect.w = w, local_rect.h = h; }
	void SetTextureRect(SDL_Rect r){ tex_rect = r; }
	void SetLocalRect(SDL_Rect r){ local_rect = r; }
	void SetLabel(GuiElement* label);
	void addChild(GuiElement* child);
	void removeChild(GuiElement* child);

public:
	GUI_Type	 type;
	GuiElement*  parent;
	GuiElement* descriptionlabel;
	list<GuiElement*> childs;

	bool         debug = false;
	bool		 active = true;
	bool         visible = true;
	bool		 interactable = false;
	bool		 draggable = false;
	bool		 focusable = false;
	bool		 mouseIn;
	bool		 focusIn;
	bool		 mask;
	Module*    listener;
	SDL_Rect     tex_rect;
	TextColor color;
private:
	SDL_Rect     local_rect;
};

class GuiLabel : public GuiElement
{
public:
	GuiLabel(std::string t, _TTF_Font* f, iPoint p, TextColor color, GuiElement* par, Module* list);
	~GuiLabel(){}

	void draw();
	void update(GuiElement* hover, GuiElement* focus){}
	void SetText(std::string t);

public:
	std::string text;
	_TTF_Font* font;
	SDL_Texture* tex;
	
};

class GuiImage : public GuiElement
{
public:
	GuiImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list = NULL);
	~GuiImage(){};

	void draw();
	void update(GuiElement* hover, GuiElement* focus){}
};

class Fader : public GuiElement
{
public:

	Fader(iPoint p, SDL_Rect r, GuiElement* par, Module* list = NULL, SDL_Texture* tex = NULL,  int alpha = 0);
	~Fader(){};

	void draw();
	void setAlpha(int alpha = 0);

	SDL_Texture* tex = NULL;
	int alpha = 0;
};

class GuiAnimation : public GuiElement
{
public:

	GuiAnimation(iPoint p, SDL_Rect r, GuiElement* par, Module* list = NULL, SDL_Texture* tex = NULL, Animation* anim = NULL);
	~GuiAnimation(){};

	void draw();
	void update(GuiElement* hover, GuiElement* focus){}

	SDL_Texture* tex = NULL;
	Animation* anim = NULL;
};

class GuiInputBox : public GuiElement
{
public:

	GuiInputBox(std::string t, _TTF_Font* f, iPoint p, int width, SDL_Rect r, iPoint offset, GuiElement* par, Module* list);
	~GuiInputBox(){};


	void draw();
	void update(GuiElement* hover, GuiElement* focus);
	//Utils
	bool Input();

public:
	GuiLabel text;
	GuiImage image;

	bool	inputOn;
	bool	init;
	bool	password;
	iPoint	cursor;
	int		cursor_pos;
};

class GuiButton : public GuiElement
{
public:
	GuiButton(iPoint p, SDL_Rect idle_r1, SDL_Rect hover_r1, SDL_Rect click_r1, std::string t, _TTF_Font* f, Module* list = NULL, GuiElement* parent = NULL);
	~GuiButton(){}

	void draw();
	void update(GuiElement* hover, GuiElement* focus){}

public:
	GuiImage button_image;
	GuiLabel button_label;
	SDL_Rect idle_tex;
	SDL_Rect hover_tex;
	SDL_Rect click_tex;

};

class GuiSlider : public GuiElement
{
public:
	GuiSlider(iPoint p, SDL_Rect tex_1, SDL_Rect tex_2, int width, int thumb_h ,iPoint offset, float value, GuiElement* par, Module* list);
	~GuiSlider(){};

	void draw();
	void update(GuiElement* hover, GuiElement* focus);
	float getValue()const;

public:
	GuiImage image;
	GuiImage thumb;

	float	max_value;
	int		width;
	int     thumb_pos;
	bool	clicking = false;
};


class GuiMouseImage : public GuiElement
{
public:
	GuiMouseImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list = NULL);
	~GuiMouseImage(){}

	void draw();
	void update();

public:
	GuiImage mouse_image;
	int mouse_y, mouse_x;
};
//----
#endif _GUIELEMENTS_H_