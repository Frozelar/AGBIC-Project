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
	int curlevel = 0;
	int oldMode = 0;
	Uint32 curfps = 0, curtime = 0, gamestart = 0;

	Game::Mode = TITLE;

	while (!quit)
	{
		curfps = SDL_GetTicks();
		switch (Game::Mode)
		{
		case TITLE:
			Level::closeLevel();
			Game::gPlayer->resetAbilities();
			curtime = 0;
			Game::score = 0;
			quit = Menu::loop(TITLE, { 0, 0, Window::getw(), Window::geth() }, &Game::inputEvent);
			if (quit)
			{
				Game::close();
				return 0;
			}
			break;
		case PAUSE:
			quit = Menu::loop(PAUSE, Graphics::MENU_RECT, &Game::inputEvent);
			break;
		}

		if (Game::Mode != TITLE && Level::getID() == -1)
			Level::generateLevel(curlevel/*++*/);

		switch (Game::Mode)
		{
		case LEVEL_BEGIN:
			Graphics::handleGameOverlay(0, 0, true);
			quit = Level::begin();
			// levelstart = SDL_GetTicks();
			break;
		case LEVEL_END:
			quit = Level::end();
			break;
		}
		if (Game::Mode == GAME && oldMode == TITLE)
			gamestart = SDL_GetTicks();
		oldMode = Game::Mode;

		if(Game::Mode != BOSS)
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
		if(Game::gPlayer->destroy)
			quit = Game::gPlayer->destroy;
		// if (1000 / Game::FPS > SDL_GetTicks() - curfps)
		// 	SDL_Delay((1000 / Game::FPS) - (SDL_GetTicks() - curfps));
		curtime = (curfps - gamestart) / 1000;
		if (Game::Mode == GAME)
			Graphics::handleGameOverlay(curtime, Game::score);
		curlevel = Level::getID();
	}
	Level::closeLevel();
	Game::close();
	return 0;
}