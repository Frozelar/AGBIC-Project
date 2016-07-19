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
#include "Enemy.h"
#include "Collectible.h"
#include "Graphics.h"
#include "Level.h"

// updated with user input
SDL_Event Game::inputEvent;

// game score
int Game::score = 0;

// store specified entities (allEntities owns every entity, the rest simply point to them)
std::vector<StaticEntity*> Game::allEntities;
std::vector<StaticEntity*> Game::staticEntities;
std::vector<PhysicsEntity*> Game::dynamicEntities;
std::vector<StaticEntity*> Game::renderedEntities;
std::vector<StaticEntity*> Game::collisionEntities;
std::vector<Collectible*> Game::collectibles;
std::vector<Enemy*> Game::enemies;

// player
Player* Game::gPlayer = NULL;

// identifiers for specified objects (used in graphic file loading)
std::vector<std::string> Game::blockIDs = { "White", "Goal" };
std::vector<std::string> Game::collectibleIDs = { "Coin", "Sprint", "HighJump", "DoubleJump", "Key" };
std::vector<std::string> Game::enemyIDs = { "Ice", "IceBoss", "Eye" };
std::vector<std::string> Game::particleIDs = { "Snow" };

// constant values
const int Game::FPS = 60;
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
const int Game::BOB_SPEED = 64;
const int Game::WARMUP_DURATION = 32;

// chances of an enemy spawning
const int Game::DEFAULT_ENEMY_SPAWN_CHANCE = 128;
int Game::enemySpawnChance = 128; //64;

// map of game controls
std::map<std::string, int> Game::Controls;

// offsets used in level loading
std::vector<int> Game::entityOffset = { 1, 1000, 2000, 3000 };	// player, block, enemy, collectible

// which state the game is in
GameMode Game::Mode = TITLE;

// std::vector<StaticEntity*> Game::destroyBuffer;

// call init()
Game::Game()
{
	init();
}

// call close()
Game::~Game()
{
	close();
}

// seed rng, initialize controls, create player object
bool Game::init()
{
	srand(time(NULL));
	Controls["Move Left"] = SDLK_a;
	Controls["Move Right"] = SDLK_d;
	Controls["Jump"] = SDLK_w;
	Controls["Pause"] = SDLK_p;
	gPlayer = new Player({ 100, 0, UNIT_W, UNIT_H });

	return true;
}

// delete all entities and delete player
void Game::close()
{
	clearEntities();
	if (gPlayer != NULL)
	{
		delete gPlayer;
		gPlayer = NULL;
	}
}

// check for any collisions with given object
// PhysicsEntity* = first colliding entity, bool = make entities resolve collision?, index = index of entity in collisionEntities[] (if applicable)
bool Game::checkCollision(PhysicsEntity* e1, bool resolveCollision, int index)
{
	bool collided = false;
	StaticEntity* e2 = NULL;
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

// return whether or not the two rectangles are colliding
bool Game::checkCollision(SDL_Rect r1, SDL_Rect r2)
{
	if ((r1.x + r1.w > r2.x && r1.x < r2.x + r2.w) && (r1.y + r1.h > r2.y && r1.y < r2.y + r2.h))
		return true;
	else
		return false;
}

// detect which direction the two given objects are colliding in, and store this info in the first entity
int Game::findCollision(PhysicsEntity* e1, SDL_Rect r2)
{
	int dir = -1;
	SDL_Rect check = e1->rect;

	if (e1->aerialSpeed > 0 && r2.y >= e1->rect.y + e1->rect.h - e1->aerialSpeed)
		dir = DOWN;
	else if (e1->aerialSpeed < 0 && r2.y + r2.h <= e1->rect.y - e1->aerialSpeed)
		dir = UP;

	if (e1->moveSpeed > 0 && r2.x >= e1->rect.x + e1->rect.w - e1->moveSpeed)
		dir = RIGHT;
	else if (e1->moveSpeed < 0 && r2.x + r2.w <= e1->rect.x - e1->moveSpeed)
		dir = LEFT;

	if (dir == -1)
		dir = UP; // DOWN;

	/*
	if (e1->aerialSpeed != 0)// || e1->moveSpeed != 0)
	{
		if (e1->aerialSpeed > 0)
			dir = DOWN;
		else if (e1->aerialSpeed < 0)
			dir = UP;
		check.y -= e1->aerialSpeed;
		// check.x -= e1->moveSpeed;
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
		// check.y -= e1->aerialSpeed;
		check.x -= e1->moveSpeed;
		if (checkCollision(check, r2))
			dir = -1;
	}
	if (dir == -1)
	{
		dir = DOWN;
		// std::cout << "COLLISION ERROR: findCollision(). Direction set to DOWN to avoid crashing." << std::endl;
	}
	*/
	return dir;
}

// create new entity
// SDL_Rect = rect for entity, int = entity type, int = subtype
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
		SDL_Rect actualbox = { box.x, box.y, box.w, box.h };
		allEntities.push_back(new Enemy(actualbox, subtype));
		dynamicEntities.push_back(static_cast<PhysicsEntity*>(allEntities.back()));
		renderedEntities.push_back(static_cast<PhysicsEntity*>(allEntities.back()));
		collisionEntities.push_back(static_cast<PhysicsEntity*>(allEntities.back()));
		enemies.push_back(static_cast<Enemy*>(allEntities.back()));
		enemies.back()->dynamicEntitiesIndex = dynamicEntities.size() - 1;
	}
	else if (type == COLLECTIBLE)
	{
		allEntities.push_back(new Collectible(box, subtype));
		staticEntities.push_back(static_cast<StaticEntity*>(allEntities.back()));
		renderedEntities.push_back(static_cast<StaticEntity*>(allEntities.back()));
		collisionEntities.push_back(static_cast<StaticEntity*>(allEntities.back()));
		collectibles.push_back(static_cast<Collectible*>(allEntities.back()));
	}
	else
		return false;
	return true;
}

// clear all entities
void Game::clearEntities(void)
{
	for (int i = collectibles.size() - 1; i >= 0; i--)
	{
		//if (collectibles[i] != NULL)
		//	collectibles[i] = NULL;
		collectibles.pop_back();
	}
	for (int i = enemies.size() - 1; i >= 0; i--)
	{
		//if (enemies[i] != NULL)
		//	enemies[i] = NULL;
		enemies.pop_back();
	}
	for (int i = staticEntities.size() - 1; i >= 0; i--)
	{
		//if (staticEntities[i] != NULL)
		//	staticEntities[i] = NULL;
		staticEntities.pop_back();
	}
	for (int i = dynamicEntities.size() - 1; i >= 0; i--)
	{
		//if (dynamicEntities[i] != NULL)
		//	dynamicEntities[i] = NULL;
		dynamicEntities.pop_back();
	}
	for (int i = renderedEntities.size() - 1; i >= 0; i--)
	{
		//if (renderedEntities[i] != NULL)
		//	renderedEntities[i] = NULL;
		renderedEntities.pop_back();
	}
	for (int i = collisionEntities.size() - 1; i >= 0; i--)
	{
		//if (collisionEntities[i] != NULL)
		//	collisionEntities[i] = NULL;
		collisionEntities.pop_back();
	}
	for (int i = allEntities.size() - 1; i >= 0; i--)
	{
		//if (allEntities[i] != NULL)
		//{
		//	delete allEntities[i];
		//	allEntities[i] = NULL;
		//}
		allEntities.pop_back();
	}
}

// called each frame
// destroy entity if needed and call onProcess() for each entity
void Game::process()
{
	for (int i = 0; i < collectibles.size(); i++)
	{
		if (collectibles[i] != NULL)
		{
			if (collectibles[i]->destroy)
			{
				collectibles[i]->onDestroy();
				delete collectibles[i];
				collectibles[i] = NULL;
				collectibles.erase(collectibles.begin() + i);
			}
			else
			{
				collectibles[i]->onProcess();
			}
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i] != NULL)
		{
			if (enemies[i]->destroy)
			{
				enemies[i]->onDestroy();
				dynamicEntities.erase(dynamicEntities.begin() + enemies[i]->dynamicEntitiesIndex);
				delete enemies[i];
				enemies[i] = NULL;
				enemies.erase(enemies.begin() + i);
			}
			else
			{
				enemies[i]->onProcess();
			}
		}
	}
	for (int i = 0; i < dynamicEntities.size(); i++)
	{
		if (dynamicEntities[i] != NULL && !dynamicEntities[i]->destroy)
		{
			dynamicEntities[i]->handleMovements();
			dynamicEntities[i]->onProcess();
		}
	}
	if (enemySpawnChance > 0)
	{
		if (rand() % enemySpawnChance == 1)
		{
			SDL_Rect box = { 0, 0, UNIT_W, UNIT_H };
			SDL_Rect vp = Graphics::getViewport();
			box.x = rand() % (gPlayer->rect.x + vp.w) + (gPlayer->rect.x - vp.w);
			box.w /= (rand() % 2 == 0 ? 2 : 1);
			box.h = box.w;
			box.y = -box.h;
			newEntity(box, ENEMY, ICE);
			dynamicEntities.back()->moveSpeed = Game::MOVE_SPEED * (rand() % 2 + (-1));
		}
	}

	if (Level::getID() == 0)
	{
		if (gPlayer->rect.y > Level::geth('p'))
		{
			Level::generateLevel(1);
			Game::Mode = LEVEL_BEGIN;
		}
	}
	else if (Level::getID() == 1)
	{
		if (gPlayer->rect.y + gPlayer->rect.h < 0)
		{
			Level::generateLevel(0);
			Game::Mode = LEVEL_BEGIN;
		}
	}
}

/*
void Game::manageBuffers()
{
	for (int i = 0; i < destroyBuffer.size(); i++)
	{
		if (destroyBuffer[i] != NULL)
		{
			delete destroyBuffer[i];
			destroyBuffer[i] = NULL;
		}
		destroyBuffer.pop_back();
	}
}
*/