#ifndef __ENT_ITEM_H__
#define __ENT_ITEM_H__

#include "EntStatic.h"

class Player;
class EntItem : public EntStatic
{
public:
	EntItem(const iPoint &p, uint ID);

	void draw()
	{}
	void drawDebug()
	{}

public:
};



class itmPotionHP : public EntItem
{
public:
	itmPotionHP(const iPoint &p, uint ID);

	void Effect();
	
	int HP_points = 50;
};
#endif