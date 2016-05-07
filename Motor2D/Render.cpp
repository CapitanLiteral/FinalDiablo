
#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Window.h"
#include "Render.h"


#define VSYNC true

Render::Render() : Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screen_surface->w;
		camera.h = app->win->screen_surface->h;
		camera.x = INIT_POS_X;
		camera.y = INIT_POS_Y;
	}

	return ret;
}

// Called before the first frame
bool Render::start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	//NOTE: My camera cooling, very simple and works, buts ask ric about if this is cheating
	SetViewPort({ 0, 0, camera.w, camera.h });

	for (int i = 0; i < int(FRONT); i++)
	{
		std::multimap<int, Sprite*> map;
		std::pair<spriteLayer, std::multimap<int, Sprite*>> pair = { spriteLayer(i), map };
	}

	return true;
}

// Called each loop iteration
bool Render::preUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}
// NOTE: Function of C, compare Sprites to prepare sort
bool compare_sprites(const Sprite* first, const Sprite* second)
{
	/*if ((first->y > second->y) /*&& (first->position_map.y > second->position_map.y / 2))
	{
		return (first->position_map.y > second->position_map.y);
	}
	/*else if ((first->y < second->y) /*&& (first->position_map.y / 2  > second->position_map.y))
	{
		return (first->position_map.y < second->position_map.y);
	}*/
	
	if (first->y < second->y)
	{
		return true;
	}
	else if (first->y > second->y)
	{
		return false;
	}
}
bool Render::update(float dt)
{
	// NOTE:iterate list sprites call function sort and blit
	
	//insert sort function here
	sprites.sort(compare_sprites);

	iterator = sprites.begin();
	for (iterator; iterator != sprites.end(); iterator++)
	{
		DrawSprite(*iterator);
	}


	return true;
}


bool Render::postUpdate()
{
	bool ret = true;
	/*
	// draw each layer
	std::map<spriteLayer, std::multimap<int, Sprite*>>::iterator layer;
	for (layer = spriteMap.begin(); layer != spriteMap.end(); layer++)
	{
		// blit sprites
		std::multimap<int, Sprite* > ::iterator sprite;
		for (sprite = layer->second.begin(); sprite != layer->second.end(); sprite++)
		{
			DrawSprite(sprite->second);
		}

		layer->second.clear();
	}

	//sprites.clear();

	// blit deebug figures
	std::multimap<int, SimpleGeoFigure*>::iterator figure;
	for (figure = debugFigures.begin(); figure != debugFigures.end(); figure++)
	{
		drawFigure(*figure->second);
	}
	debugFigures.clear();*/

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	return ret;
}

// Called before quitting
bool Render::cleanUp()
{
	// erase list Sprites
	/*list<Sprite*>::iterator item = sprites.begin();
	for (; item != sprites.end(); item++)
	{
		RELEASE((*item));
	}*/
	sprites.clear();

	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = app->win->getScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

iPoint Render::WorldToScreen(int x, int y) const
{
	iPoint ret;
	int scale = app->win->getScale();

	ret.x = (x + camera.x / scale);
	ret.y = (y - camera.x / scale);

	return ret;
}

void Render::CenterCamera(int x, int y)
{
	uint win_x;
	uint win_y;
	app->win->getWindowSize(win_x, win_y);

	camera.x = (win_x / 2) - x;
	camera.y = (win_y / 2) - y;
}

bool Render::completeBlit(SDL_Texture* texture, int x, int y, const SDL_Rect section, SDL_Color tint, double angle, int pivot_x, int pivot_y, SDL_RendererFlip flip) const
{
	bool ret = true;

	if (!(ret = (texture != NULL)))
	{
		return ret;
	}

	// get correct measures to render
	SDL_Rect rect = { x, y, 0, 0 };

	if (section.h > 0 && section.w > 0)
	{
		if (ret = (section.w >= 0 && section.h >= 0))
		{
			rect.w = section.w;
			rect.h = section.h;
		}
		else
		{
			return ret;
		}
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	// scale
	uint scale = app->win->getScale();
	rect.x *= scale;
	rect.y *= scale;
	rect.w *= scale;
	rect.h *= scale;

	// set pivot offset
	SDL_Point* p = NULL;
	SDL_Point pivot;
	if (pivot_x != 0 && pivot_y != 0)
	{
		pivot.x = pivot_x * scale;
		pivot.y = pivot_y * scale;
		p = &pivot;
	}
	
	// check if inside camera's view
	if (!(ret = isInsideCameraZone({ rect.x, rect.y, rect.w, rect.h })))
	{
		return ret;
	}

	// apply tint
	if (!(ret = (SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b) == 0)))
	{
		LOG("Invalid Texture color modulation");
		return ret;
	}

	if (tint.a != 0)
	{
		if (!(ret = (SDL_SetTextureAlphaMod(texture, tint.a) == 0)))
		{
			LOG("Invalid Texture alpha modulation");
			return ret;
		}
	}

	if (section.h > 0 && section.w > 0)
	{
		if (!(ret = (SDL_RenderCopyEx(renderer, texture, &section, &rect, angle, p, flip) == 0)))
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		}
	}
	else
	{
		if (!(ret = (SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, p, flip) == 0)))
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		}
	}


	return ret;
}

bool Render::isInsideCameraZone(SDL_Rect rect)const
{
	return (rect.x + rect.w >= camera.x &&
		rect.x <= camera.x + camera.w &&
		rect.y + rect.h >= camera.y &&
		rect.y <= camera.y + camera.h);
}


// Blit to screen
bool Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_Color tint, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	//Camera culling
	if (section)
		if (!(x + section->w > -camera.x && x < -camera.x + camera.w &&
			y + section->h > -camera.y && y < -camera.y + camera.h))
			return false;
	//
		uint scale = app->win->getScale();

		SDL_Rect rect;
		rect.x = (int)(camera.x * speed) + x * scale;
		rect.y = (int)(camera.y * speed) + y * scale;

		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}

		rect.w *= scale;
		rect.h *= scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivot_x != INT_MAX && pivot_y != INT_MAX)
		{
			pivot.x = pivot_x;
			pivot.y = pivot_y;
			p = &pivot;
		}
		/*
		// apply tint
		if (!(ret = (SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b) == 0)))
		{
			LOG("Invalid Texture color modulation");
			return ret;
		}*/

		if (tint.a != 0)
		{
			if (!(ret = (SDL_SetTextureAlphaMod(texture, tint.a) == 0)))
			{
				LOG("Invalid Texture alpha modulation");
				return ret;
			}
		}

		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	

	return ret;
}

bool Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	
	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	if (use_camera)
	{
		x *= scale;
		y *= scale;
		x += camera.x;
		y += camera.y;
	}

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor) * scale);
		points[i].y = (int)(y + radius * sin(i * factor) * scale);
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw circle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
//NOTE: Sprites functions

//NOTE: add Sprite to the list of Sprites
bool Render::addSpriteToList(Sprite* sprite)
{
	bool ret = true;
	
	if (sprite != NULL)
	{
		sprites.push_back(sprite);
	}
	/*
	if (ret = (sprite != NULL))
	{
		if (ret = (sprite->texture != NULL))
		{
			std::pair<int, Sprite*> pair = { sprite->y, sprite };

			try // check for out-of-range throw
			{
				spriteMap.at(sprite->layer).insert(pair);
			}
			catch (const std::out_of_range& oor)
			{
				std::multimap<int, Sprite*> map;
				map.insert(pair);
				std::pair<spriteLayer, std::multimap<int, Sprite*>> layer = { sprite->layer, map };
				spriteMap.insert(layer);
			}
		}
		else
		{
			LOG("Render: Error, invalid texture to blit.");
		}
	}
	else
	{
		LOG("Render: Error, invalid sprite to blit.");
	}*/

	return ret;
}
//Sprite
bool Render::DrawSprite(Sprite* sprite, float speed, double angle, int pivot_x, int pivot_y)
{
	bool ret = true;

	Blit(sprite->texture, sprite->position_map.x - sprite->pivot.x, sprite->position_map.y - sprite->pivot.y, &(sprite->section_texture), sprite->tint);

	return ret;

}
bool Render::SortSprites()
{
	bool ret = true;
	
	return ret;
}


//Constructors of sprites
Sprite::Sprite() : texture(NULL), position_map({ 0, 0 }), section_texture({ 0, 0, 0, 0 }), tint({ 255, 255, 255, 0 }),
	flip(SDL_FLIP_NONE), layer(SCENE), angle(0)
{}

Sprite::Sprite(SDL_Texture* texture, iPoint position, SDL_Rect section, spriteLayer layer,
	SDL_Color tint, iPoint pivot, double angle, SDL_RendererFlip flip) :
	texture(texture), position_map(position), pivot(pivot), section_texture(section), tint(tint),
	flip(flip), layer(layer), angle(angle)
{}

Sprite::Sprite(SDL_Texture* tex, iPoint& pMap, iPoint& piv, SDL_Rect& tex_section) : tint({ 255, 255, 255, 0 }),
	flip(SDL_FLIP_NONE), layer(SCENE), angle(0)
{
	texture = tex;
	position_map = pMap;
	section_texture = tex_section;
	pivot = piv;
}

Sprite::~Sprite()
{
	//LOG("Destructor");
	SDL_DestroyTexture(texture);

}

void Sprite::updateSprite(SDL_Texture* tex, iPoint& p, iPoint& piv, SDL_Rect& section)
{
	texture = tex;
	position_map = p;
	section_texture = section;
	pivot = piv;
	y = p.y;
}

void Sprite::updateSprite(iPoint& p, iPoint& piv, SDL_Rect& section)
{
	position_map = p;
	section_texture = section;
	pivot = piv;
	y = p.y;
}

//void Sprite::DrawSprite()
void Sprite::setAlpha(int alpha)
{
	if (alpha >= 0 && alpha <= 255)
	{
		tint.a = alpha;
	}
}


bool Render::drawFigure(SimpleGeoFigure figure)
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, figure.color.r, figure.color.g, figure.color.b, figure.color.a);

	switch (figure.type)
	{
	case RECTANGLE:
	{
		SDL_Rect rect;
		rect.x = figure.x;
		rect.y = figure.y;
		rect.w = figure.getCoordX();
		rect.h = figure.getCoordY();

		if (figure.useCamera)
		{
			rect.x += camera.x;
			rect.y += camera.y;
		}

		//scale
		rect.x *= scale;
		rect.y *= scale;
		rect.w *= scale;
		rect.h *= scale;

		ret = (((figure.isFilled()) ? SDL_RenderFillRect(renderer, &rect) : SDL_RenderDrawRect(renderer, &rect)) != 0);

		break;
	}
	case CIRCLE:
	{
		SDL_Point points[360];

		float factor = (float)M_PI / 180.0f;

		for (uint i = 0; i < 360; ++i)
		{
			points[i].x = figure.x + figure.getCoordX();
			points[i].x = int(points[i].x * cos(i * factor));

			points[i].y = figure.y + figure.getCoordY();
			points[i].y = int(points[i].y * sin(i * factor));
		}

		ret = (SDL_RenderDrawPoints(renderer, points, 360) == 0);

		break;
	}
	case LINE:
	{
		SDL_Rect rect;
		rect.x = figure.x;
		rect.y = figure.y;
		rect.w = figure.getCoordX();
		rect.h = figure.getCoordY();

		if (figure.useCamera)
		{
			rect.x += camera.x;
			rect.y += camera.y;
		}

		//scale
		rect.x *= scale;
		rect.y *= scale;
		rect.w *= scale;
		rect.h *= scale;

		ret = (SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.w, rect.h) == 0);

		break;
	}
	default:
	{
		SDL_Rect rect;
		rect.x = figure.x;
		rect.y = figure.y;
		rect.w = figure.getCoordX();
		rect.h = figure.getCoordY();

		if (figure.useCamera)
		{
			rect.x += camera.x;
			rect.y += camera.y;
		}

		//scale
		rect.x *= scale;
		rect.y *= scale;
		rect.w *= scale;
		rect.h *= scale;

		ret = (((figure.isFilled()) ? SDL_RenderFillRect(renderer, &rect) : SDL_RenderDrawRect(renderer, &rect)) != 0);
	}
	}

	if (!ret)
	{
		LOG("Cannot draw figure to screen. SDL_RenderFillRect error: %s", SDL_GetError());
	}

	return ret;
}
