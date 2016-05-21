#include "Player.h"
#include "Attributes.h"
#include "Collision.h"
#include "App.h"
#include "p2Log.h"
#include "FileSystem.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Input.h"
#include "Pathfinding.h"
#include "Game.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Gui.h"
#include "Window.h"
#include "Audio.h"

Player::Player()
{
	AttributeBuilder builder;
	builder.base_damage = 50;
	builder.base_movementSpeed = 200; //tmp maybe this speed is better, less strange movement in animation and more acurated with the game
	builder.base_lifeRegen = 15;
	attributes = new PlayerAttributes(builder);

	worldPosition.x = 0;
	worldPosition.y = 0;
	mapPosition.x = 0;
	mapPosition.y = 0;

	colliderOffset.SetToZero();
	colliderSize.SetToZero();

	for (int i = 0; i < 4; i++)
	{
		activePotis.push_back(true);
	}
	

}

bool Player::awake(pugi::xml_node &config)
{
	colliderSize.Set(50, 100);
	colliderOffset.Set(25, 100);
	return true;
}

bool Player::start()
{
	bool ret = true;

	currentPhase = BARBARIAN;

	if (attributes == NULL)
	{
		AttributeBuilder builder;
		builder.base_damage = 50;
		builder.base_movementSpeed = 200; //tmp maybe this speed is better, less strange movement in animation and more acurated with the game
		builder.base_lifeRegen = 15;
		attributes = new PlayerAttributes(builder);
	}
		

	


	//POTIS
	atlas = app->gui->getAtlas();
	potiAtlas = { 703, 892, 29, 29 };

	SDL_Rect rect = { worldPosition.x - colliderOffset.x, 
					  worldPosition.y - colliderOffset.y,	// Position
					  colliderSize.x, colliderSize.y};		// Size
	collider = app->collision->addCollider(rect, COLLIDER_PLAYER, this);

	deathImage = app->gui->addGuiImageFader({ 0, 0 }, { 679, 1065, 669, 501 }, 5, NULL, NULL);
	//deathImage->SetLocalPosition({ 0, 0 });
	deathImage->Center(true, true);
	deathImage->Desactivate();

	//Entity range detection
	entityRange = 100.0f;

	//FX sounds
	fxPlayerLvlUp = app->audio->LoadFx("audio/fx/LvlUp.wav");
	walkFx = app->audio->LoadFx("audio/fx/Walk_sound");
	
	// ANIMATION
	if (loadAnimations())
	{
		//Sprites
		pDebug = app->tex->Load("maps/mini_path.png");

		barbarianImage = app->tex->Load("images/Barbarian.png");
		butcherImage = app->tex->Load("images/Butcher.png");
		diabloImage = app->tex->Load("images/Diablo.png");
		particlesAtlas = app->tex->Load("Particles/Particles.png");

		current_animation = &barbarianAnim.find({ IDLE, D_FRONT })->second;

		sprite = new Sprite(barbarianImage,
			worldPosition,
			current_animation->pivot,
			current_animation->getCurrentFrame());
		sprite->layer = SCENE;
		app->render->addSpriteToList(sprite);
	}
	else
		ret = false;

	rageArround.anim.setAnimation(0, 132, 64, 64, 7, 0);
	rageArround.active = true;
	rageArround.anim.speed = 0.15f;
	rageArround.anim.loop = true;
	rageArround.anim.pivot.Set(30, 50);
	rageArround.life = 10;
	rageArround.fx = app->audio->LoadFx("audio/fx/Diablo_attack");
	rageArround.texture = particlesAtlas;


	rageCoolDown.start();//tmp
	skill2CoolDown.start();

	rageMod = new TempMod(rageDuration, 0.05, NONFLAT_DAMAGE); //0.5%dmg = 0.05
	

	return ret;
}
bool Player::preUpdate()
{
	

	return true;
}

bool Player::update(float dt)
{

	handleInput(dt);

	updateAction();

	//LOG("Exp: %d", attributes->getExp());
	//LOG("Level: %f", attributes->getLevel());
	//LOG("Rage: %f", attributes->getRage());
	
	//POTIS
	for (int i = 0; i < 4; i++)
	{
		if (activePotis[i])
		{
			app->render->Blit(atlas, position.x - app->render->camera.x + 50 + i*29, position.y - app->render->camera.y-50, &potiAtlas);
			//app->render->Blit(atlas, position.x + 32 * i, position.y-500, &potiAtlas);
		}
	}

	bool ret = true;

	if (!app->debug)
	{
		app->render->CenterCamera(worldPosition.x, worldPosition.y);
	}
	
	if (app->debug)
	{
		if (app->input->getKey(SDL_SCANCODE_M) == KEY_REPEAT)
			attributes->addExp(100);
		if (app->input->getMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
		{
			iPoint p = app->input->getMouseWorldPosition();
			LOG("X: %d Y: %d", p.x, p.y);
		}
		if (app->input->getKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			worldPosition.x = 2789;
			worldPosition.y = 1827;
		}
	}
	
	
	//TODO JOSEP PARTICULA LVL UP
	if (attributes->getLevel() == 5)
	{
		currentPhase = BUTCHER;
		sprite->texture = butcherImage;
		if (soundChecked == false)
		{
			app->audio->PlayFx(fxPlayerLvlUp, 0);
			soundChecked = true;
		}
	}
	else if (attributes->getLevel() == 10)
	{
		currentPhase = DIABLO;
		sprite->texture = diabloImage;

		if (soundChecked == true)
		{
			app->audio->PlayFx(fxPlayerLvlUp, 0);
			soundChecked = false;
		}
	}

	


	if (current_action != DEATH)
	{
		switch (current_action)
		{
		case IDLE:
			break;
		case WALKING:
			break;
		case RUNNING:
			//LowerStamina();
			break;
		case BASIC_ATTACK:
			if (current_animation->isOver() && enemyFocus != NULL)
			{
				//Particula de atac basic
				current_input_event = I_STOP;
				current_animation->Reset();
			}
			break;
		case SKILL1:
			attributes->addMod(rageMod);
			current_rage = app->particleManager->createParticle(rageArround, worldPosition.x, worldPosition.y, rageDuration, { 0, 0 }, { 64, 64 }, COLLIDER_PLAYER_PARTICLE, this, true, particlesAtlas);
			rageTime.start();
			attributes->addRage(-furySkill1Cost);
			skill = 0;
		break;
		case SKILL2:
			iPoint p = app->input->getMouseWorldPosition();
			fPoint dirVec(-(worldPosition.x - p.x), worldPosition.y - p.y);
			app->particleManager->createLineEmisor(worldPosition.x, worldPosition.y, dirVec, this);
			attributes->addRage(-furySkill2Cost);
			skill = 0;
		break;
		}
	}
	else
	{
		setCurrentAnimation();
		inputBlocked = true;
		dead = true;

		if (current_animation->isOver())
		{
			if (imageTimerStarted == false)
			{
				showLoseImage.start();
				imageTimerStarted = true;
			}
		}
	}
	

	if (showLoseImage.ReadSec() >= timeToShow && dead)
	{
		//Start lose image
		deathImage->Activate();
		deathImage->timer.start();
		respawn();
	}

	if (app->debug)
	{
		drawDebug();
	}

	return ret;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (enemyFocus != NULL)
	{
		if ((c1 == collider || c2 == collider) && (c1 == enemyFocus->getCollider() || c2 == enemyFocus->getCollider()))
		{
			collision = true;
		}
	}
	
	if (c1->type == COLLIDER_PLAYER_PARTICLE || c2->type == COLLIDER_PLAYER_PARTICLE)
	{
		if (c1->entityLinked)
		{
			if (c2->collisioned == false)
			{
				int dmg = c1->entityLinked->attributes->getMaxLife()*0.03;
				c1->entityLinked->attributes->addLife(-dmg);
				c2->collisioned = true;
			}
		}
		else if(c2->entityLinked && c2->type == COLLIDER_ENEMY)
		{
			if (c1->collisioned == false)
			{
				int dmg = c2->entityLinked->attributes->getMaxLife()*0.03;
				c2->entityLinked->attributes->addLife(-dmg);
				c1->collisioned = true;
			}
		}
	}

}

void Player::respawn()
{
	worldPosition = startingPosition;

	collider->SetPos(startingPosition.x - colliderOffset.x, startingPosition.y - colliderOffset.y);

	attributes->addLife(attributes->getMaxLife());

	current_action = IDLE;
	current_direction = D_FRONT;

	current_animation->Reset();

	setCurrentAnimation();

	//At end of tp unblit lose image
		//Done by the gui element itself

	dead = false;
	imageTimerStarted = false;
	inputBlocked = false;
}

void Player::draw()
{
	//Not elegant, but works // May be the vibration of player comes from here ERROR
	if (current_action == WALKING || current_action == RUNNING)
		setDirection();
	//if (previous_direction != current_direction || 
	//	previous_action != current_action)
	//{
	setCurrentAnimation();
	//	previous_action = current_action;
	//	previous_direction = current_direction;
	//}
	

	sprite->updateSprite(worldPosition,
						current_animation->pivot,
						current_animation->getCurrentFrame());

	if (current_rage)
	{
		if (rageTime.ReadSec() < rageDuration)
		{
			current_rage->position.x = worldPosition.x;
			current_rage->position.y = worldPosition.y;
			current_rage->col->SetPos(worldPosition.x - current_rage->anim.pivot.x, worldPosition.y - current_rage->anim.pivot.y);
		}
		else
		{
			current_rage = NULL;
		}
	}//This if MUST be removed and forgotten for ever

}
/*
vector<iPoint> Player::getNewPath(iPoint target)
{
	iPoint start = app->map->WorldToMap(worldPosition.x, worldPosition.y);
	iPoint goal = target;
	vector<iPoint> _path;
	int steps = app->pathfinding->getNewPath(start, goal, path);
	_path = path;
	if (steps > 0)
	{
		//StateMachine change
		current_input_event = I_WALK;

		movement = true;
		currentNode = -1;
		getNewTarget();
	}
	return _path;
}

void Player::getNewTarget()
{
	if ((uint)currentNode + 1 < path.size())
	{
		currentNode++;
		setTarget(app->map->getTileCenter(path[currentNode].x, path[currentNode].y));
	}
	else
	{
		//Maybe ERROR, watch out //setInput(INPUT_STOP_MOVE);
		movement = false;
	}
}

void Player::setTarget(iPoint _target)
{
	target = _target;
	movement = true;
	targetReached = false;
}

void Player::updateVelocity(float dt)
{
	velocity.x = target.x - worldPosition.x;
	velocity.y = target.y - worldPosition.y;

	velocity.SetModule(attributes->getMovementSpeed());

	//Maybe ERROR, watch out //SetDirection();
}

void Player::updateMovement(float dt)
{
	if (movement)
	{
		if (!targetReached)
		{
			updateVelocity(dt);
			move(dt);
			targetReached = isTargetReached();
		}
		else
		{
			getNewTarget();
		}
	}
	if (isInDestiny() || (enemyFocus != NULL && enemyFocus->getMapPos().DistanceTo(worldPosition) <= targetRadius && current_action == WALKING))
	{
		current_input_event = I_STOP;
	}

}
void Player::move(float dt)
{
	fPoint vel = velocity * dt;
	if (path.size() > 0)
	{
		worldPosition.x += int(vel.x);
		worldPosition.y += int(vel.y);
	}
	else
	{
		current_input_event = I_STOP;

	}
	
	//LOG("Moving 1");
	//NOTE: Collider movement, may be changed

	collider->SetPos(worldPosition.x - colliderOffset.x, worldPosition.y - colliderOffset.y); //Maybe ERROR, watch out
	if (isInDestiny() || (enemyFocus != NULL && enemyFocus->getMapPos().DistanceTo(worldPosition) <= targetRadius && current_action == WALKING))
	{
		path.clear();
	}
}
bool Player::isTargetReached() //Maybe ERROR, watch out //This does not work
{
	fPoint vel;

	vel.x = target.x - worldPosition.x;
	vel.y = target.y - worldPosition.y;
	
	if (vel.getModule() <= targetRadius)
	{
		return true;
	}

	return false;
}
bool Player::isInDestiny() //Maybe ERROR, watch out //This does not work
{
	if (app->map->WorldToMap(clickCoords.x, clickCoords.y) == app->map->WorldToMap(worldPosition.x, worldPosition.y))
	{
		return true;
	}

	return false;
}

*/

void Player::drawDebug() const
{
	iPoint position = getMapPosition();

	app->render->Blit(pDebug, position.x, position.y);
	app->render->DrawCircle(worldPosition.x, worldPosition.y, targetRadius, 255, 0, 0, 1000);

	app->render->DrawCircle(worldPosition.x, worldPosition.y, entityRange, 0, 0, 255);
	//App->render->DrawCircle(p_pos.x, p_pos.y, attack_range, 255, 0, 0);

	if (movement)
	{
		app->render->DrawLine(worldPosition.x, worldPosition.y, target.x, target.y, 0, 0, 255);
		app->render->DrawLine(worldPosition.x, worldPosition.y, velocity.x + worldPosition.x, velocity.y + worldPosition.y, 0, 255, 255);

		//Path
		for (int i = 0; i < path.size(); i++)
		{
			iPoint tmp = path[i];
			tmp = app->map->getTileBlit(tmp.x, tmp.y);
			app->render->Blit(pDebug, tmp.x, tmp.y);
		}
	}

	if (app->debug)
	{
		if (app->input->getKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			attributes->addLife(-2050);
		}
	}
}

Player::~Player()
{
	RELEASE(attributes);
}

void Player::handleInput(float dt)
{
	if (!inputBlocked)
	{
		
		if (current_action != DEATH)
		{
			if (attributes->getLife() <= 0)
			{
				current_input_event = I_DIE;
			}

			if (app->input->getKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				activePotis[0] = false;
				float maxLife = attributes->getMaxLife();
				maxLife *= 0.1f;
				attributes->addMod(new TempMod(3.0f, maxLife, FLAT_LIFE_REGEN));
			}
			if (app->input->getKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				activePotis[1] = false;
				float maxLife = attributes->getMaxLife();
				maxLife *= 0.1f;
				attributes->addMod(new TempMod(3.0f, 30.0f, FLAT_LIFE_REGEN));
			}
			if (app->input->getKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				activePotis[2] = false;
				float maxLife = attributes->getMaxLife();
				maxLife *= 0.1f;
				attributes->addMod(new TempMod(3.0f, 30.0f, FLAT_LIFE_REGEN));
			}
			if (app->input->getKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				activePotis[3] = false;
				float maxLife = attributes->getMaxLife();
				maxLife *= 0.1f;
				attributes->addMod(new TempMod(3.0f, 30.0f, FLAT_LIFE_REGEN));
			}

			//SKILLS
			if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
			{
				//Do skill
				if (attributes->getLevel() >= 3)
				{
					current_input_event = I_SKILL;
					skill = 2;
				}
			}
			if (app->input->getKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				//Do skill
				if (attributes->getLevel() >= 5)
				{
					current_input_event = I_SKILL;
					skill = 1;
				}
			}
			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				current_input_event = I_ATTACK;
				
			}
			if (app->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				iPoint tileCollision = app->map->WorldToMap(worldPosition.x, worldPosition.y-20);
				
				MapLayer* walkability = NULL;
				std::list<MapLayer*>::iterator it = app->map->data.layers.begin();
				while (it != app->map->data.layers.end())
				{
					if ((*it)->name == "Navigation")
					{
						walkability = *it;
						break;
					}
					it++;
				}
				if (!((walkability != NULL)))
				{
					LOG("Couldn't find 'Walkable' layer");
				}

				if (walkability->get(tileCollision.x, tileCollision.y) == 2)
				{
					worldPosition.y -= attributes->getMovementSpeed()*dt;
					collider->SetPos(worldPosition.x - collider->rect.w / 2, worldPosition.y - collider->rect.h);
				}

				
			}
			if (app->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				iPoint tileCollision = app->map->WorldToMap(worldPosition.x - 50, worldPosition.y);

				MapLayer* walkability = NULL;
				std::list<MapLayer*>::iterator it = app->map->data.layers.begin();
				while (it != app->map->data.layers.end())
				{
					if ((*it)->name == "Navigation")
					{
						walkability = *it;
						break;
					}
					it++;
				}
				if (!((walkability != NULL)))
				{
					LOG("Couldn't find 'Walkable' layer");
				}

				if (walkability->get(tileCollision.x, tileCollision.y) == 2)
				{
					worldPosition.x -= attributes->getMovementSpeed()*dt;
					collider->SetPos(worldPosition.x - collider->rect.w / 2, worldPosition.y - collider->rect.h);
				}
			}
			if (app->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				iPoint tileCollision = app->map->WorldToMap(worldPosition.x + 50, worldPosition.y);

				MapLayer* walkability = NULL;
				std::list<MapLayer*>::iterator it = app->map->data.layers.begin();
				while (it != app->map->data.layers.end())
				{
					if ((*it)->name == "Navigation")
					{
						walkability = *it;
						break;
					}
					it++;
				}
				if (!((walkability != NULL)))
				{
					LOG("Couldn't find 'Walkable' layer");
				}

				if (walkability->get(tileCollision.x, tileCollision.y) == 2)
				{
					worldPosition.x += attributes->getMovementSpeed()*dt;
					collider->SetPos(worldPosition.x - collider->rect.w / 2, worldPosition.y - collider->rect.h);
				}
			}
			if (app->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				iPoint tileCollision = app->map->WorldToMap(worldPosition.x, worldPosition.y + 20);

				MapLayer* walkability = NULL;
				std::list<MapLayer*>::iterator it = app->map->data.layers.begin();
				while (it != app->map->data.layers.end())
				{
					if ((*it)->name == "Navigation")
					{
						walkability = *it;
						break;
					}
					it++;
				}
				if (!((walkability != NULL)))
				{
					LOG("Couldn't find 'Walkable' layer");
				}

				if (walkability->get(tileCollision.x, tileCollision.y) == 2)
				{
					worldPosition.y += attributes->getMovementSpeed()*dt;
					collider->SetPos(worldPosition.x - collider->rect.w / 2, worldPosition.y - collider->rect.h);
				}
			}
		}
		else
		{
			
		}
	}
	inputBlocked = false;
	//LOG("Input: %d", current_input_event);
	//LOG("Action: %d", current_action);
}
/*void Player::setMovement(int x, int y)
{
	if (path_on)
	{
		SetNewPath(x, y);
	}
	else
	{
		iPoint target = App->map->GetTileCenter(x, y);
		SetTarget(target);

		//StateMachine change
		SetInput(INPUT_MOVE);
	}
}*/
ACTION_STATE Player::updateAction()
{
	if (current_input_event != I_NULL)
	{
		switch (current_action)
		{
		case IDLE:
		{
			if (current_input_event == I_WALK)
			{
				current_action = WALKING;
			}
			else if (current_input_event == I_RUN)
			{
				current_action = RUNNING;
			}
			else if (current_input_event == I_DIE)
			{
				current_action = DEATH;
			}
			else if (current_input_event == I_ATTACK)
			{
				current_action = BASIC_ATTACK;
			}
			else if (current_input_event == I_SKILL)
			{
				current_action = IDLE;

				switch (skill)
				{
				case 1:
					if (attributes->getRage() >= furySkill1Cost && rageCoolDown.ReadSec() >= cooldownRageDuration)
					{
						current_action = SKILL1;
						rageCoolDown.start();
					}
					break;
				case 2:
					if (attributes->getRage() >= furySkill2Cost && skill2CoolDown.ReadSec() >= cooldownSkill2Duration)
					{
						current_action = SKILL2;
						skill2CoolDown.start();
					}
					break;
				}
			}
		}
			break;

		case WALKING:
		{
			if (current_input_event == I_STOP)
			{
				current_action = IDLE;
			}
			else if (current_input_event == I_RUN)
			{
				current_action = RUNNING;
			}
			else if (current_input_event == I_DIE)
			{
				current_action = DEATH;
			}
			else if (current_input_event == I_ATTACK)
			{
				current_action = BASIC_ATTACK;
			}
			else if (current_input_event == I_SKILL)
			{
				current_action = IDLE;

				switch (skill)
				{
				case 1:
					if (attributes->getRage() >= furySkill1Cost && rageCoolDown.ReadSec() >= cooldownRageDuration)
					{
						current_action = SKILL1;
						rageCoolDown.start();
					}
					break;
				case 2:
					if (attributes->getRage() >= furySkill2Cost && skill2CoolDown.ReadSec() >= cooldownSkill2Duration)
					{
						current_action = SKILL2;
						skill2CoolDown.start();
					}
					break;
				}
			}
		}
			break;

		case BASIC_ATTACK:
		{
			if (current_input_event == I_STOP)
			{
				current_action = IDLE;
			}
			else if (current_input_event == I_DIE)
			{
				current_action = DEATH;
			}
			else if (current_input_event == I_WALK)
			{
				current_action = WALKING;
			}
			else if (current_input_event == I_ATTACK)
			{
				current_action = BASIC_ATTACK;
			}
		}

			break;

		case DEATH:
		{
			if (current_input_event == I_STOP)
			{
				current_action = IDLE;
			}
		}

			break;

		case RUNNING:
		{
			if (current_input_event == I_STOP)
			{
				current_action = IDLE;
			}
			else if (current_input_event == I_WALK)
			{
				current_action = WALKING;
			}
			else if (current_input_event == I_DIE)
			{
				current_action = DEATH;
			}
			else if (current_input_event == I_ATTACK)
			{
				current_action = BASIC_ATTACK;
			}
			else if (current_input_event == I_SKILL)
			{
				//Maybe check here which hability is
				switch (skill)
				{
				case 1:
					if (attributes->getRage() >= furySkill1Cost && rageCoolDown.ReadSec() >= cooldownRageDuration)
					{
						current_action = SKILL1;
						rageCoolDown.start();
					}
					break;
				case 2:
					if (attributes->getRage() >= furySkill2Cost && skill2CoolDown.ReadSec() >= cooldownSkill2Duration)
					{
						current_action = SKILL2;
						skill2CoolDown.start();
					}
					break;
				}
				
			}
		}
			break;

		case SKILL1:
		{
			if (current_input_event == I_STOP)
			{
				current_action = IDLE;
			}
			else if (current_input_event == I_DIE)
			{
				current_action = DEATH;
			}
			else if (current_input_event == I_ATTACK)
			{
				current_action = BASIC_ATTACK;
			}
			else if (current_input_event == I_WALK)
			{
				current_action = WALKING;
			}
		}
			break;
		}


	}
	current_input_event = I_NULL;

	return current_action;
}

bool Player::Alive()
{
	return alive;
}

// Getters & Setters
iPoint Player::getMapPosition() const
{
	return mapPosition;
}

iPoint Player::getWorldPosition() const
{
	return worldPosition;
}

Sprite* Player::getSprite() const
{
	return sprite;
}

void Player::setMapPosition(iPoint tile)
{
	mapPosition = tile;
	setColliderPosition(app->map->MapToWorld(tile.x, tile.y));
}

void Player::setWorldPosition(iPoint coords)
{
	worldPosition = coords;
	setColliderPosition(coords);
}

iPoint Player::getStartingWorldPosition() const
{
	return startingPosition;
}

// Must use this to set respawn point.
void Player::setStartingWorldPosition(iPoint coords)
{
	startingPosition = coords;
}

//False to stop the player movement
void Player::setInputBlocked(bool value)
{
	inputBlocked = value;
}

//if entity is on range
bool Player::entityInRange()
{
	iPoint targetEntity;

	for (std::map<uint, Entity*>::iterator iterator = app->game->em->activeEntities.begin(); //es comproven totes cada frame meh. alguna manera millor hi haura
		iterator != app->game->em->activeEntities.end();
		iterator++)
	{
		if (iterator->second->getCollider())
		{
			targetEntity = iterator->second->getWorldPosition();
			iPoint dist;

			dist.x = targetEntity.x - getWorldPosition().x;
			dist.y = targetEntity.y - getWorldPosition().y;

			float range = sqrt(dist.x*dist.x + dist.y*dist.y);

			if (entityRange > range)
			{
				return true;
			}
		}
	}

	return false;
}

void Player::setColliderPosition(iPoint coords)
{
	collider->SetPos(coords.x - colliderOffset.x, coords.y - colliderOffset.y);
}
//Nonsense things
fPoint Player::getPivotPosition()
{
	fPoint ret{0, 0};

	return ret;
}

bool Player::getInputBlocked() const
{
	return inputBlocked;
}

void Player::setDirection()
{
	float angle = velocity.getAngle();

	DIRECTION dir;

	if (angle < 22.5 && angle > -22.5)
		dir = D_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = D_FRONT_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = D_FRONT;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = D_FRONT_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = D_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = D_BACK_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = D_BACK;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = D_BACK_RIGHT;

	if (dir != current_direction)
	{
		current_direction = dir;
	}

}

void Player::setDirection(fPoint pos)
{
	//NOTE: This has been created to change the direction without moving the player
	fPoint direction;
	direction.x = pos.x - worldPosition.x;
	direction.y = pos.y - worldPosition.y;

	direction.SetModule(1);

	float angle = direction.getAngle();

	DIRECTION dir;

	if (angle < 22.5 && angle > -22.5)
		dir = D_RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		dir = D_FRONT_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		dir = D_FRONT;
	else if (angle >= 112.5 && angle <= 157.5)
		dir = D_FRONT_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		dir = D_LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		dir = D_BACK_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		dir = D_BACK;
	else if (angle >= -67.5 && angle <= -22.5)
		dir = D_BACK_RIGHT;

	if (dir != current_direction)
	{
		current_direction = dir;
	}

}


bool Player::loadAnimations()
{
	bool ret = true;

	pugi::xml_document	anim_file;
	pugi::xml_node		anim;
	char* buff;
	int size = app->fs->Load("animations/player_animations.xml", &buff);
	pugi::xml_parse_result result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations");

	if (ret == true)
	{
		for (pugi::xml_node ent = anim.child("BARBARIAN"); ent != NULL; ent = ent.next_sibling())
		{
			for (pugi::xml_node action = ent.child("IDLE"); action != NULL; action = action.next_sibling())
			{
				for (pugi::xml_node dir = action.child("UP"); dir != action.child("loop"); dir = dir.next_sibling())
				{
					std::pair<ACTION_STATE, DIRECTION> p;
					int state = action.child("name").attribute("value").as_int();
					p.first = (ACTION_STATE)state;

					int di = dir.first_child().attribute("name").as_int();
					p.second = (DIRECTION)di;

					Animation anims;
					int x = dir.first_child().attribute("x").as_int();
					int y = dir.first_child().attribute("y").as_int();
					int w = dir.first_child().attribute("w").as_int();
					int h = dir.first_child().attribute("h").as_int();
					int fN = dir.first_child().attribute("frameNumber").as_int();
					int margin = dir.first_child().attribute("margin").as_int();
					bool loop = action.child("loop").attribute("value").as_bool();
					int pivotX = dir.first_child().attribute("pivot_x").as_int();
					int pivotY = dir.first_child().attribute("pivot_y").as_int();
					float animSpeed = action.child("speed").attribute("value").as_float();
					anims.setAnimation(x, y, w, h, fN, margin);
					anims.loop = loop;
					anims.speed = animSpeed;
					anims.pivot.x = pivotX;
					anims.pivot.y = pivotY;
					app->audio->PlayFx(walkFx);
					int entity = ent.child("name").attribute("value").as_int();
					iPoint piv;
					switch (entity)
					{
					case 0:
						barbarianAnim.insert(std::pair<std::pair<ACTION_STATE, DIRECTION>, Animation >(p, anims));
						barbarianAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
						piv = barbarianAnim.find({ p.first, p.second })->second.pivot;
						break;

					case 1:
						butcherAnim.insert(std::pair<std::pair<ACTION_STATE, DIRECTION>, Animation >(p, anims));
						butcherAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
						break;

					case 2:
						diabloAnim.insert(std::pair<std::pair<ACTION_STATE, DIRECTION>, Animation >(p, anims));
						diabloAnim.find({ p.first, p.second })->second.pivot.Set(pivotX, pivotY);
						break;
					}

				}
			}
		}
	}


	return ret;
}

void Player::setCurrentAnimation()
{
	switch (currentPhase)
	{
	case BARBARIAN:
		current_animation = &barbarianAnim.find({ current_action, current_direction })->second;
		break;
	case BUTCHER:
		current_animation = &butcherAnim.find({ current_action, current_direction })->second;
		break;
	case DIABLO:
		current_animation = &diabloAnim.find({ current_action, current_direction })->second;
		break;
	default:
		break;
	}
}