#include "Game.h"
//#include "Entities.h"
#include "Player.h"

SDL_Event Game::inputEvent;
std::vector<StaticEntity*> Game::staticEntities;
std::vector<PhysicsEntity*> Game::dynamicEntities;
Player* Game::gPlayer = NULL;
std::vector<std::string> Game::blockTypes = { "White" };
const int Game::UNIT_W = 32;
const int Game::UNIT_H = 32;
const float Game::GRAVITY_START = 1;
const float Game::GRAVITY_MULT = 1.05;
const float Game::GRAVITY_MAX = 8;
const float Game::JUMP_MAX = -1;
const float Game::JUMP_MULT = 0.95;
const float Game::JUMP_START = -8;
const float Game::MOVE_SPEED = 2;
const float Game::ROTATION_SPEED = 2.5;
std::map<std::string, int> Game::Controls;

Game::Game()
{
	init();
}

Game::~Game()
{
	close();
}

bool Game::init()
{
	Controls["Move Left"] = SDLK_a;
	Controls["Move Right"] = SDLK_d;
	Controls["Jump"] = SDLK_w;
	gPlayer = new Player({ 100, 0, UNIT_W, UNIT_H });

	return true;
}

void Game::close()
{
	for (int i = 0; i < staticEntities.size(); i++)
	{
		if (staticEntities[i] != NULL)
		{
			delete staticEntities[i];
			staticEntities[i] = NULL;
		}
	}
	for (int i = 0; i < dynamicEntities.size(); i++)
	{
		if (dynamicEntities[i] != NULL)
		{
			delete dynamicEntities[i];
			dynamicEntities[i] = NULL;
		}
	}
	if (gPlayer != NULL)
	{
		delete gPlayer;
		gPlayer = NULL;
	}
}

// PhysicsEntity* = first colliding entity, bool = make entities resolve collision?
bool Game::checkCollision(PhysicsEntity* e1, bool resolveCollision, int index)
{
	bool collided = false;
	Entity* e2 = NULL;
	if (e1 != NULL)
	{
		for (int i = 0; i < staticEntities.size() || i < dynamicEntities.size(); i++)
		{
			if (i < staticEntities.size())
			{
				e2 = staticEntities[i];
				if (checkCollision(e1->rect, e2->rect))
				{
					if (!resolveCollision)
						return true;
					else
						collided = true;
					e1->collisions[findCollision(e1, e2->rect)] = e2;
				}
			}
			if (i < dynamicEntities.size() && i != index)
			{
				e2 = dynamicEntities[i];
				if (checkCollision(e1->rect, e2->rect))
				{
					if (!resolveCollision)
						return true;
					else
						collided = true;
					e1->collisions[findCollision(e1, e2->rect)] = e2;
				}
			}
		}
	}
	return collided;
}

bool Game::checkCollision(SDL_Rect r1, SDL_Rect r2)
{
	if ((r1.x + r1.w > r2.x && r1.x < r2.x + r2.w) && (r1.y + r1.h > r2.y && r1.y < r2.y + r2.h))
		return true;
	else
		return false;
}

int Game::findCollision(PhysicsEntity* e1, SDL_Rect r2)
{
	int dir = -1;
	SDL_Rect check = e1->rect;
	if (e1->moveSpeed != 0)
	{
		if (e1->moveSpeed > 0)
			dir = RIGHT;
		else if (e1->moveSpeed < 0)
			dir = LEFT;
		check.x -= e1->moveSpeed;
		if (checkCollision(check, r2))
			dir = -1;
	}
	check = e1->rect;
	if (e1->aerialSpeed != 0 && dir == -1)
	{
		if (e1->aerialSpeed > 0)
			dir = DOWN;
		else if (e1->aerialSpeed < 0)
			dir = UP;
		check.y -= e1->aerialSpeed;
		if (checkCollision(check, r2))
			dir = -1;
	}
	if (dir == -1)
	{
		dir = DOWN;
		std::cout << "COLLISION ERROR: findCollision(). Direction set to DOWN to avoid crashing." << std::endl;
	}
	return dir;
}

bool Game::newEntity(SDL_Rect box, int type, int subtype)
{
	if (type == STATIC_ENTITY || type == BLOCK)
		staticEntities.push_back(new StaticEntity(box, type, subtype));
	else if (type == PHYSICS_ENTITY || type == ENEMY)
		dynamicEntities.push_back(new PhysicsEntity(box, type, subtype));
	return true;
}