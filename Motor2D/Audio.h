#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include <list>
using namespace std;

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	float volume = 50.0f;

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool awake(pugi::xml_node&);

	//Update
	bool update(float dt);

	// Called before quitting
	bool cleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	_Mix_Music*			music = NULL;
	list<Mix_Chunk*>	fx;
     

};

#endif // __AUDIO_H__