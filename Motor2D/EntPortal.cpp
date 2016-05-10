#include "EntPortal.h"
#include "Player.h"
#include "App.h"
#include "Game.h"
#include "Textures.h"
#include "Gui.h"
#include "Render.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Collision.h"

EntPortal::EntPortal(const iPoint &p, uint ID) : EntStatic(p, ID)
{
	type = PORTAL;
	tex = app->game->em->portal_tex;
	SetAnimation();

	//Sprite creation

	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	SDL_Rect current_sprite = sprite_rect;
	sprite = new Sprite(tex, pos, sprite_pivot, current_sprite);
	app->render->addSpriteToList(sprite);

	collider = app->collision->addCollider(getPlayerRect(), COLLIDER_PLAYER, app->game->em);
}

EntPortal::~EntPortal()
{

}

void EntPortal::SetAnimation()
{
	sprite_rect = collider_rect = { 0, 0, 78, 130 };

	sprite_pivot = { collider_rect.w / 2, collider_rect.h - 20};

	//NOTE: Set Particle Animation Here

}

//Drawing methods
void EntPortal::draw()
{
	//iPoint pos = getBlitPosition();

	if (sprite)
	{
		fPoint p = getPivotPosition();
		iPoint pos(p.x, p.y);
		sprite->updateSprite(tex, pos, sprite_pivot, sprite_rect);
	}

	//NOTE: draw Particle Animation here

}

void EntPortal::drawDebug()
{
	iPoint t_pos = getTilePosition();
	fPoint p_pos = getPivotPosition();
	SDL_Rect rect = getPlayerRect();

	app->render->DrawQuad({ p_pos.x, p_pos.y, 3, 3 }, 255, 0, 0, 255, false);

	app->render->DrawQuad(rect, 255, 0, 0, 255, false);
}

void EntPortal::SetDestiny(Scene* dest)
{
	destiny = dest;
}