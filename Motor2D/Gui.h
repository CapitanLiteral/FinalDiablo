#ifndef __GUI_H__
#define __GUI_H__

#include "Module.h"
//I'm doing trap here...
//So i can take this out i have to change the constructors..., i'll do it later
#include "GuiElements.h"
#include "GuiInventory.h"
#include <list>


#define CURSOR_WIDTH 2

// ---------------------------------------------------
;
class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Call before first frame
	bool start();

	// Called before all updates
	bool preUpdate();

	// Called after all updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();



	// Gui creation functions
	GuiImage* addGuiImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list);
	Fader* addFader(iPoint p, SDL_Rect r, GuiElement* par, Module* list, SDL_Texture* tex = NULL, int alpha = 0);
	GuiAnimation* addGuiAnimation(iPoint p, SDL_Rect r, GuiElement* par, Module* list, SDL_Texture* tex = NULL, Animation* anim = NULL);
	GuiImage* addGuiImageWithLabel(iPoint p, SDL_Rect r, p2SString t, _TTF_Font* f, iPoint i, GuiElement* par, Module* list);
	GuiLabel* addGuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, TextColor color, Module* list);
	GuiLabel* addGuiLabel(p2SString t, _TTF_Font* f, iPoint p, GuiElement* par, Module* list);
	GuiInputBox* addGuiInputBox(p2SString t, _TTF_Font* f, iPoint p, int width, SDL_Rect r, iPoint offset, GuiElement* par, Module* list);
	GuiButton* addGuiButton(iPoint p, SDL_Rect idle_r1, SDL_Rect hover_r1, SDL_Rect click_r1, p2SString t = "", _TTF_Font* f = NULL, Module* list = NULL, GuiElement* parent = NULL);
	GuiSlider* addGuiSlider(iPoint p, SDL_Rect tex_1, SDL_Rect tex_2, int width, int thumb_h, iPoint offset, float value, GuiElement* par, Module* list);
	GuiLabel* SetLabel();
	GuiMouseImage* addGuiMouseImage(iPoint p, SDL_Rect r, GuiElement* par, Module* list);

	//Function that creates an inventory
	GuiInventory* addGuiInventory(iPoint p, SDL_Rect r, int columns, int rows, int slot_w, int slot_h, GuiElement* par = NULL, Module* list = NULL);

	//get selected element
	GuiElement* FindSelectedElement();

	SDL_Texture* getAtlas() const;

public:

	list<GuiElement*> gui_elements;
	bool			mouse_hovering;
	GuiItem*		dragged_item;

private:
	bool			debug;
	bool            mouse_clicked;
	SDL_Texture*	atlas;
	p2SString		atlas_file_name;
	int             mouse_x, mouse_y;

	GuiElement*		focus;
	GuiMouseImage* mouse = NULL;
};

#endif // __GUI_H__