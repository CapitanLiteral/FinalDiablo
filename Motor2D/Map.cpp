#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "FileSystem.h"
#include "Textures.h"
#include "Map.h"
#include <math.h>
#include "Input.h"

Map::Map() : Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

bool Map::preUpdate()
{
	if (app->debug)
		if (app->input->getKey(SDL_SCANCODE_F2) == KEY_DOWN)
			debugMap = !debugMap;

	if (!app->debug)
		debugMap = false;

	return true;
}

void Map::draw()
{
	if(map_loaded == false)
		return;
	//STL CHANGE
	//NOTE: well
	//Camera Culling
	//----------------------
	SDL_Rect cam = app->render->camera;
	//----------------------

	list<MapLayer*>::iterator item = data.layers.begin();

	for(; item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;


		//NOTE: when drawing navigation map, framerate drops to the half
		if (!app->debug || !debugMap)
			if (!debugMap)
				if (layer->properties.get("Nodraw") != 0)
					continue;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->get(x, y);
				if(tile_id > 0)
				{
					TileSet* tileset = getTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->getTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					//NOTE: Maybe this has to be implemented on Render.cpp
					//NOTE: changing the offset of the tiles because Ric cheated with the original, think about make it general for any map
					//NOTE: because of test sake
					//----------------------

						if (layer->name == "Background")
							app->render->Blit(tileset->texture, pos.x - data.tile_width / 2 + tileset->offset_x, pos.y, &r);
						else if (layer->name == "Navigation")
							app->render->Blit(tileset->texture, pos.x - data.tile_width / 2 , pos.y, &r);
						
					
					//----------------------
				}
			}
		}
	}
	
}

int Properties::get(const char* value, int default_value) const
{
	list<Property*>::const_iterator item = plist.begin();

	while(item != plist.end())
	{
		if((*item)->name == value)
			return (*item)->value;
		item++;
	}

	return default_value;
}


TileSet* Map::getTilesetFromTileId(int id) const
{
	//STL CHANGE
	list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* set = (*item);

	while(item != data.tilesets.end())
	{
		if(id < (*item)->firstgid)
		{
			
			//Well, this is more dificult to get, but is to navigate through list withough changing value of the iterator
			//Ask Ric
			set = item._Ptr->_Prev->_Myval;
			break;
		}
		set = *item;
		item++;
	}

	return set;
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2);
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

//Translates any screen position to the tile position (in world coordinates) that it belongs
iPoint Map::getTileWorld(int x, int y) const
{
	// NOTE : this maybe can be better applied anywhere else
	iPoint ret(x, y);

	ret = WorldToMap(ret.x, ret.y);
	ret = MapToWorld(ret.x, ret.y);

	return ret;
}

//gets the World coordinate of the center of the tile from the Map coordinate of that tile
iPoint Map::getTileCenter(int x, int y) const
{
	iPoint ret(x, y);

	ret = MapToWorld(ret.x, ret.y);
	
	ret.y += data.tile_height/2;

	return ret;
}

//Gives the blit position of a tile from the map coordinates of the tile
iPoint Map::getTileBlit(int x, int y)const
{
	iPoint ret(x, y);

	ret = MapToWorld(ret.x, ret.y);
	ret.x -= data.tile_width / 2;
	return ret;

}

SDL_Rect TileSet::getTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool Map::cleanUp()
{
	LOG("unLoading map");

	//STL CHANGE
	// remove all tilesets
	list<TileSet*>::iterator item;
	item = data.tilesets.begin();

	while(item != data.tilesets.end())
	{
		SDL_DestroyTexture(item._Ptr->_Myval->texture);
		RELEASE(*item);
		item++;
	}
	data.tilesets.clear();

	//STL CHANGE
	// remove all layers
	list<MapLayer*>::iterator item2;
	item2 = data.layers.begin();

	while(item2 != data.layers.end())
	{
		RELEASE(*item2);
		item2++;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.getString(), file_name);

	char* buf;
	int size = app->fs->Load(tmp.getString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		//STL CHANGE
		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);
		//STL CHANGE
		if(ret == true)
			data.layers.push_back(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);
		//STL CHANGE
		list<TileSet*>::iterator item = data.tilesets.begin();
		while(item != data.tilesets.end())
		{
			TileSet* s = *item;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.getString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item++;
		}
		//STL CHANGE
		list<MapLayer*>::iterator item_layer = data.layers.begin();
		while(item_layer != data.layers.end())
		{
			MapLayer* l = *item_layer;
			LOG("Layer ----");
			LOG("name: %s", l->name.getString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer++;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.getString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.getString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.getString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.getString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.plist.push_back(p);
		}
	}

	return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	//STL CHANGE
	bool ret = false;
	list<MapLayer*>::const_iterator item;
	item = data.layers.begin();

	for(item = data.layers.begin(); item != data.layers.end(); item++)
	{
		MapLayer* layer = *item;

		if(layer->properties.get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->get(x, y);
				TileSet* tileset = (tile_id > 0) ? getTilesetFromTileId(tile_id) : NULL;
				
				if(tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
				}
				else
				{
					map[i] = 0;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}