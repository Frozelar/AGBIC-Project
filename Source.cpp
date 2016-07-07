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

#include "Include.h"
#include "Window.h"
#include "Game.h"
#include "Graphics.h"
#include "Player.h"
#include "Level.h"
#include "Audio.h"

// create static classes
Window gWindow;
Game gGame;
Graphics gGraphics;
Level gLevel;
Audio gAudio;

int main(int argc, char** argv)
{
	bool quit = false;
	int curlevel = 0;

	Game::Mode = LEVEL_BEGIN;

	while (!quit)
	{
		if (Level::getID() == -1)
			Level::generateLevel(curlevel++);

		if (Game::Mode == LEVEL_BEGIN)
			quit = Level::begin();
		else if (Game::Mode == LEVEL_END)
			quit = Level::end();

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
			default:
				Game::gPlayer->handleInput(&Game::inputEvent);
				break;
			}
		}
		Game::gPlayer->handleMovements();
		Game::process();
		// Level::moveLevel();
		Graphics::renderAll();
	}
	Level::closeLevel();
	Game::close();
	return 0;
}