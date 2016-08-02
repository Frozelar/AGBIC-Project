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
#include "Menu.h"

// create static classes
Window gWindow;
Game gGame;
Graphics gGraphics;
Level gLevel;
Audio gAudio;
Menu gMenu;

int main(int argc, char** argv)
{
	bool quit = false;
	// int curlevel = 0;
	// int oldMode = 0;
	// Uint32 curfps = 0, curtime = 0, gamestart = 0;

	Game::Mode = TITLE;

	while (!quit)
	{
		if (Game::Mode != BOSS)
			Graphics::manageCamera();
		quit = Game::manageMode();
		if (quit)
			break;
		while (SDL_PollEvent(&Game::inputEvent) != NULL)
		{
			switch (Game::inputEvent.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			//case SDL_WINDOWEVENT:
			//	Window::handleEvent(&Game::inputEvent);
			//	break;
			default:
				Game::gPlayer->handleInput(&Game::inputEvent);
				break;
			}
		}
		if(Game::gPlayer->destroy)
			quit = Game::gPlayer->destroy;
		Game::gPlayer->handleMovements();
		Game::process();
		// Level::moveLevel();
		Graphics::renderAll();
		// if (1000 / Game::FPS > SDL_GetTicks() - curfps)
		// 	SDL_Delay((1000 / Game::FPS) - (SDL_GetTicks() - curfps));
		if (Game::Mode == GAME || Game::Mode == GAME_END)
			Graphics::handleGameOverlay(Game::gTime, Game::gScore);
	}
	Level::closeLevel();
	Game::close();
	return 0;
}