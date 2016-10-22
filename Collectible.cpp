#include "Collectible.h"
#include "Player.h"
#include "Game.h"
#include "Graphics.h"
#include "Audio.h"
#include "Message.h"

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
	Audio::play(COLLECT, 's');
	switch (subtype)
	{
	case C1:
		Game::gScore += 1;
		// Graphics::particleDensity += 0.1;
		// Game::enemySpawnChance -= 4;
		break;
	case C5:
		Game::gScore += 5;
		break;
	case C10:
		Game::gScore += 10;
		break;
	case C50:
		Game::gScore += 50;
		break;
	case C100:
		Game::gScore += 100;
		break;
	case SPRINT:
		Game::gScore += 1;
		Graphics::particleDensity += 0.3;
		Game::enemySpawnChance -= 8;
		Game::gPlayer->abilities["Sprint"] = true;
		break;
	case HIGH_JUMP:
		Game::gScore += 1;
		Graphics::particleDensity += 0.3;
		Game::enemySpawnChance -= 8;
		Game::gPlayer->abilities["High Jump"] = true;
		break;
	case DOUBLE_JUMP:
		Game::gScore += 1;
		Graphics::particleDensity += 0.3;
		Game::enemySpawnChance -= 8;
		Game::gPlayer->abilities["Double Jump"] = true;
		break;
	case KEY:
		Game::gScore += 5;
		Graphics::particleDensity += 0.5;
		Game::enemySpawnChance -= 12;
		Game::gPlayer->abilities["Key"] = true;
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