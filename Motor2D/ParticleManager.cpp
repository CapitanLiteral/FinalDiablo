#include "ParticleManager.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "p2Log.h"
#include "Audio.h"
#include "FileSystem.h"
#include "Collision.h"

#include <math.h>

#include "Input.h"

ParticleManager::ParticleManager() : Module()
{
	LOG("ParticleManager: Creating.");

	name.create("particle_manager");
}

ParticleManager::~ParticleManager()
{

}

bool ParticleManager::awake(pugi::xml_node &node)
{
	LOG("ParticleManager: Awake.");

	bool ret = true;

	particlesFile.assign(node.child("particle_doc").attribute("value").as_string());

	return ret;
}

bool ParticleManager::start()
{

	LOG("ParticleManager: Start.");
	bool ret = true;

	ret = loadParticlesFile(particleDoc);
	particleAtlas = app->tex->Load(particleDoc.child("particles").child("texture").attribute("value").as_string());
	spriteAtlas = new Sprite(particleAtlas, { 0, 0 }, { 0, 0, 0, 0}, UI);
	app->render->addSpriteToList(spriteAtlas);

	return ret;
}

bool ParticleManager::update(float dt)
{
	bool ret = true;
	
	std::list<Particle*>::iterator it = particleList.begin();
	while (it != particleList.end())
	{
		if ((*it)->update(dt) == false)
		{
			(*it)->col->to_delete = true;
			RELEASE(*it);
			it = particleList.erase(it);
		}
		else
			++it;
	}

	std::list<Emisor*>::iterator it2 = emisorList.begin();
	while (it2 != emisorList.end())
	{
		if ((*it2)->update(dt) == false)
		{
			RELEASE(*it2);
			it2 = emisorList.erase(it2);
		}
		else
			++it2;
	}

	return ret;
}

bool ParticleManager::postUpdate()
{
	bool ret = true;

	std::list<Particle*>::iterator it = particleList.begin();
	for (; it != particleList.end(); ++it)
		(*it)->postUpdate();

	std::list<Emisor*>::iterator it2 = emisorList.begin();
	for (; it2 != emisorList.end(); ++it2)
		(*it2)->postUpdate();

	return ret;
}

bool ParticleManager::cleanUp()
{
	LOG("ParticleManager: CleanUp.");

	bool ret = true;

	ret = cleanActiveParticles();
	ret = cleanActiveEmisors();

	return ret;
}

bool ParticleManager::cleanActiveParticles()
{
	bool ret = true;

	std::list<Particle*>::iterator it = particleList.begin();
	for (; it != particleList.end(); ++it)
		RELEASE(*it);

	particleList.clear();

	ret = particleList.empty();

	return ret;
}

bool ParticleManager::cleanActiveEmisors()
{
	bool ret = true;

	std::list<Emisor*>::iterator it = emisorList.begin();
	for (; it != emisorList.end(); ++it)
		RELEASE(*it);

	emisorList.clear();

	ret = emisorList.empty();

	return ret;
}

bool ParticleManager::loadParticlesFile(pugi::xml_document& file)
{
	bool ret = true;

	char* buff;
	int size = app->fs->Load("Particles/particles.xml", &buff);
	pugi::xml_parse_result result = file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		ret = false;
		LOG("Could not load particles xml file. Pugi error: %s", result.description());
	}

	return ret;
}

Sprite* ParticleManager::getParticleAtlas()const
{
	return spriteAtlas;
}

SDL_Texture* ParticleManager::getAtlas()const
{
	return particleAtlas;
}

pugi::xml_document* ParticleManager::getParticleDoc()
{
	return &particleDoc;
}

Particle* ParticleManager::createParticle(const Particle& refParticle, int x, int y, Uint32 secLife, iPoint colliderOffset, iPoint colliderSize, COLLIDER_TYPE colliderType, Module* listener, bool active, SDL_Texture* texture)
{
	Particle* ret = NULL;

	ret = new Particle(refParticle);
	ret->position.Set(x, y);
	ret->life = secLife;
	if (texture)
		ret->texture = texture;
	else
		ret->texture = particleAtlas;

	if (colliderType != COLLIDER_NONE)
	{
		ret->col = app->collision->addCollider({ x - colliderOffset.x, y - colliderOffset.y, colliderSize.x, colliderSize.y }, colliderType, listener);
	}

	ret->active = active;
	if (active)
		ret->timer.start();

	particleList.push_back(ret);

	return ret;
}

RadialEmisor* ParticleManager::createRadialEmisor(int x, int y, Module* listener, bool active)
{
	RadialEmisor* ret = NULL;

	ret = new RadialEmisor();
	ret->position.Set(x, y);
	ret->active = active;

	if (listener)
		ret->listener = listener;

	if (active)
		ret->timer.start();

	emisorList.push_back(ret);

	return ret;
}

LineEmisor* ParticleManager::createLineEmisor(int  x, int y, fPoint direction, Module* listener, bool active)
{
	LineEmisor* ret = NULL;

	ret = new LineEmisor(direction);
	ret->position.Set(x, y);
	ret->active = active;

	if (listener)
		ret->listener = listener;

	if (active)
		ret->timer.start();

	emisorList.push_back(ret);

	return ret;
}

ConeEmisor* ParticleManager::createConeEmisor(int x, int y, fPoint direction, Module* listener, bool active)
{
	ConeEmisor* ret = NULL;

	ret = new ConeEmisor(direction);
	ret->position.Set(x, y);
	ret->active = active;

	if (listener)
		ret->listener = listener;

	if (active)
		ret->timer.start();

	emisorList.push_back(ret);

	return ret;
}

void ParticleManager::OnCollision(Collider* c1, Collider* c2)
{

}

// --Particle-------------

Particle::Particle() : fx(-1), life(0), fxPlayed(false), alive(true), active(true)
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : fx(p.fx), life(p.life), fxPlayed(p.fxPlayed), alive(true), active(p.active), anim(p.anim), position(p.position), speed(p.speed)
{
	/*particleSprite = new Sprite(p.texture);
	app->render->addSpriteToList(particleSprite);*/
}

Particle::~Particle()
{

}

bool Particle::update(float dt)
{
	bool ret = true;

	if (life > 0)
	{
		if (timer.ReadSec() >= life || alive == false)
			ret = false;
	}
	else
	{
		if (anim.Finished() || alive == false)
			ret = false;
	}

	if (alive && active)
	{
		position.x += speed.x * dt;
		position.y += speed.y * dt;

		if (col)
		{
			col->rect.x += speed.x * dt;
			col->rect.y += speed.y * dt;
		}
	}

	return ret;
}

bool Particle::postUpdate()
{
	bool ret = true;

	if (alive)
	{
		if (active)
		{
			if (texture)
			{
				iPoint pos(position.x, position.y);
				iPoint piv(anim.pivot.x, anim.pivot.y);
				app->render->Blit(texture, position.x - anim.pivot.x, position.y - anim.pivot.y, &anim.getCurrentFrame());
				//particleSprite->updateSprite(pos, piv, anim.getCurrentFrame());
			}

			if (fxPlayed == false)
			{
				fxPlayed = true;
				app->audio->PlayFx(fx);
			}
		}

		if (app->debug == true)
			drawDebug();
	}

	return ret;
}

void Particle::enable()
{
	active = true;
	timer.start();
}

void Particle::disable()
{
	active = false;
}

void Particle::destroyParticle()
{
	alive = false;
}

void Particle::setRandomSpeed(float velocity, float minAngle, float maxAngle)
{
	float angle = minAngle + (rand() % (int)(maxAngle - minAngle));
	speed.x = velocity * cos(angle * (PI / 180));
	speed.y = velocity * sin(angle * (PI / 180));
}

void Particle::setLinearSpeed(float velocity, fPoint directiorVec)
{
	speed.x = velocity * directiorVec.x;
	speed.y = velocity * directiorVec.y;
}

void Particle::drawDebug()
{
	SDL_Rect sect{ position.x - anim.pivot.x, position.y - anim.pivot.y, anim.PeekCurrentFrame().w, anim.PeekCurrentFrame().h };
	app->render->DrawQuad(sect, 255, 0, 0, 255, false, true);
}


// --Emisor-----------------

Emisor::Emisor() : fx(-1), duration(0), fxPlayed(false), alive(true), active(false)
{
	position.SetToZero();
	speed.SetToZero();
}

Emisor::~Emisor()
{

}

bool Emisor::update(float dt)
{
	bool ret = true;

	return ret;
}

bool Emisor::postUpdate()
{
	bool ret = true;

	return ret;

}

void Emisor::enable()
{
	active = true;
	timer.start();
}

void Emisor::disable()
{
	active = false;
}

void Emisor::destroy()
{
	alive = false;
}

void Emisor::drawDebug()
{
	app->render->DrawQuad({ position.x - 1, position.y - 1, 3, 3 }, 0, 255, 0, 255, true, true);
}


// -- RadialEmisor------------------

RadialEmisor::RadialEmisor() : Emisor()
{
	pugi::xml_node radialEmisorNode = app->particleManager->getParticleDoc()->child("particles").child("radial_emisor");

	int animX = radialEmisorNode.child("anim").attribute("x").as_int();
	int animY = radialEmisorNode.child("anim").attribute("y").as_int();
	int animW = radialEmisorNode.child("anim").attribute("w").as_int();
	int animH = radialEmisorNode.child("anim").attribute("h").as_int();
	int animFrames = radialEmisorNode.child("anim").attribute("frame_number").as_int();
	float animSpeed = radialEmisorNode.child("anim").attribute("speed").as_float();
	bool loop = radialEmisorNode.child("anim").attribute("loop").as_bool();
	int margin = radialEmisorNode.child("anim").attribute("margin").as_int();
	int pivotX = radialEmisorNode.child("anim").attribute("pivot_x").as_int();
	int pivotY = radialEmisorNode.child("anim").attribute("pivot_y").as_int();

	particleEmited.anim.setAnimation(animX, animY, animW, animH, animFrames, margin);
	particleEmited.anim.loop = loop;
	particleEmited.anim.pivot.Set(pivotX, pivotY);
	particleEmited.anim.speed = animSpeed;
	particleEmited.life = radialEmisorNode.child("particle_life").attribute("value").as_int();
	particleEmited.speed.SetToZero();

	float eSpeedX = radialEmisorNode.child("emisor_speed").attribute("x").as_float();
	float eSpeedY = radialEmisorNode.child("emisor_speed").attribute("y").as_float();

	speed.Set(eSpeedX, eSpeedY);

	velocity = radialEmisorNode.child("particle_velocity").attribute("value").as_float();
	frequence = radialEmisorNode.child("frequence").attribute("value").as_int();
	duration = radialEmisorNode.child("duration").attribute("value").as_float();

	colliderOffset.x = radialEmisorNode.child("collider").attribute("offsetX").as_int();
	colliderOffset.y = radialEmisorNode.child("collider").attribute("offsetY").as_int();
	colliderSize.x = radialEmisorNode.child("collider").attribute("sizeW").as_int();
	colliderSize.y = radialEmisorNode.child("collider").attribute("sizeH").as_int();
	colliderType = (COLLIDER_TYPE)radialEmisorNode.child("collider").attribute("type").as_int();

	particleEmited.texture = app->particleManager->getAtlas();
}

RadialEmisor::~RadialEmisor()
{

}

bool RadialEmisor::update(float dt)
{
	bool ret = true;

	if (duration > 0)
	{
		if (timer.ReadSec() >= duration || alive == false)
			ret = false;
	}

	if (alive && active)
	{
		if (acumulator <= 1.0f && particlesEmited < frequence)
		{
			Particle* p = app->particleManager->createParticle(particleEmited, position.x, position.y, particleEmited.life, colliderOffset, colliderSize, colliderType, listener);
			p->setRandomSpeed(velocity);
			acumulator += dt;
			++particlesEmited;
		}
		else
		{
			acumulator += dt;
			if (acumulator >= 1.0f)
			{
				acumulator = 0.0f;
				particlesEmited = 0;
			}
		}

		position.x += speed.x * dt;
		position.x += speed.y * dt;
	}

	return ret;
}

bool RadialEmisor::postUpdate()
{
	bool ret = true;

	if (fxPlayed == false)
	{
		fxPlayed = true;
		app->audio->PlayFx(fx);
	}

	if (app->debug)
		drawDebug();

	return ret;
}

void RadialEmisor::drawDebug()
{
	app->render->DrawQuad({ position.x - 5, position.y - 5, 10, 10 }, 0, 0, 255, 255, true, true);
	app->render->DrawCircle(position.x, position.y, velocity*particleEmited.life, 255, 102, 255, 255, true);
}


// -- RadialEmisor------------------

LineEmisor::LineEmisor(fPoint director) : Emisor()
{
	pugi::xml_node lineEmisorNode = app->particleManager->getParticleDoc()->child("particles").child("line_emisor");

	int animX = lineEmisorNode.child("anim").attribute("x").as_int();
	int animY = lineEmisorNode.child("anim").attribute("y").as_int();
	int animW = lineEmisorNode.child("anim").attribute("w").as_int();
	int animH = lineEmisorNode.child("anim").attribute("h").as_int();
	int animFrames = lineEmisorNode.child("anim").attribute("frame_number").as_int();
	float animSpeed = lineEmisorNode.child("anim").attribute("speed").as_float();
	bool loop = lineEmisorNode.child("anim").attribute("loop").as_bool();
	int margin = lineEmisorNode.child("anim").attribute("margin").as_int();
	int pivotX = lineEmisorNode.child("anim").attribute("pivot_x").as_int();
	int pivotY = lineEmisorNode.child("anim").attribute("pivot_y").as_int();

	particleEmited.anim.setAnimation(animX, animY, animW, animH, animFrames, margin);
	particleEmited.anim.loop = loop;
	particleEmited.anim.pivot.Set(pivotX, pivotY);
	particleEmited.anim.speed = animSpeed;
	particleEmited.life = lineEmisorNode.child("particle_life").attribute("value").as_int();
	particleEmited.speed.SetToZero();

	float eSpeedX = lineEmisorNode.child("emisor_speed").attribute("x").as_float();
	float eSpeedY = lineEmisorNode.child("emisor_speed").attribute("y").as_float();

	speed.Set(eSpeedX, eSpeedY);

	velocity = lineEmisorNode.child("particle_velocity").attribute("value").as_float();
	frequence = lineEmisorNode.child("frequence").attribute("value").as_int();
	duration = lineEmisorNode.child("duration").attribute("value").as_float();

	colliderOffset.x = lineEmisorNode.child("collider").attribute("offsetX").as_int();
	colliderOffset.y = lineEmisorNode.child("collider").attribute("offsetY").as_int();
	colliderSize.x = lineEmisorNode.child("collider").attribute("sizeW").as_int();
	colliderSize.y = lineEmisorNode.child("collider").attribute("sizeH").as_int();
	colliderType = (COLLIDER_TYPE)lineEmisorNode.child("collider").attribute("type").as_int();

	particleEmited.texture = app->particleManager->getAtlas();

	direction = director;
	direction.y = -direction.y;
}

LineEmisor::~LineEmisor()
{

}

bool LineEmisor::update(float dt)
{
	bool ret = true;

	if (duration > 0)
	{
		if (timer.ReadSec() >= duration || alive == false)
			ret = false;
	}

	if (alive && active)
	{
		if (acumulator <= 1.0f && particlesEmited < frequence)
		{
			Particle* p = app->particleManager->createParticle(particleEmited, position.x, position.y, particleEmited.life, colliderOffset, colliderSize, colliderType, listener);
			p->setLinearSpeed(velocity, direction);
			acumulator += dt;
			++particlesEmited;
		}
		else
		{
			acumulator += dt;
			if (acumulator >= 1.0f)
			{
				acumulator = 0.0f;
				particlesEmited = 0;
			}
		}

		position.x += speed.x * dt;
		position.x += speed.y * dt;
	}

	return ret;
}

bool LineEmisor::postUpdate()
{
	bool ret = true;

	if (fxPlayed == false)
	{
		fxPlayed = true;
		app->audio->PlayFx(fx);
	}

	if (app->debug)
		drawDebug();

	return ret;
}

void LineEmisor::drawDebug()
{
	app->render->DrawQuad({ position.x - 5, position.y - 5, 10, 10 }, 0, 0, 255, 255, true, true);
	app->render->DrawLine(position.x, position.y, position.x + (direction.x * velocity * particleEmited.life), position.y + (direction.y * velocity * particleEmited.life), 255, 102, 255, 255, true);
}

// -- RadialEmisor------------------

ConeEmisor::ConeEmisor(fPoint director) : Emisor()
{
	pugi::xml_node coneEmisorNode = app->particleManager->getParticleDoc()->child("particles").child("cone_emisor");

	int animX = coneEmisorNode.child("anim").attribute("x").as_int();
	int animY = coneEmisorNode.child("anim").attribute("y").as_int();
	int animW = coneEmisorNode.child("anim").attribute("w").as_int();
	int animH = coneEmisorNode.child("anim").attribute("h").as_int();
	int animFrames = coneEmisorNode.child("anim").attribute("frame_number").as_int();
	float animSpeed = coneEmisorNode.child("anim").attribute("speed").as_float();
	bool loop = coneEmisorNode.child("anim").attribute("loop").as_bool();
	int margin = coneEmisorNode.child("anim").attribute("margin").as_int();
	int pivotX = coneEmisorNode.child("anim").attribute("pivot_x").as_int();
	int pivotY = coneEmisorNode.child("anim").attribute("pivot_y").as_int();

	particleEmited.anim.setAnimation(animX, animY, animW, animH, animFrames, margin);
	particleEmited.anim.loop = loop;
	particleEmited.anim.pivot.Set(pivotX, pivotY);
	particleEmited.anim.speed = animSpeed;
	particleEmited.life = coneEmisorNode.child("particle_life").attribute("value").as_int();
	particleEmited.speed.SetToZero();

	float eSpeedX = coneEmisorNode.child("emisor_speed").attribute("x").as_float();
	float eSpeedY = coneEmisorNode.child("emisor_speed").attribute("y").as_float();

	speed.Set(eSpeedX, eSpeedY);

	velocity = coneEmisorNode.child("particle_velocity").attribute("value").as_float();
	frequence = coneEmisorNode.child("frequence").attribute("value").as_int();
	duration = coneEmisorNode.child("duration").attribute("value").as_float();

	angle = coneEmisorNode.child("open_angle").attribute("value").as_float();

	colliderOffset.x = coneEmisorNode.child("collider").attribute("offsetX").as_int();
	colliderOffset.y = coneEmisorNode.child("collider").attribute("offsetY").as_int();
	colliderSize.x = coneEmisorNode.child("collider").attribute("sizeW").as_int();
	colliderSize.y = coneEmisorNode.child("collider").attribute("sizeH").as_int();
	colliderType = (COLLIDER_TYPE)coneEmisorNode.child("collider").attribute("type").as_int();

	particleEmited.texture = app->particleManager->getAtlas();

	direction = director;
	direction.y = -direction.y;
}

ConeEmisor::~ConeEmisor()
{

}

bool ConeEmisor::update(float dt)
{
	bool ret = true;

	if (duration > 0)
	{
		if (timer.ReadSec() >= duration || alive == false)
			ret = false;
	}

	if (alive && active)
	{
		if (acumulator <= 1.0f && particlesEmited < frequence)
		{
			Particle* p = app->particleManager->createParticle(particleEmited, position.x, position.y, particleEmited.life, colliderOffset, colliderSize, colliderType, listener);
			float angleDir = calcAngle(direction);
			p->setRandomSpeed(velocity, angleDir - angle, angleDir + angle);
			acumulator += dt;
			++particlesEmited;
		}
		else
		{
			acumulator += dt;
			if (acumulator >= 1.0f)
			{
				acumulator = 0.0f;
				particlesEmited = 0;
			}
		}

		position.x += speed.x * dt;
		position.x += speed.y * dt;
	}

	return ret;
}

bool ConeEmisor::postUpdate()
{
	bool ret = true;

	if (fxPlayed == false)
	{
		fxPlayed = true;
		app->audio->PlayFx(fx);
	}

	if (app->debug)
		drawDebug();

	return ret;
}

void ConeEmisor::drawDebug()
{
	app->render->DrawQuad({ position.x - 5, position.y - 5, 10, 10 }, 0, 0, 255, 255, true, true);
	app->render->DrawLine(position.x, position.y, position.x + (direction.x * velocity * particleEmited.life), position.y + (direction.y * velocity * particleEmited.life), 255, 102, 255, 255, true);

	float angleDir = calcAngle(direction);
	fPoint s;
	s.x = cos((angleDir - angle) * (PI / 180));
	s.y = sin((angleDir - angle) * (PI / 180));
	fPoint s2;
	s2.x = cos((angleDir + angle) * (PI / 180));
	s2.y = sin((angleDir + angle) * (PI / 180));

	app->render->DrawLine(position.x, position.y, position.x + (direction.x * velocity * s.x * particleEmited.life), position.y + (direction.y * velocity * s.y * particleEmited.life), 255, 102, 255, 255, true);
	app->render->DrawLine(position.x, position.y, position.x + (direction.x * velocity * s2.x * particleEmited.life), position.y + (direction.y * velocity * s2.y * particleEmited.life), 255, 102, 255, 255, true);

}

float ConeEmisor::calcAngle(fPoint vec)
{
	return atan2f(vec.y, vec.x) * (180 / PI);
}