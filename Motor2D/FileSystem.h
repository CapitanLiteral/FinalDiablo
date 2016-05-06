#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

class FileSystem : public Module
{
public:

	FileSystem();

	// Destructor
	virtual ~FileSystem();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before quitting
	bool cleanUp();

	// Utility functions
	bool addPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	const char* getSaveDirectory() const
	{
		return "save/";
	}

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;

private:

};

#endif // __FILESYSTEM_H__