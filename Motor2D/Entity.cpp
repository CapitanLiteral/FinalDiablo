#include "Entity.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Collision.h"

//Constructor
Entity::Entity(const iPoint &p, uint ID)
{
	iPoint tmp = app->map->WorldToMap(p.x, p.y);
	//position.x = tmp.x;
	//position.y = tmp.y;
	tmp = app->map->MapToWorld(tmp.x, tmp.y);
	position.x = tmp.x - (app->map->data.tile_width / 2);
	position.y = tmp.y;
	id = ID;
	collider_rect = {0, 0, 0, 0};
	collider = NULL;

}

//Destructor
Entity::~Entity()
{
	SDL_DestroyTexture(tex);
	//Take an eye on this
	if (sprite)
	{
		app->render->sprites.remove(sprite);
		RELEASE(sprite);
	}
}

//getters
iPoint Entity::getMapPosition() const
{
	return app->map->WorldToMap(position.x, position.y);
}

iPoint Entity::getTilePosition() const
{
	iPoint ret = getMapPosition();
	ret = app->map->MapToWorld(ret.x, ret.y);
	ret.x -= app->map->data.tile_width / 2;
	return ret;
}

iPoint Entity::getBlitPosition() const
{
	fPoint tmp = getPivotPosition();
	iPoint ret(tmp.x, tmp.y);
	ret.x -= sprite_pivot.x;
	ret.y -= sprite_pivot.y;

	return ret;
}

fPoint Entity::getPivotPosition() const
{
	//NOTE : put a tile pivot?, more accesible
	return (position);
}

SDL_Rect Entity::getPlayerRect() const
{
	//NOTE: this may be adapted when we use colliders
	fPoint blit = getPivotPosition();

	blit.x -= colliderOffset.x;
	blit.y -= colliderOffset.y;

	return{ blit.x, blit.y, collider_rect.w, collider_rect.h };
}

SDL_Rect Entity::getPlayerCollider()const
{
	if (collider)
	{
		return collider->rect;
	}
	else
		return{ 0, 0, 0, 0 };
}