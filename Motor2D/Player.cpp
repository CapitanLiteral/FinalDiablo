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

Player::Player()
{
	AttributeBuilder builder;
	attributes = new PlayerAttributes(builder);
	
	worldPosition.x = 0;
	worldPosition.y = 0;
	mapPosition.x = 0;
	mapPosition.y = 0;

	colliderOffset.SetToZero();
	colliderSize.SetToZero();

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
	SDL_Rect rect = { worldPosition.x - colliderOffset.x, 
					  worldPosition.y - colliderOffset.y,	// Position
					  colliderSize.x, colliderSize.y};		// Size
	collider = app->collision->addCollider(rect, COLLIDER_PLAYER, this);

	// ANIMATION
	if (loadAnimations())
	{
		//Sprites
		pDebug = app->tex->Load("mini_path.png");

		barbarianImage = app->tex->Load("images/Barbarian.png");
		butcherImage = app->tex->Load("images/Butcher.png");
		diabloImage = app->tex->Load("images/Diablo.png");

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

	return ret;
}
bool Player::preUpdate()
{
	handleInput();

	updateAction();

	return true;
}

bool Player::update(float dt)
{
	bool ret = true;

	app->render->CenterCamera(worldPosition.x, worldPosition.y);

	updateMovement(dt);

	if (current_action != DEATH)
	{
		switch (current_action)
		{
		case IDLE:
			break;
		case WALKING:
			//updateMovement(dt);
			break;
		case RUNNING:
			//updateMovement(dt);
			//LowerStamina();
			break;
		case SKILL:
			break;

		}
	}
	else
	{
		respawn();
	}

	return ret;
}

void Player::respawn()
{
	worldPosition = startingPosition;
}

void Player::draw()
{
	sprite->updateSprite(worldPosition,
						current_animation->pivot,
						current_animation->getCurrentFrame());
}

vector<iPoint> Player::getNewPath(iPoint target)
{
	iPoint start = app->map->WorldToMap(worldPosition.x, worldPosition.y);
	iPoint goal = target;
	vector<iPoint> _path;
	int steps = app->pathfinding->getNewPath(start, goal, _path);

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
}
void Player::move(float dt)
{
	fPoint vel = velocity * dt;

	worldPosition.x += int(vel.x);
	worldPosition.y += int(vel.y);

	//NOTE: Collider movement, may be changed
	collider->SetPos(worldPosition.x, worldPosition.y); //Maybe ERROR, watch out
}
bool Player::isTargetReached()
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

Player::~Player()
{
	RELEASE(attributes);
}

void Player::handleInput()
{
	if (!inputBlocked)
	{
		if (current_action != DEATH)
		{
			if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
			{
				//Do skill
			}
			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				//Maybe ERROR, watch out //Only to test pathfinding
				target = app->input->getMouseWorldPosition();
				target = app->map->WorldToMap(target.x, target.y);
				path = getNewPath(target);

				//Do things
				if (app->input->getKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
				{
					//Sprint
					current_input_event = I_RUN;
				}
				else
				{
					//Move
					//Attack
				}
				
			}
		}
	}
}

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
				//Maybe check here which hability is
				current_action = SKILL;
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
				current_action = SKILL;
			}
		}
			break;

		case SKILL:
		{
			if (current_input_event == I_STOP)
			{
				current_action = IDLE;
			}
			else if (current_input_event == I_DIE)
			{
				current_action = DEATH;
			}
		}
			break;
		}

		if (previous_action != current_action)
		{
			//SetDirection();
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

		previous_action = current_action;

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

/*void Player::SetDirection()
{
	float angle = p_velocity.getAngle();

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

}*/

void Player::SetDirection(fPoint pos)
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