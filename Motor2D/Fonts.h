#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#include <list>
using namespace std;

#define DEFAULT_FONT "fonts/open_sans/DiabloII.TTF"
#define DEFAULT_FONT_SIZE 19

struct SDL_Texture;
struct _TTF_Font;

enum TextColor
{
	FONT_BLACK,
	FONT_RED,
	FONT_GREEN,
	FONT_BLUE,
	FONT_YELLOW,
	FONT_WHITE
};


class Fonts : public Module
{
public:

	Fonts();

	// Destructor
	virtual ~Fonts();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before quitting
	bool cleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = {0, 0, 0, 0}, _TTF_Font* font = NULL);
	SDL_Texture* Print(const char* text, TextColor color, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	list<_TTF_Font*>	fonts;
	_TTF_Font*			default;
	_TTF_Font*			description;
};


#endif // __FONTS_H__