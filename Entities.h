#pragma once
#include "Include.h"

enum EntityType {
	ENTITY, STATIC_ENTITY, PHYSICS_ENTITY, PLAYER, BLOCK, ENEMY, COLLECTIBLE
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
	virtual void syncGFX(void);
	int getType(void);
	int getSubtype(void);

	SDL_Rect gfxRect;
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
	virtual void handleCollisions(void);

	float moveSpeed;
	float aerialSpeed;
	std::vector<Entity*> collisions;
};