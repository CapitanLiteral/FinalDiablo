#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include <list>
#include "Timer.h"

class Sprite;
class Collider;
enum COLLIDER_TYPE;

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
	SDL_Texture*		texture = NULL;
	bool				alive;
	bool				active;
	Collider*			col = NULL;
	iPoint				colOffset;

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

	//Particles Emited Collider
	iPoint colliderOffset;
	iPoint colliderSize;
	COLLIDER_TYPE colliderType = (COLLIDER_TYPE )- 1;
	Module* listener = NULL;

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

	float calcAngle(fPoint vec);
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

};

// ------------------------------------------------

class CrossEmisor1 : public Emisor
{
public:
	Particle particleEmited;
	CrossEmisor1();
	~CrossEmisor1();

	bool update(float dt);
	bool postUpdate();

	void drawDebug();

private:
	float particleVelocity;
};

// ------------------------------------------------

class CrossEmisor2 : public Emisor
{
public:
	Particle particleEmited;
	CrossEmisor2();
	~CrossEmisor2();

	bool update(float dt);
	bool postUpdate();

	void drawDebug();

private:
	float particleVelocity;
};

// ------------------------------------------------

class TridentEmisor : public Emisor
{
public:
	Particle particleEmited;
	TridentEmisor(fPoint director);
	~TridentEmisor();

	bool update(float dt);
	bool postUpdate();

	void drawDebug();

private:
	float particleVelocity;
	float angle;
	fPoint direction;
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

	void OnCollision(Collider* c1, Collider* c2);

	bool cleanActiveParticles();

	bool cleanActiveEmisors();

	Sprite* getParticleAtlas()const;
	SDL_Texture* getAtlas()const;

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
	Particle* createParticle(const Particle& refParticle, int x, int y, Uint32 secLife, iPoint colliderOffset = { 0, 0 }, iPoint colliderSize = { 0, 0 }, COLLIDER_TYPE colliderType = (COLLIDER_TYPE)-1, Module* listener = NULL, bool active = true, SDL_Texture* texture = NULL);

	RadialEmisor* createRadialEmisor(int x, int y, Module* listener = NULL, bool active = true);

	LineEmisor* createLineEmisor(int  x, int y, fPoint direction, Module* listener = NULL, bool active = true);

	ConeEmisor* createConeEmisor(int x, int y, fPoint direction, Module* listener = NULL, bool active = true);

	CrossEmisor1* createCross1Emisor(int x, int y, Module* listener = NULL, bool active = true);

	CrossEmisor2* createCross2Emisor(int x, int y, Module* listener = NULL, bool active = true);

	TridentEmisor* createTridentEmisor(int x, int y, fPoint direction, Module* listener = NULL, bool active = true);

	int fxPlayerSkill=0;
};





#endif