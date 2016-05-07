#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include <list>
#include "Timer.h"

class Sprite;

#define PI 3.14159265

struct SDL_Texture;

// ------------------------------------------------
struct Particle
{
	Animation			anim;
	unsigned int		fx; //This is for audio
	fPoint				position;
	fPoint				speed;
	Uint32				life;	// Time the particle life
	bool				fxPlayed;
	Timer				timer;
	Sprite*				particleSprite = NULL;
	bool				alive;
	bool				active;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool update(float dt);
	bool postUpdate();
	void enable();
	void disable();
	void destroyParticle();

	void drawDebug();

	void setRandomSpeed(float velocity, float minAngle = 0.0f, float maxAngle = 360.0f);
	void setLinearSpeed(float velocity, fPoint directiorVec);
};

// ------------------------------------------------

class Emisor
{
public:
	fPoint		position;
	fPoint		speed;
	float		duration;
	Timer		timer;
	bool		active;
	bool		alive;
	uint	    fx;
	bool		fxPlayed;

public:
	Emisor();
	virtual ~Emisor();
	virtual bool update(float dt);
	virtual bool postUpdate();
	void enable();
	void disable();
	void destroy();

protected:
	virtual void drawDebug();
};

// ------------------------------------------------

class RadialEmisor : public Emisor
{
public:
	Particle particleEmited;
	float velocity;
	int frequence;

	RadialEmisor();
	~RadialEmisor();

	bool update(float dt);
	bool postUpdate();

	void drawDebug();

private:
	float acumulator = 0.0f;
	int particlesEmited = 0;
};

// ------------------------------------------------

class LineEmisor : public Emisor
{
public:
	Particle particleEmited;
	float velocity;
	int frequence;
	fPoint direction;

	LineEmisor(fPoint director);
	~LineEmisor();

	bool update(float dt);
	bool postUpdate();

	void drawDebug();

private:
	float acumulator = 0.0f;
	int particlesEmited = 0;

};

// ------------------------------------------------

class ConeEmisor : public Emisor
{
public:
	Particle particleEmited;
	float velocity;
	int frequence;
	fPoint direction;
	float angle;

	ConeEmisor(fPoint director);
	~ConeEmisor();

	bool update(float dt);
	bool postUpdate();

	void drawDebug();

private:
	float acumulator = 0.0f;
	int particlesEmited = 0;

	float calcAngle(fPoint vec);
};

// ------------------------------------------------

class ParticleManager : public Module
{
public:
	ParticleManager();

	~ParticleManager();

	bool awake(pugi::xml_node &node);

	bool start();

	bool update(float dt);

	bool postUpdate();

	bool cleanUp();

	bool cleanActiveParticles();

	bool cleanActiveEmisors();

	Sprite* getParticleAtlas()const;

	pugi::xml_document* getParticleDoc();

private:
	std::string particlesFile;
	SDL_Texture* particleAtlas = NULL;
	Sprite*		 spriteAtlas = NULL;
	pugi::xml_document particleDoc;

	std::list<Particle*> particleList;
	std::list<Emisor*> emisorList;

	bool loadParticlesFile(pugi::xml_document& file);

public:
	Particle* createParticle(const Particle& refParticle, int x, int y, Uint32 secLife, bool active = true, SDL_Texture* texture = NULL);

	RadialEmisor* createRadialEmisor(int x, int y, bool active = true);

	LineEmisor* createLineEmisor(int  x, int y, fPoint direction, bool active = true);

	ConeEmisor* createConeEmisor(int x, int y, fPoint direction, bool active = true);
};





#endif