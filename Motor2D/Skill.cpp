#include "Skill.h"
#include "App.h"
#include "Game.h"
#include "Player.h"

Skill::Skill(SKILL_TYPE t) : skill_type(t), player(app->game->player)
{}
