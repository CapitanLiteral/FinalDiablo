#include "playerParticle.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Game.h"

//NOTE: Everything is for testing purposes, must be changed

playerParticle::playerParticle()
{
	
}

playerParticle::playerParticle(fPoint init, fPoint end)
{
	particle_rect.x = init.x;
	particle_rect.y = init.y;
	particle_rect.w = 5;
	particle_rect.h = 5;

	destination.x = end.x;
	destination.y = end.y;

	speed = 5;

	SetInitVelocity();
}

playerParticle::~playerParticle()
{

}

bool playerParticle::update(float dt)
{
	particle_rect.x += velocity.x;
	particle_rect.y += velocity.y;
	draw();

	return true;
}

void playerParticle::draw()
{
	app->render->DrawQuad(particle_rect, 255, 0, 0, 255, true, true);
}

void playerParticle::SetInitVelocity()
{
	iPoint target = app->input->getMouseWorldPosition();

	velocity.x = target.x - particle_rect.x;
	velocity.y = target.y - particle_rect.y;

	velocity.SetModule(speed);
}



