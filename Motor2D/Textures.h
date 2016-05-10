#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"
#include "p2List.h"

#include <list>
using namespace std;

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called before quitting
	bool cleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				unLoad(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);
	void				getSize(const SDL_Texture* texture, uint& width, uint& height) const;



public:

	list<SDL_Texture*>textures;
	
};


#endif // __TEXTURES_H__