#include "PlayerSkills.h"
#include "App.h"
#include "Player.h"
#include "Game.h"
#include "EntityManager.h"
#include "entEnemy.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"

/*
//--Basic Attack
*/

//NOTE: provisional
sklBasicAttack::sklBasicAttack() : sklMelee()
{
	//NOTE : Has to be unLoaded somewhere
	skill_tex = app->tex->Load("textures/vamp_attack.png");
}

sklBasicAttack::~sklBasicAttack()
{

}

void sklBasicAttack::skillUpdate()
{
	if (player->current_animation->Finished())
	{
		player->current_input = INPUT_STOP_MOVE;
		player->attacking = false;
		player->input_locked = false;
	}
}

void sklBasicAttack::skillEffect()
{
	player->enemy->TakeDamage(player->atk_damage);
}

void sklBasicAttack::setSkillAnimations()
{
	for (int i = 0; i < 8; i++)
	{
		Animation atk;
		atk.SetFrames(0, (92 + SPRITE_MARGIN) * i, 119, 92, 20, SPRITE_MARGIN);
		atk.speed = 0.4f;
		atk.loop = false;

		skill_animation_set.push_back(atk);
	}
}

void sklBasicAttack::skillInit()
{

}

