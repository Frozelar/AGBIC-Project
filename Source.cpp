#include "Include.h"
#include "Window.h"
#include "Game.h"
#include "Graphics.h"
#include "Player.h"
#include "Level.h"
#include "Audio.h"

Window gWindow;
Game gGame;
Graphics gGraphics;
Level gLevel;
Audio gAudio;

int main(int argc, char** argv)
{
	bool quit = false;
	int curlevel = 0;
	// Game::newEntity({ 100, 600, 32, 32 }, BLOCK, WHITE);
	// Game::newEntity({ 196, 632, 32, 32 }, BLOCK, WHITE);
	while (!quit)
	{
		if (Level::getID() == -1)
			Level::generateLevel(curlevel++);
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
		// Level::moveLevel();
		Graphics::renderAll();
	}
	Level::closeLevel();
	Game::close();
	return 0;
}