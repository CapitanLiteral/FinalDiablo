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
}

bool Player::update(float dt)
{
	bool ret = true;

	app->render->CenterCamera(worldPosition.x, worldPosition.y);



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

bool Player::Alive()
{
	return alive;
}

// Getters & Setters
iPoint Player::getMapPosition() const
{
	return worldPosition;
}

iPoint Player::getWorldPosition() const
{
	return mapPosition;
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