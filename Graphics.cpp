#include "Graphics.h"
#include "Texture.h"
#include "Window.h"
#include "Player.h"
#include "Game.h"
#include "Level.h"

SDL_Rect Graphics::viewport = { 0, 0, 0, 0 };
std::vector<Texture*> Graphics::blockGFX;
std::vector<std::pair<Texture*, int>> Graphics::bgObjects;
Texture* Graphics::bg;
Texture* Graphics::playerGFX;
SDL_Color Graphics::renderColor = { 0, 255, 255, 255 };
std::string Graphics::rDir = "Resources/";
std::string Graphics::rExt = ".png";
std::string Graphics::blockPrefix = "blk";
std::string Graphics::playerPrefix = "player";
std::string Graphics::bgPrefix = "bg";
const int Graphics::GFX_OFFSET = 0;
const int Graphics::BG_ALPHA_BASE = 255;

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
	// SDL_RenderSetClipRect(Window::renderer, &viewport);
	// SDL_RenderSetViewport(Window::renderer, &viewport);

	manageBG();
	bg->txRender();
	for (int i = 0; i < bgObjects.size(); i++)
		bgObjects[i].first->txRender();

	// only render if entity's rect is colliding with viewport rect
	for (int i = 0; i < Game::renderedEntities.size(); i++)
	{
		if (Game::checkCollision(viewport, Game::renderedEntities[i]->rect))
		{
			// Game::renderedEntities[i]->syncGFX();
			// blockGFX[Game::renderedEntities[i]->getSubtype()]->rect = Game::renderedEntities[i]->rect;
			blockGFX[Game::renderedEntities[i]->getSubtype()]->rect = { Game::renderedEntities[i]->rect.x - viewport.x, Game::renderedEntities[i]->rect.y - viewport.y, Game::renderedEntities[i]->rect.w, Game::renderedEntities[i]->rect.h };
			blockGFX[Game::renderedEntities[i]->getSubtype()]->txRender();
		}
	}
	// gPlayer.syncGFX();
	// Game::gPlayer->syncGFX();
	// playerGFX->rect = Game::gPlayer->rect;
	playerGFX->rect = { Game::gPlayer->rect.x - viewport.x, Game::gPlayer->rect.y - viewport.y, Game::gPlayer->rect.w, Game::gPlayer->rect.h };
	playerGFX->txRender(NULL, NULL, plrot, SDL_FLIP_NONE);

	SDL_RenderPresent(Window::renderer);
}

void Graphics::manageCamera()
{
	
	if ((Game::gPlayer->rect.x + Game::gPlayer->rect.w / 2 > viewport.w / 2) ||
		(Game::gPlayer->rect.x + Game::gPlayer->rect.w / 2 < viewport.w / 2))
		viewport.x = Game::gPlayer->rect.x + Game::gPlayer->rect.w / 2 - viewport.w / 2;
	if ((Game::gPlayer->rect.y + Game::gPlayer->rect.h / 2 > viewport.h / 2) ||
		(Game::gPlayer->rect.y + Game::gPlayer->rect.h / 2 < viewport.h / 2))
		viewport.y = Game::gPlayer->rect.y + Game::gPlayer->rect.h / 2 - viewport.h / 2;

	if (viewport.x + viewport.w >= Level::getw('p'))
		viewport.x = Level::getw('p') - viewport.w;
	if (viewport.y + viewport.h >= Level::geth('p'))
		viewport.y = Level::geth('p') - viewport.h;
	if (viewport.x < 0)
		viewport.x = 0;
	if (viewport.y < 0)
		viewport.y = 0;
}

void Graphics::loadBG(int which)
{
	closeBG();

	switch (which)
	{
	case MOUNTAINS:
		bg = new Texture(0, 0, 0, 0);
		bg->txLoadF(rDir + bgPrefix + std::to_string(which) + rExt);
		manageBG();
		break;
	}
}

void Graphics::closeBG()
{
	if (bg != NULL)
	{
		delete bg;
		bg = NULL;
	}
	for (int i = bgObjects.size() - 1; i >= 0; i--)
	{
		if (bgObjects[i].first != NULL)
		{
			delete bgObjects[i].first;
			bgObjects[i].first = NULL;
		}
		bgObjects.pop_back();
	}
}

void Graphics::manageBG()
{
	SDL_Rect window{ 0, 0, Window::getw(), Window::geth() };
	static SDL_Rect vp = viewport;
	for (int i = 0; i < bgObjects.size(); i++)
	{
		if (bgObjects.size() > 2 && !Game::checkCollision(window, bgObjects[i].first->rect))
		{
			delete bgObjects[i].first;
			bgObjects[i].first = NULL;
			bgObjects.erase(bgObjects.begin() + i);
			i--;
		}
		else
		{
			if(viewport.x != vp.x)
				bgObjects[i].first->rect.x -= bgObjects[i].second * (vp.x - viewport.x < 0 ? 1 : -1);
		}
	}
	if (bgObjects.size() > 0)
	{
		if (bgObjects[0].first->rect.x > window.x && vp.x - viewport.x > 0)
		{
			bgObjects.insert(bgObjects.begin(), std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2) + (Game::MOVE_SPEED * 0.5)));
			bgObjects[0].first->txLoadF(rDir + bgPrefix + "Object" + std::to_string(Level::getBGID()) + rExt);
			bgObjects[0].first->txSetAlpha(BG_ALPHA_BASE / 2);
			bgObjects[0].first->rect.x = window.x - bgObjects[0].first->rect.w + 1;
			// bgObjects[0].first->rect.x = bgObjects[1].first->rect.x - bgObjects[0].first->rect.w + bgObjects[0].second;
		}
	}
	for (int i = bgObjects.size();; i++)
	{
		if (i > 0)
			if (bgObjects[i - 1].first->rect.x + bgObjects[i - 1].first->rect.w > window.x + window.w)
				break;
		bgObjects.push_back(std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2 + 1) + (Game::MOVE_SPEED * 0.5)));
		bgObjects[i].first->txLoadF(rDir + bgPrefix + "Object" + std::to_string(Level::getBGID()) + rExt);
		bgObjects[i].first->txSetAlpha(BG_ALPHA_BASE / (i*2 + 2));
		if (i > 0)
		{
			bgObjects[i].first->rect.x = bgObjects[i - 1].first->rect.x + bgObjects[i - 1].first->rect.w - 1;
			if (bgObjects[i].second == bgObjects[i - 1].second)
				bgObjects[i].second += rand() % 2 + -1;
			if (bgObjects[i].second == 0)
				bgObjects[i].second++;
		}
	}
	vp = viewport;
}