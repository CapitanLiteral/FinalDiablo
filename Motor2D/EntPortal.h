#ifndef __ENT_PORTAL_H__
#define __ENT_PORTAL_H__

#include "EntStatic.h"

class Scene;

class EntPortal : public EntStatic
{
public:
	EntPortal(const iPoint &p, uint ID);
	~EntPortal();
	void SetAnimation();

	void draw();
	void drawDebug();

	void Teleport();
	void SetDestiny(Scene* dest);
public:
	Scene* destiny;
};

#endif