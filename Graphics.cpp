#include "Graphics.h"
#include "Texture.h"
#include "Window.h"
#include "Player.h"
#include "Game.h"
#include "Entities.h"

SDL_Rect Graphics::viewport = { 0, 0, 0, 0 };
std::vector<Texture*> Graphics::blockGFX;
Texture* Graphics::playerGFX;
SDL_Color Graphics::renderColor = { 0, 255, 255, 255 };
std::string Graphics::rDir = "Resources/";
std::string Graphics::rExt = ".png";
std::string Graphics::blockPrefix = "blk";
std::string Graphics::playerPrefix = "player";
const int Graphics::GFX_OFFSET = 0;

Graphics::Graphics()
{
	init();
}

Graphics::~Graphics()
{
	close();
}

bool Graphics::init()
{
	playerGFX = new Texture(0, 0, 0, 0);
	playerGFX->txLoadF(rDir + playerPrefix + rExt);
	for (int i = 0; i < Game::blockTypes.size(); i++)
	{
		blockGFX.push_back(new Texture(0, 0, 0, 0));
		blockGFX[i]->txLoadF(rDir + blockPrefix + Game::blockTypes[i] + rExt);
	}
	return true;
}

void Graphics::close()
{
	if (playerGFX != NULL)
	{
		delete playerGFX;
		playerGFX = NULL;
	}
	for (int i = 0; i < blockGFX.size(); i++)
	{
		if (blockGFX[i] != NULL)
		{
			delete blockGFX[i];
			blockGFX[i] = NULL;
		}
	}
}

void Graphics::renderAll()
{
	static int plrot = 0;
	plrot = abs(Game::gPlayer->aerialSpeed != 0 ? plrot + Game::gPlayer->aerialSpeed * Game::ROTATION_SPEED * (Game::gPlayer->aerialSpeed < 0 ? -1 : 1) : 0);
	viewport.w = Window::getw();
	viewport.h = Window::geth();
	SDL_RenderClear(Window::renderer);
	SDL_SetRenderDrawColor(Window::renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
	SDL_RenderSetViewport(Window::renderer, &viewport);

	// only render if entity's rect is colliding with viewport rect
	for (int i = 0; i < Game::staticEntities.size(); i++)
	{
		if (Game::checkCollision(viewport, Game::staticEntities[i]->rect))
		{
			Game::staticEntities[i]->syncGFX();
			blockGFX[Game::staticEntities[i]->getSubtype()]->rect = Game::staticEntities[i]->gfxRect;
			blockGFX[Game::staticEntities[i]->getSubtype()]->txRender();
		}
	}
	// gPlayer.syncGFX();
	Game::gPlayer->syncGFX();
	playerGFX->rect = Game::gPlayer->gfxRect;
	playerGFX->txRender(NULL, NULL, plrot, SDL_FLIP_NONE);

	SDL_RenderPresent(Window::renderer);
}