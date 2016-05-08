#ifndef __SNINTRO_H__
#define __SNINTRO_H__

#include "Scene.h"
#include <list>
#include "Animation.h"

using namespace std;

class GuiImage;
class GuiElement;
class GuiButton;
class GuiAnimation;

class snIntro : public Scene
{
public:
	// Constructor
	snIntro();

	// Destructor
	~snIntro();

	// Called before render is available
	bool awake(pugi::xml_node& conf);

	// Called the first frame
	bool start();

	// preUpdate
	bool preUpdate();

	// update
	bool update(float dt);

	// postUpdate
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Called on certain event
	void OnEvent(GuiElement* element, GUI_Event even);

	//Load/unLoad, called when the scene changes
	bool Load();
	bool unLoad();
	

private:
	
	bool pass;
	bool exit;

	//Ui elements
	GuiButton* play_button = NULL;
	GuiButton* exit_button = NULL;
	GuiImage* background = NULL;
	GuiAnimation* logo = NULL;

	Animation logoAnim;
	SDL_Texture* logoSprite = NULL;
	//NOTE : added this list to iterate easily the gui_elements of the scene. Ask ric about a more optimal way to control the UI
	//NOTE: do the button class so it can have the diferent images and even a label, so it doesn?t interfere with the rest of the ui
	list<GuiElement*> intro_gui;
};

#endif // __SNINTRO_H__

