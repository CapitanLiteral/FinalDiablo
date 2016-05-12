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

	snIntro();
	~snIntro();

	bool start();
	bool update(float dt);

	void OnEvent(GuiElement* element, GUI_Event even);

	bool Load();
	bool unLoad();
	

private:

	bool exit;

	//Ui elements
	GuiButton* playButton = NULL;
	GuiButton* exitButton = NULL;
	GuiImage* background = NULL;
	GuiAnimation* logo = NULL;

	Animation logoAnim;
	SDL_Texture* logoSprite = NULL;
};

#endif // __SNINTRO_H__

