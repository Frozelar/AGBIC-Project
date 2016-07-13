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

#include "Window.h"
#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Graphics.h"
#include "Audio.h"

// directory and extension of level files
std::string Level::lDir = "Resources/";
std::string Level::lExt = ".map";

// numerical level ID
int Level::levelID = -1;

// level information
int Level::w = 0;
int Level::h = 0;
int Level::wPixels = 0;
int Level::hPixels = 0;
int Level::totalUnits = 0;
int Level::totalPixels = 0;

// numerical level background ID
int Level::bgID = 0;

// numerical level music ID
int Level::musicID = 0;

// int Level::moveX = 0;
// int Level::moveY = 0;

// 
Level::Level()
{
}

// call clearEntities()
Level::~Level()
{
	Game::clearEntities();
}

// generate given level, play music, initialize background, store level information
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

// clear entities, close level-specific graphics, clear level info
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

// perform automated process when level begins
bool Level::begin()
{
	bool quit = false;
	bool done = false;
	int curfps = 0;
	int target = Game::gPlayer->rect.y;
	Game::gPlayer->rect.y = -Game::gPlayer->rect.h;
	Game::gPlayer->aerialSpeed = Game::GRAVITY_START;

	while (!quit && !done)
	{
		curfps = SDL_GetTicks();
		Graphics::manageCamera();
		while (SDL_PollEvent(&Game::inputEvent) != NULL)
		{
			switch (Game::inputEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				Window::handleEvent(&Game::inputEvent);
				break;
			}
		}
		// Game::gPlayer->handleMovements();
		// Game::process();
		Graphics::renderAll();

		Game::gPlayer->aerialSpeed *= Game::GRAVITY_MULT;
		if (Game::gPlayer->aerialSpeed > Game::GRAVITY_MAX)
			Game::gPlayer->aerialSpeed = Game::GRAVITY_MAX;
		Game::gPlayer->rect.y += Game::gPlayer->aerialSpeed;

		if (Game::gPlayer->rect.y >= target)
			done = true;

		if (1000 / Game::FPS > SDL_GetTicks() - curfps)
			SDL_Delay((1000 / Game::FPS) - (SDL_GetTicks() - curfps));
	}
	Game::Mode = GAME;
	return quit;
}

// perform automated process when level is finished
bool Level::end()
{
	bool quit = false;
	bool done = false;
	int curfps = 0;
	Game::gPlayer->aerialSpeed = Game::JUMP_MAX;

	while (!quit && !done)
	{
		curfps = SDL_GetTicks();
		Graphics::manageCamera();
		while (SDL_PollEvent(&Game::inputEvent) != NULL)
		{
			switch (Game::inputEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				Window::handleEvent(&Game::inputEvent);
				break;
			}
		}
		// Game::gPlayer->handleMovements();
		// Game::process();
		Graphics::renderAll();

		Game::gPlayer->aerialSpeed *= Game::GRAVITY_MULT;
		if (Game::gPlayer->aerialSpeed <= Game::JUMP_START)
			Game::gPlayer->aerialSpeed = Game::JUMP_START;
		Game::gPlayer->rect.y += Game::gPlayer->aerialSpeed;

		if (Game::gPlayer->rect.y <= -Game::gPlayer->rect.h)
			done = true;

		if (1000 / Game::FPS > SDL_GetTicks() - curfps)
			SDL_Delay((1000 / Game::FPS) - (SDL_GetTicks() - curfps));
	}
	Game::Mode = MAP;
	return quit;
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

// return level width
// char = 'u' for width in units, or 'p' for width in pixels; returns 0 if invalid argument
int Level::getw(char which)
{
	if (which == 'u')
		return w;
	else if (which == 'p')
		return wPixels;
	return 0;
}

// return level height
// char = 'u' for height in units, or 'p' for height in pixels; returns 0 if invalid argument
int Level::geth(char which)
{
	if (which == 'u')
		return h;
	else if (which == 'p')
		return hPixels;
	return 0;
}

// return total units or pixels
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