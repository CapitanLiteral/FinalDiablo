#include "EntItem.h"
#include "Player.h"
#include "App.h"
#include "Game.h"
#include "Textures.h"
#include "Gui.h"
#include "Render.h"

EntItem::EntItem(const iPoint &p, uint ID) : EntStatic(p, ID)
{

}

itmPotionHP::itmPotionHP(const iPoint &p, uint ID) : EntItem(p, ID)
{
	type = ITEM_HEALTH;
	//NOTE:
	tex = app->gui->getAtlas();
	sprite_rect = collider_rect = { 0, 0, 29, 29 };
	sprite_pivot = pivot = { sprite_rect.w, sprite_rect.h };

	fPoint po = getPivotPosition();
	iPoint pos(po.x, po.y);
	SDL_Rect section = { 2285, 799, 29, 29 };
	sprite = new Sprite(tex, pos, sprite_pivot, section);
	app->render->addSpriteToList(sprite);
}

//NOTE: this is totally, absolutely provisional
void itmPotionHP::Effect()
{
}