#ifndef __RENDER_H__
#define __RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Module.h"
#include <list>
#include <map>

using namespace std;
//NOTE : put this to the config file
#define CAM_SPEED 200.0f
#define INIT_POS_X 280
#define INIT_POS_Y 0

enum spriteLayer
{
	BACKGROUND = 0,
	SCENE,
	MENU,
	FADER,
	UI,
	FRONT
};

struct Sprite
{
	Sprite();
	Sprite(SDL_Texture* texture, iPoint position = { 0, 0 },
		SDL_Rect section = { 0, 0, 0, 0 }, spriteLayer layer = SCENE,
		SDL_Color tint = { 255, 255, 255, 0 }, iPoint pivot = { 0, 0 },
		double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// Outdated Constructor
	Sprite(SDL_Texture* texture, iPoint& pos, iPoint& piv, SDL_Rect& section_texture);
	~Sprite();
	
	void updateSprite(SDL_Texture* tex, iPoint& p, iPoint& piv, SDL_Rect& section);

	void setAlpha(int alpha);

	SDL_Texture*		texture;
	iPoint				position_map;
	iPoint				pivot;
	SDL_Rect			section_texture;
	SDL_Color			tint;
	SDL_RendererFlip	flip;

	int				y;
	double			angle;
	spriteLayer		layer;

};

enum simpleGeoFigureTYPE;
struct SimpleGeoFigure;

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();
	bool update(float dt);
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;
	iPoint WorldToScreen(int x, int y) const;
	void CenterCamera(int x, int y);

	// draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_Color tint = { 255, 255, 255, 0 }, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	
	//blit sprites
	//NOTE: With list
	bool addSpriteToList(Sprite* sprite);
	bool DrawSprite(Sprite* sprite, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool SortSprites();
	// Set background color
	void SetBackgroundColor(SDL_Color color);

	// Previous methods
	bool drawFigure(SimpleGeoFigure figure);
	bool completeBlit(SDL_Texture* texture, int x, int y, const SDL_Rect section = { 0, 0, 0, 0 }, SDL_Color tint = { 255, 255, 255, 0 }, double angle = 0, int pivot_x = 0, int pivot_y = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	bool isInsideCameraZone(SDL_Rect rect)const;

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;

	// Old sprite containers
	list<Sprite*>	sprites;
	list<Sprite*>::iterator iterator;

	std::map<spriteLayer, std::multimap<int, Sprite*>> spriteMap;
	std::multimap<int, SimpleGeoFigure*> debugFigures;
};

enum simpleGeoFigureTYPE
{
	RECTANGLE = 0,
	CIRCLE,
	LINE,
	DOT
};

struct SimpleGeoFigure
{
	SimpleGeoFigure(int x = 0, int y = 0, SDL_Color color = { 255, 255, 255, 100 }, bool useCamera = true, simpleGeoFigureTYPE type = DOT) :
		x(x), y(y), color(color), useCamera(useCamera){}

	virtual int  getCoordX() { return 1; }
	virtual int  getCoordY() { return 1; }
	virtual bool isFilled()  { return true; }

	int x, y;
	int layer;
	simpleGeoFigureTYPE type;
	SDL_Color color;
	bool useCamera;
};

struct Circle : public SimpleGeoFigure
{
	Circle(int radius = 0, int x = 0, int y = 0, SDL_Color color = { 255, 255, 255, 100 }, bool useCamera = true) :
		radius(radius), SimpleGeoFigure(x, y, color, useCamera, CIRCLE) {}

	int getCoordX() { return radius; }
	int getCoordY() { return radius; }

	int radius;
};

struct Rect : public SimpleGeoFigure
{
	Rect(SDL_Rect rect = { 0, 0, 0, 0 }, bool filled = true, SDL_Color color = { 255, 255, 255, 100 }, bool useCamera = true) :
		w(rect.w), h(rect.h), SimpleGeoFigure(rect.x, rect.y, color, useCamera, RECTANGLE) {}

	int getCoordX() { return w; }
	int getCoordY() { return h; }
	bool isFilled() { return filled; }

	int w, h;
	bool filled;
};

struct Line : public SimpleGeoFigure
{
	Line(SDL_Rect rect = { 0, 0, 0, 0 }, bool filled = true, SDL_Color color = { 255, 255, 255, 100 }, bool useCamera = true) :
		destX(rect.w), destY(rect.h), SimpleGeoFigure(rect.x, rect.y, color, useCamera, LINE) {}

	int getCoordX() { return destX; }
	int getCoordY() { return destY; }

	int destX, destY;
};

#endif // __RENDER_H__