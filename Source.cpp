#include "Include.h"
#include "Window.h"
#include "Game.h"
#include "Graphics.h"
#include "Player.h"

Window gWindow;
Game gGame;
Graphics gGraphics;

int main(int argc, char** argv)
{
	bool quit = false;
	Game::newEntity({ 100, 600, 32, 32 }, BLOCK, WHITE);
	Game::newEntity({ 196, 632, 32, 32 }, BLOCK, WHITE);
	while (!quit)
	{
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
		Graphics::renderAll();
	}
	return 0;
}