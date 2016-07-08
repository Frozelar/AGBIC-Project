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

#include "Entities.h"
#include "Graphics.h"
#include "Game.h"
#include "Audio.h"

// entity rect = passed rect
Entity::Entity(SDL_Rect box)
{
	rect = box;
}

// 
Entity::~Entity()
{
}

// initialize variables
StaticEntity::StaticEntity(SDL_Rect pbox, int ptype, int psubtype) : Entity(pbox)
{
	entityType = ptype;
	subtype = psubtype;
	destroy = false;
	// gfxRect = { pbox.x - Graphics::GFX_OFFSET * 2, pbox.y - Graphics::GFX_OFFSET * 2, pbox.w + Graphics::GFX_OFFSET, pbox.h + Graphics::GFX_OFFSET };
}

// call onDestroy()
StaticEntity::~StaticEntity()
{
	onDestroy();
}

/*
void StaticEntity::syncGFX()
{
	gfxRect = { rect.x - Graphics::GFX_OFFSET * 2, rect.y - Graphics::GFX_OFFSET * 2, rect.w + Graphics::GFX_OFFSET, rect.h + Graphics::GFX_OFFSET };
}
*/

// return entityType
int StaticEntity::getType()
{
	return entityType;
}

// return subtype
int StaticEntity::getSubtype()
{
	return subtype;
}

// 
void StaticEntity::onProcess()
{
}

// 
void StaticEntity::onDestroy()
{
}

/*
void StaticEntity::render()
{
	
}
*/

// initialize members
PhysicsEntity::PhysicsEntity(SDL_Rect pbox, int ptype, int psubtype) : StaticEntity(pbox, ptype, psubtype)
{
	moveSpeed = 0;
	aerialSpeed = 0;
	for (int i = 0; i < TOTAL_DIRECTIONS; i++)
		collisions.push_back(NULL);
}

// 
PhysicsEntity::~PhysicsEntity()
{
}

// move entity horizontally and vertically, and manage collisions
void PhysicsEntity::handleMovements()
{
	if (collisions[DOWN] == NULL && aerialSpeed == 0)
		aerialSpeed = Game::GRAVITY_START;
	if (moveSpeed != 0)
		rect.x += moveSpeed;
	if (aerialSpeed != 0)
		rect.y += aerialSpeed;
	cycleAerials();
	/*
	if (moveSpeed != 0)
		rect.x += moveSpeed;
	if (Game::checkCollision(this))
	{
		if (collisions[LEFT] != NULL)
			while (Game::checkCollision(rect, collisions[LEFT]->rect))
				rect.x++;
		if (collisions[RIGHT] != NULL)
			while (Game::checkCollision(rect, collisions[RIGHT]->rect))
				rect.x--;
		collisions[LEFT] = NULL;
		collisions[RIGHT] = NULL;
	}
	if (aerialSpeed != 0)
		rect.y += aerialSpeed;
	cycleAerials();
	if (collisions[DOWN] != NULL)
	{
		rect.y++;
		if (!Game::checkCollision(rect, collisions[DOWN]->rect))
			collisions[DOWN] = NULL;
		rect.y--;
	}
	else if (collisions[DOWN] == NULL && aerialSpeed == 0)
		aerialSpeed = Game::GRAVITY_START;
	if (Game::checkCollision(this))
	{
		aerialSpeed = 0;
		if (collisions[UP] != NULL)
			while (Game::checkCollision(rect, collisions[UP]->rect))
				rect.y++;
		if (collisions[DOWN] != NULL)
			while (Game::checkCollision(rect, collisions[DOWN]->rect))
				rect.y--;
		collisions[UP] = NULL;
		// collisions[DOWN] = NULL;
	}
	*/
	
	/*
	if (moveSpeed != 0)
		rect.x += moveSpeed;
	if(aerialSpeed != 0)
		rect.y += aerialSpeed;
	cycleAerials();

	if (collisions[DOWN] == NULL && aerialSpeed == 0)
		aerialSpeed = Game::GRAVITY_START;
	if (Game::checkCollision(this))
		handleCollisions();
	if (collisions[DOWN] != NULL)
	{
		rect.y++;
		if (!Game::checkCollision(rect, collisions[DOWN]->rect))
			collisions[DOWN] = NULL;
		rect.y--;
	}
	if (collisions[LEFT] != NULL)
		if (!Game::checkCollision(rect, collisions[LEFT]->rect))
			collisions[LEFT] = NULL;
	if (collisions[RIGHT] != NULL)
		if (!Game::checkCollision(rect, collisions[RIGHT]->rect))
			collisions[RIGHT] = NULL;
	if (collisions[UP] != NULL)
		if (!Game::checkCollision(rect, collisions[UP]->rect))
			collisions[UP] = NULL;
			*/
}

// manage aerialSpeed
void PhysicsEntity::cycleAerials()
{
	if (aerialSpeed == Game::JUMP_START)
		Audio::play(JUMP, 's');
	if (aerialSpeed < 0)
	{
		aerialSpeed *= Game::JUMP_MULT;
		if (aerialSpeed >= Game::JUMP_MAX)
			aerialSpeed = Game::GRAVITY_START;
	}
	else if (aerialSpeed > 0)
	{
		aerialSpeed *= Game::GRAVITY_MULT;
		if (aerialSpeed > Game::GRAVITY_MAX)
			aerialSpeed = Game::GRAVITY_MAX;
	}
}

/*
void PhysicsEntity::handleCollisions()
{
	if (collisions[UP] != NULL && aerialSpeed < 0)
	{
		aerialSpeed = 0;
		while (Game::checkCollision(rect, collisions[UP]->rect))
			rect.y++;
		// collisions[UP] = NULL;
	}

	if (collisions[DOWN] != NULL && aerialSpeed > 0)
	{
		aerialSpeed = 0;
		while (Game::checkCollision(rect, collisions[DOWN]->rect))
			rect.y--;
		// rect.y++;
		// collisions[DOWN] = NULL;
	}

	if (collisions[LEFT] != NULL && moveSpeed < 0)
	{
		// moveSpeed = 0;
		while (Game::checkCollision(rect, collisions[LEFT]->rect))
			rect.x++;
		// collisions[LEFT] = NULL;
	}

	if (collisions[RIGHT] != NULL && moveSpeed > 0)
	{
		// moveSpeed = 0;
		while (Game::checkCollision(rect, collisions[RIGHT]->rect))
			rect.x--;
		// collisions[RIGHT] = NULL;
	}
}
*/