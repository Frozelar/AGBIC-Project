#include "Collectible.h"
#include "Game.h"

// initialize variables
Collectible::Collectible(SDL_Rect box, int type) : StaticEntity(box, COLLECTIBLE, type)
{
	bobSpeed = Game::BOB_SPEED;
	direction = UP;
}

// 
Collectible::~Collectible()
{
}

// called once when entity is deleted
void Collectible::onDestroy()
{
	switch (subtype)
	{
	case COIN:
		Game::score += 1;
		break;
	}
}

// called each frame
// make collectible bob in air
void Collectible::onProcess()
{
	bobSpeed *= 0.5;
	if (bobSpeed == 0)
	{
		if (direction == UP)
			bobSpeed = -Game::BOB_SPEED;
		else if (direction == DOWN)
			bobSpeed = Game::BOB_SPEED;
		direction = invertDir(direction);
	}
	if(abs(bobSpeed) % (Game::BOB_SPEED / 8) == 0)
		rect.y += ceil(bobSpeed / (Game::BOB_SPEED / 2));
}