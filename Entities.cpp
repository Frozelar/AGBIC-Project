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
	// gfxRect = { pbox.x - Graphics::GFX_OFFSET * 2, pbox.y - Graphics::GFX_OFFSET * 2, pbox.w + Graphics::GFX_OFFSET, pbox.h + Graphics::GFX_OFFSET };
}

StaticEntity::~StaticEntity()
{
}

/*
void StaticEntity::syncGFX()
{
	gfxRect = { rect.x - Graphics::GFX_OFFSET * 2, rect.y - Graphics::GFX_OFFSET * 2, rect.w + Graphics::GFX_OFFSET, rect.h + Graphics::GFX_OFFSET };
}
*/

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

// Move the entity and manage collisions
void PhysicsEntity::handleMovements()
{
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

// Manage aerialSpeed
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