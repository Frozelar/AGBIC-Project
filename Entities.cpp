#include "Entities.h"
#include "Graphics.h"
#include "Game.h"

Entity::Entity(SDL_Rect box)
{
	rect = box;
}

Entity::~Entity()
{
}

StaticEntity::StaticEntity(SDL_Rect pbox, int ptype, int psubtype) : Entity(pbox)
{
	entityType = ptype;
	subtype = psubtype;
	gfxRect = { pbox.x - Graphics::GFX_OFFSET * 2, pbox.y - Graphics::GFX_OFFSET * 2, pbox.w + Graphics::GFX_OFFSET, pbox.h + Graphics::GFX_OFFSET };
}

StaticEntity::~StaticEntity()
{
}

void StaticEntity::syncGFX()
{
	gfxRect = { rect.x - Graphics::GFX_OFFSET * 2, rect.y - Graphics::GFX_OFFSET * 2, rect.w + Graphics::GFX_OFFSET, rect.h + Graphics::GFX_OFFSET };
}

int StaticEntity::getType()
{
	return entityType;
}

int StaticEntity::getSubtype()
{
	return subtype;
}

/*
void StaticEntity::render()
{
	
}
*/

PhysicsEntity::PhysicsEntity(SDL_Rect pbox, int ptype, int psubtype) : StaticEntity(pbox, ptype, psubtype)
{
	moveSpeed = 0;
	aerialSpeed = 0;
	for (int i = 0; i < TOTAL_DIRECTIONS; i++)
		collisions.push_back(NULL);
}

PhysicsEntity::~PhysicsEntity()
{
}

void PhysicsEntity::handleMovements()
{
	if (moveSpeed != 0)
		rect.x += moveSpeed;
	if(aerialSpeed != 0)
		rect.y += aerialSpeed;
	cycleAerials();

	if (collisions[DOWN] == NULL && aerialSpeed == 0)
		aerialSpeed = Game::GRAVITY_START;
	if (Game::checkCollision(this))
		handleCollisions();
}

void PhysicsEntity::cycleAerials()
{
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

void PhysicsEntity::handleCollisions()
{
	if (collisions[UP] != NULL && aerialSpeed < 0)
	{
		aerialSpeed = 0;
		while (Game::checkCollision(rect, collisions[UP]->rect))
			rect.y++;
		collisions[UP] = NULL;
	}

	if (collisions[DOWN] != NULL && aerialSpeed > 0)
	{
		aerialSpeed = 0;
		while (Game::checkCollision(rect, collisions[DOWN]->rect))
			rect.y--;
		rect.y++;
		collisions[DOWN] = NULL;
	}

	if (collisions[LEFT] != NULL && moveSpeed < 0)
	{
		// moveSpeed = 0;
		while (Game::checkCollision(rect, collisions[LEFT]->rect))
			rect.x++;
		collisions[LEFT] = NULL;
	}

	if (collisions[RIGHT] != NULL && moveSpeed > 0)
	{
		// moveSpeed = 0;
		while (Game::checkCollision(rect, collisions[RIGHT]->rect))
			rect.x--;
		collisions[RIGHT] = NULL;
	}
}