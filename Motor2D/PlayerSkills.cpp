#include "PlayerSkills.h"
#include "App.h"
#include "Player.h"
#include "Game.h"
#include "EntityManager.h"
#include "entEnemy.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "playerParticle.h" 

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

//Blood Arrow
//NOTE: change this to be aplicable with the system particle
sklBloodArrow::sklBloodArrow() : sklRanged()
{
	skill_tex = app->tex->Load("textures/vamp_cast.png");
}

sklBloodArrow::~sklBloodArrow()
{}

void sklBloodArrow::skillEffect()
{}

void sklBloodArrow::skillInit()
{
	player->particle_destination.x = app->input->getMouseWorldPosition().x;
	player->particle_destination.y = app->input->getMouseWorldPosition().y;
	player->SetDirection(player->particle_destination);
}

void sklBloodArrow::skillUpdate()
{
	//NOTE: provisional
	if (player->current_animation->CurrentFrame() >= 7 && !player->particle_is_casted)
	{
		playerParticle* particle = new playerParticle({ player->p_position.x, player->p_position.y - 40 }, player->particle_destination);
		player->particle_list.push_back(particle);
		player->particle_is_casted = true;
	}

	if (player->current_animation->Finished())
	{
		player->current_input = INPUT_STOP_MOVE;
		player->input_locked = false;
		player->particle_is_casted = false;
	}
}

void sklBloodArrow::setSkillAnimations()
{
	for (int i = 0; i < 12; i++)
	{
		Animation cst;
		cst.SetFrames(0, (92 + SPRITE_MARGIN) * i, 119, 92, 12, SPRITE_MARGIN);
		cst.speed = 0.3f;
		cst.loop = false;

		skill_animation_set.push_back(cst);
	}

}