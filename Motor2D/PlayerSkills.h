#ifndef _PLAYERSKILLS_H_
#define _PLAYERSKILLS_H_

#include "Skill.h"


class sklBasicAttack : public sklMelee
{
public:
	sklBasicAttack();
	~sklBasicAttack();

	void skillEffect();

	void skillInit();
	void skillUpdate();
	void setSkillAnimations();

public:

};
#endif _PLAYERSKILLS_H_