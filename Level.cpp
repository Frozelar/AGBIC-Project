#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Graphics.h"
#include "Audio.h"

std::string Level::lDir = "Resources/";
std::string Level::lExt = ".map";
int Level::levelID = -1;
int Level::w = 0;
int Level::h = 0;
int Level::wPixels = 0;
int Level::hPixels = 0;
int Level::totalUnits = 0;
int Level::totalPixels = 0;
int Level::bgID = 0;
int Level::musicID = 0;
// int Level::moveX = 0;
// int Level::moveY = 0;

Level::Level()
{
}

Level::~Level()
{
	Game::clearEntities();
}

bool Level::generateLevel(int which)
{
	int x = 0, y = 0;
	int unitType = -1;
	std::ifstream levelMap;
	// SDL_Rect playerRect{ 0, 0, Game::UNIT_W, Game::UNIT_H };
	SDL_Rect entityRect{ 0, 0, Game::UNIT_W, Game::UNIT_H };

	closeLevel();
	if (which >= 0)
		levelMap.open(lDir + "level" + std::to_string(which) + lExt);
	levelID = which;

	levelMap >> unitType;
	w = unitType;
	wPixels = w * Game::UNIT_W;
	levelMap >> unitType;
	h = unitType;
	hPixels = h * Game::UNIT_H;
	totalUnits = w * h;
	totalPixels = totalUnits * Game::UNIT_W;

	levelMap >> unitType;
	bgID = unitType;
	levelMap >> unitType;
	musicID = unitType;

	for (int i = 0; i < totalPixels;)
	{
		unitType = -1;

		levelMap >> unitType;

		if (levelMap.fail())
		{
			std::cout << "Failed to load " << lDir << "level" << std::to_string(which) << lExt << "!  Darn." << std::endl;
			levelMap.close();
			return false;
		}

		if (unitType != -1)
		{
			if (unitType != 0)
			{
				entityRect.x = x;
				entityRect.y = y;
				if (unitType > PLAYER)
					Game::newEntity(entityRect, floor(unitType / 1000) + 1, unitType - Game::entityOffset[floor(unitType / 1000)]);
				else
					Game::gPlayer->rect = entityRect;
			}

			i += Game::UNIT_W;

			x += Game::UNIT_W;
			if (x >= wPixels)
			{
				x = 0;
				y += Game::UNIT_H;
			}
		}
	}
	Audio::play(musicID, 'm');
	Graphics::loadBG(bgID);

	levelMap.close();
	return true;
}

void Level::closeLevel(void)
{
	Game::clearEntities();
	Graphics::closeLevelGFX();
	levelID = -1;
	musicID = 0;
	bgID = 0;
	w = wPixels = 0;
	h = hPixels = 0;
	totalUnits = totalPixels = 0;
}

/*
void Level::moveLevel(int mX, int mY)
{
	if (mX != 0 || mY != 0)
	{
		moveX += mX;
		moveY += mY;
	}
	else
	{

	}
}
*/

// char = 'u' for width in units, or 'p' for width in pixels; returns 0 if invalid argument
int Level::getw(char which)
{
	if (which == 'u')
		return w;
	else if (which == 'p')
		return wPixels;
	return 0;
}

// char = 'u' for height in units, or 'p' for height in pixels; returns 0 if invalid argument
int Level::geth(char which)
{
	if (which == 'u')
		return h;
	else if (which == 'p')
		return hPixels;
	return 0;
}

// char = 'u' for total level units, or 'p' for total level pixels; returns 0 if invalid argument
int Level::getTotal(char which)
{
	if (which == 'u')
		return totalUnits;
	else if (which == 'p')
		return totalPixels;
	return 0;
}

// return the level's ID
int Level::getID()
{
	return levelID;
}

// return the level's background ID
int Level::getBGID()
{
	return bgID;
}

// return the level's music ID

int Level::getMusicID()
{
	return musicID;
}