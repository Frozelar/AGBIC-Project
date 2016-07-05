/*
Ice Age: A platformer game being made as a hobby.  Made for the 'A Game By Its Cover 2016' game jam.
Copyright (C) 2016 Frozelar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Game.h"
//#include "Entities.h"
#include "Player.h"

SDL_Event Game::inputEvent;
std::vector<StaticEntity*> Game::allEntities;
std::vector<StaticEntity*> Game::staticEntities;
std::vector<PhysicsEntity*> Game::dynamicEntities;
std::vector<StaticEntity*> Game::renderedEntities;
std::vector<StaticEntity*> Game::collisionEntities;
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
const int Game::MOVE_SPEED = 2;
const float Game::ROTATION_SPEED = 2.5;
std::map<std::string, int> Game::Controls;
std::vector<int> Game::entityOffset = { 1, 1000, 2000, 3000 };	// player, block, enemy, collectible

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
	srand(time(NULL));
	Controls["Move Left"] = SDLK_a;
	Controls["Move Right"] = SDLK_d;
	Controls["Jump"] = SDLK_w;
	gPlayer = new Player({ 100, 0, UNIT_W, UNIT_H });

	return true;
}

void Game::close()
{
	clearEntities();
	if (gPlayer != NULL)
	{
		delete gPlayer;
		gPlayer = NULL;
	}
}

// PhysicsEntity* = first colliding entity, bool = make entities resolve collision?, index = index of entity in collisionEntities[] (if applicable)
bool Game::checkCollision(PhysicsEntity* e1, bool resolveCollision, int index)
{
	bool collided = false;
	Entity* e2 = NULL;
	if (e1 != NULL)
	{
		for (int i = 0; i < collisionEntities.size(); i++)
		{
			if (i < collisionEntities.size() && i != index)
			{
				e2 = collisionEntities[i];
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

/*
// PhysicsEntity* = first colliding entity, bool = make entities resolve collision?, index = index of entity in physicsEntities[] (if applicable)
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
*/

// Return whether or not the two rectangles are colliding
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
	if (e1->aerialSpeed != 0 || e1->moveSpeed != 0)
	{
		if (e1->aerialSpeed > 0)
			dir = DOWN;
		else if (e1->aerialSpeed < 0)
			dir = UP;
		check.y -= e1->aerialSpeed;
		if (checkCollision(check, r2))
			dir = -1;
	}
	check = e1->rect;
	if (e1->moveSpeed != 0 && dir == -1)
	{
		if (e1->moveSpeed > 0)
			dir = RIGHT;
		else if (e1->moveSpeed < 0)
			dir = LEFT;
		check.x -= e1->moveSpeed;
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
	if (/*type == STATIC_ENTITY ||*/ type == BLOCK)
	{
		allEntities.push_back(new StaticEntity(box, type, subtype));
		staticEntities.push_back(static_cast<StaticEntity*>(allEntities.back()));
		renderedEntities.push_back(static_cast<StaticEntity*>(allEntities.back()));
		collisionEntities.push_back(static_cast<StaticEntity*>(allEntities.back()));
	}
	else if (/*type == PHYSICS_ENTITY ||*/ type == ENEMY)
	{
		allEntities.push_back(new PhysicsEntity(box, type, subtype));
		dynamicEntities.push_back(static_cast<PhysicsEntity*>(allEntities.back()));
		renderedEntities.push_back(static_cast<PhysicsEntity*>(allEntities.back()));
		collisionEntities.push_back(static_cast<PhysicsEntity*>(allEntities.back()));
	}
	else
		return false;
	return true;
}

void Game::clearEntities(void)
{
	for (int i = staticEntities.size() - 1; i >= 0; i--)
	{
		if (staticEntities[i] != NULL)
			staticEntities[i] = NULL;
		staticEntities.pop_back();
	}
	for (int i = dynamicEntities.size() - 1; i >= 0; i--)
	{
		if (dynamicEntities[i] != NULL)
			dynamicEntities[i] = NULL;
		dynamicEntities.pop_back();
	}
	for (int i = renderedEntities.size() - 1; i >= 0; i--)
	{
		if (renderedEntities[i] != NULL)
			renderedEntities[i] = NULL;
		renderedEntities.pop_back();
	}
	for (int i = collisionEntities.size() - 1; i >= 0; i--)
	{
		if (collisionEntities[i] != NULL)
			collisionEntities[i] = NULL;
		collisionEntities.pop_back();
	}
	for (int i = allEntities.size() - 1; i >= 0; i--)
	{
		if (allEntities[i] != NULL)
		{
			delete allEntities[i];
			allEntities[i] = NULL;
		}
		allEntities.pop_back();
	}
}