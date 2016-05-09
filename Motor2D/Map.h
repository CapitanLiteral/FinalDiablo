#ifndef __MAP_H__
#define __MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Module.h"

#include <list>
using namespace std;

//WARNING: this has to stay out, i used it only because i wanted to test the the Scene Manager, but then, it didn't detect SDL_Rect and else
#include "SDL/include/SDL.h"
// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		list<Property*>::iterator item;
		item = plist.begin();

		while(item != plist.end())
		{
			RELEASE(*item);
			item++;
		}

		plist.clear();
	}

	int get(const char* name, int default_value = 0) const;

	list<Property*>	plist;
};

// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect getTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	list<TileSet*>		tilesets;
	list<MapLayer*>		layers;
};

// ----------------------------------------------------
class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool awake(pugi::xml_node& conf);

	// Called each loop iteration
	bool preUpdate();

	void draw();

	// Called before quitting
	bool cleanUp();

	// Load new map
	bool Load(const char* path);

	//---------
	//---------Point getter and coordinate system translators
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	//get the tile world coordinates of a random point from the world
	iPoint getTileWorld(int x, int y) const;
	//get the center of the tile in world coordinates from a map coordinate
	iPoint getTileCenter(int x, int y) const;
	//get the blit position of a tile
	iPoint getTileBlit(int x, int y) const;

	//---------
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* getTilesetFromTileId(int id) const;

public:

	MapData data;
	bool debugMap = false;
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __MAP_H__