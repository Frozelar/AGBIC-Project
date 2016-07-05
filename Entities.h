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

#pragma once
#include "Include.h"

/*
enum EntityType {
	ENTITY, STATIC_ENTITY, PHYSICS_ENTITY
};
*/

enum EntityType {
	NO_ENTITY, PLAYER, BLOCK, ENEMY, COLLECTIBLE
};

enum BlockType {
	WHITE, TOTAL_BLOCK_TYPES
};

class Entity
{
public:
	Entity(SDL_Rect);
	~Entity();

	SDL_Rect rect;
};

class StaticEntity : public Entity
{
public:
	StaticEntity(SDL_Rect, int, int);
	~StaticEntity();
	
	// virtual void render(void);
	// virtual void syncGFX(void);
	int getType(void);
	int getSubtype(void);

private:
	int entityType;
	int subtype;
};

class PhysicsEntity : public StaticEntity
{
public:
	PhysicsEntity(SDL_Rect, int, int);
	~PhysicsEntity();

	virtual void handleMovements(void);
	virtual void cycleAerials(void);
	// virtual void handleCollisions(void);

	float moveSpeed;
	float aerialSpeed;
	std::vector<Entity*> collisions;
};