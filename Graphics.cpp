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

#include "Graphics.h"
#include "Texture.h"
#include "Window.h"
#include "Player.h"
#include "Game.h"
#include "Level.h"
#include "Message.h"

// viewport rect
SDL_Rect Graphics::viewport = { 0, 0, 0, 0 };

// textures for specified objects
std::vector<Texture*> Graphics::blockGFX;
std::vector<Texture*> Graphics::collectibleGFX;
std::vector<Texture*> Graphics::enemyGFX;

// store background objects and particles
std::vector<std::pair<Texture*, int>> Graphics::bgObjects;
std::vector<std::pair<Texture*, int>> Graphics::particles;

// identifiers for backgrounds and background objects (used in graphic file loading)
std::vector<std::string> Graphics::bgIDs = { "Sky" };
std::vector<std::string> Graphics::bgObjectIDs = { "Mountain" };

// background texture
Texture* Graphics::bg;

// player texture
Texture* Graphics::playerGFX;

// render color for window
SDL_Color Graphics::renderColor = { 0, 255, 255, 255 };

// directory and file extension for graphical resources
std::string Graphics::rDir = "Resources/";
std::string Graphics::rExt = ".png";

// prefixes for given entity file names
std::string Graphics::blockPrefix = "blk";
std::string Graphics::collectiblePrefix = "col";
std::string Graphics::playerPrefix = "player";
std::string Graphics::enemyPrefix = "nme";
std::string Graphics::bgPrefix = "bg";
std::string Graphics::bgObjectPrefix = "bgo";
std::string Graphics::particlePrefix = "prt";
std::string Graphics::textBGPrefix = "textBG";

// amount that graphics are offset from hitboxes
const int Graphics::GFX_OFFSET = 0;

// value used for determining alpha of background objects
const int Graphics::BG_ALPHA_BASE = 255;

// default menu size
const SDL_Rect Graphics::MENU_RECT = { Window::getw() / 4, Window::geth() / 4, Window::getw() / 2, Window::geth() / 2 };

// value used for determining density of particles
float Graphics::particleDensity = 0; //2;

// text-related members
Texture* Graphics::textBG = NULL;
std::string Graphics::fontName = "AveriaSans-Regular.ttf";
TTF_Font* Graphics::gFont = NULL;
// SDL_Color Graphics::gTextColor = { 255, 255, 255, 255 };
SDL_Color Graphics::white = { 255, 255, 255, 255 };
SDL_Color Graphics::black = { 0, 0, 0, 255 };
int Graphics::gFontSize = 36;

// container of current messages
std::vector<Message*> Graphics::messages;

// messages that are displayed when a collectible is collected for the first time
// std::vector<std::string> Graphics::collectiblePickupMsgs;
// std::vector<Texture*> Graphics::collectiblePickupTextures;

// call init()
Graphics::Graphics()
{
	init();
}

// call close()
Graphics::~Graphics()
{
	close();
}

// initialize all graphics
bool Graphics::init()
{
	std::string fnt = rDir + fontName;
	TTF_Init();
	gFont = TTF_OpenFont(fnt.c_str(), gFontSize);

	playerGFX = new Texture(0, 0, 0, 0);
	playerGFX->txLoadF(rDir + playerPrefix + rExt);
	for (int i = 0; i < Game::blockIDs.size(); i++)
	{
		blockGFX.push_back(new Texture(0, 0, 0, 0));
		blockGFX[i]->txLoadF(rDir + blockPrefix + Game::blockIDs[i] + rExt);
	}
	for (int i = 0; i < Game::collectibleIDs.size(); i++)
	{
		collectibleGFX.push_back(new Texture(0, 0, 0, 0));
		collectibleGFX[i]->txLoadF(rDir + collectiblePrefix + Game::collectibleIDs[i] + rExt);
	}
	for (int i = 0; i < Game::enemyIDs.size(); i++)
	{
		enemyGFX.push_back(new Texture(0, 0, 0, 0));
		enemyGFX[i]->txLoadF(rDir + enemyPrefix + Game::enemyIDs[i] + rExt);
	}
	textBG = new Texture(0, 0, 0, 0);
	textBG->txLoadF(rDir + textBGPrefix + rExt);
	return true;
}

// delete all graphics
void Graphics::close()
{
	closeLevelGFX();
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
	for (int i = 0; i < collectibleGFX.size(); i++)
	{
		if (collectibleGFX[i] != NULL)
		{
			delete collectibleGFX[i];
			collectibleGFX[i] = NULL;
		}
	}
	for (int i = 0; i < enemyGFX.size(); i++)
	{
		if (enemyGFX[i] != NULL)
		{
			delete enemyGFX[i];
			enemyGFX[i] = NULL;
		}
	}
	if (textBG != NULL)
	{
		delete textBG;
		textBG = NULL;
	}
}

// delete graphics only related to the level in specific
void Graphics::closeLevelGFX()
{
	clearMessages();
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
	for (int i = particles.size() - 1; i >= 0; i--)
	{
		if (particles[i].first != NULL)
		{
			delete particles[i].first;
			particles[i].first = NULL;
		}
		particles.pop_back();
	}
}

void Graphics::clearMessages()
{
	for (int i = 0; i < messages.size(); i++)
		messages.pop_back();
}

// render everything
void Graphics::renderAll(bool manageRenderer)
{
	SDL_Rect part = { 0, 0, 0, 0 };
	static int plrot = 0;
	static int clrot = 0;
	static int enrot = 0;
	plrot = abs(Game::gPlayer->aerialSpeed != 0 ? plrot + Game::gPlayer->aerialSpeed * Game::ROTATION_SPEED * (Game::gPlayer->aerialSpeed < 0 ? -1 : 1) : 0);
	clrot += Game::ROTATION_SPEED;
	enrot += Game::ROTATION_SPEED * 2;
	viewport.w = Window::getw();
	viewport.h = Window::geth();

	if (manageRenderer)
	{
		SDL_RenderClear(Window::renderer);
		SDL_SetRenderDrawColor(Window::renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
		// SDL_RenderSetClipRect(Window::renderer, &viewport);
		// SDL_RenderSetViewport(Window::renderer, &viewport);
	}

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
			switch (Game::renderedEntities[i]->getType())
			{
			case BLOCK:
				blockGFX[Game::renderedEntities[i]->getSubtype()]->txRect = { Game::renderedEntities[i]->rect.x - viewport.x, Game::renderedEntities[i]->rect.y - viewport.y, Game::renderedEntities[i]->rect.w, Game::renderedEntities[i]->rect.h };
				blockGFX[Game::renderedEntities[i]->getSubtype()]->txRender();
				break;
			case COLLECTIBLE:
				collectibleGFX[Game::renderedEntities[i]->getSubtype()]->txRect = { Game::renderedEntities[i]->rect.x - viewport.x, Game::renderedEntities[i]->rect.y - viewport.y, Game::renderedEntities[i]->rect.w, Game::renderedEntities[i]->rect.h };
				collectibleGFX[Game::renderedEntities[i]->getSubtype()]->txRender(NULL, NULL, clrot, SDL_FLIP_NONE);
				break;
			case ENEMY:
				enemyGFX[Game::renderedEntities[i]->getSubtype()]->txRect = { Game::renderedEntities[i]->rect.x - viewport.x, Game::renderedEntities[i]->rect.y - viewport.y, Game::renderedEntities[i]->rect.w, Game::renderedEntities[i]->rect.h };
				enemyGFX[Game::renderedEntities[i]->getSubtype()]->txRender(NULL, NULL, enrot, SDL_FLIP_NONE);
				break;
			}
		}
	}

	// for (int i = 0; i < particles.size(); i++)
	// 	particles[i].first->txRender();
	
	// gPlayer.syncGFX();
	// Game::gPlayer->syncGFX();
	// playerGFX->rect = Game::gPlayer->rect;
	playerGFX->txRect = { Game::gPlayer->rect.x - viewport.x, Game::gPlayer->rect.y - viewport.y, Game::gPlayer->rect.w, Game::gPlayer->rect.h };
	playerGFX->txRender(NULL, NULL, plrot, SDL_FLIP_NONE);

	manageParticles(Level::getID());
	for (int i = 0; i < particles.size(); i++)
	{
		if (Game::checkCollision(viewport, particles[i].first->txRect))
		{
			part = particles[i].first->txRect;
			particles[i].first->txRect = { particles[i].first->txRect.x - viewport.x, particles[i].first->txRect.y - viewport.y, particles[i].first->txRect.w, particles[i].first->txRect.h };
			particles[i].first->txRender();
			particles[i].first->txRect = part;
		}
	}

	for (int i = 0; i < messages.size(); i++)
	{
		if (messages[i]->displayTime <= 0 && messages[i]->displayTime != -1)
		{
			messages.erase(messages.begin() + i);
			i--;
		}
		else
			messages[i]->render();
	}

	if(manageRenderer)
		SDL_RenderPresent(Window::renderer);
}

// center camera over player if not at level edges
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

// load given background
void Graphics::loadBG(int which)
{
	closeBG();

	switch (which)
	{
	case MOUNTAINS:
		bg = new Texture(0, 0, 0, 0);
		bg->txLoadF(rDir + bgPrefix + bgIDs[which] + rExt);
		manageBG();
		break;
	}
}

// close current background
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

// handle background and background objects (called each frame)
void Graphics::manageBG()
{
	SDL_Rect window{ 0, 0, Window::getw(), Window::geth() };
	static SDL_Rect vp = viewport;
	for (int i = 0; i < bgObjects.size(); i++)
	{
		if (/* bgObjects.size() > 2 && */ !Game::checkCollision(window, bgObjects[i].first->txRect))
		{
			delete bgObjects[i].first;
			bgObjects[i].first = NULL;
			bgObjects.erase(bgObjects.begin() + i);
			i--;
		}
		else
		{
			if(viewport.x != vp.x)
				bgObjects[i].first->txRect.x -= bgObjects[i].second * (vp.x - viewport.x < 0 ? 1 : -1);
		}
	}
	if (bgObjects.size() > 0)
	{
		if (bgObjects[0].first->txRect.x > window.x && vp.x - viewport.x > 0)
		{
			bgObjects.insert(bgObjects.begin(), std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2) + (Game::MOVE_SPEED/* * 0.5*/)));
			bgObjects[0].first->txLoadF(rDir + bgObjectPrefix + bgObjectIDs[Level::getBGID()] + rExt);
			bgObjects[0].first->txSetAlpha(BG_ALPHA_BASE / 2);
			bgObjects[0].first->txRect.w *= ceil((1 / (rand() % 3 + 1)));
			// bgObjects[0].first->rect.h *= ceil(((rand() % 3 + 1) / (rand() % 3 + 1)));
			bgObjects[0].first->txRect.y += rand() % (Window::geth() / 4);
			bgObjects[0].first->txRect.x = window.x - bgObjects[0].first->txRect.w + 1;
			// bgObjects[0].first->rect.x = bgObjects[1].first->rect.x - bgObjects[0].first->rect.w + bgObjects[0].second;
		}
	}
	for (int i = bgObjects.size();; i++)
	{
		if (i > 0)
			if (bgObjects[i - 1].first->txRect.x + bgObjects[i - 1].first->txRect.w > window.x + window.w)
				break;
		bgObjects.push_back(std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2 + 1) + (Game::MOVE_SPEED/* * 0.5*/)));
		bgObjects[i].first->txLoadF(rDir + bgObjectPrefix + bgObjectIDs[Level::getBGID()] + rExt);
		bgObjects[i].first->txSetAlpha(BG_ALPHA_BASE / (rand() % (BG_ALPHA_BASE / 51) + 2));
		if (i > 0)
		{
			bgObjects[i].first->txRect.w *= ceil((1 / (rand() % 3 + 1)));
			// bgObjects[i].first->rect.h *= ceil(((rand() % 3 + 1) / (rand() % 3 + 1)));
			bgObjects[i].first->txRect.y += rand() % (Window::geth() / 4);
			// bgObjects[i].first->rect.x = bgObjects[i - 1].first->rect.x + bgObjects[i - 1].first->rect.w - 1;
			bgObjects[i].first->txRect.x = Window::getw() - 1;

			if (bgObjects[i].second == bgObjects[i - 1].second)
				bgObjects[i].second += rand() % 2 + -1;
			if (bgObjects[i].second == 0)
				bgObjects[i].second++;
		}
	}
	vp = viewport;
}

// manage particle engine each frame; do not call until you want particles to begin spawning
void Graphics::manageParticles(int which)
{
	// static int counter = 0;
	switch (which)
	{
	case SNOW:
		float loops = particleDensity;
		if (loops < 1 && loops > 0)
			loops = (rand() % abs(static_cast<int>(loops * 100 - 100)) == 1 ? 1 : 0);
		while (loops-- > 0)
		{
			particles.push_back(std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2) + 1));
			particles[particles.size() - 1].first->txLoadF(rDir + particlePrefix + Game::particleIDs[which] + rExt);
			// particles[particles.size() - 1].first->rect.x = rand() % (viewport.x + viewport.w) + viewport.x;
			particles[particles.size() - 1].first->txRect.x = rand() % (Game::gPlayer->rect.x + viewport.w) + (Game::gPlayer->rect.x - viewport.w);
			particles[particles.size() - 1].first->txRect.w /= (rand() % 9 + 1);
			if (particles[particles.size() - 1].first->txRect.w <= 0)
				particles[particles.size() - 1].first->txRect.w = 1;
			particles[particles.size() - 1].first->txRect.h = particles[particles.size() - 1].first->txRect.w;
			particles[particles.size() - 1].first->txRect.y -= particles[particles.size() - 1].first->txRect.h;
		}
		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i].first->txRect.y > Level::geth('p'))
			{
				delete particles[i].first;
				particles[i].first = NULL;
				particles.erase(particles.begin() + i);
				i--;
			}
			else
			{
				if(rand() % 8 == 1)
					particles[i].first->txRect.x += (particles[i].second / 2 * (rand() % 2 == 0 ? -1 : 1));
				particles[i].first->txRect.y += particles[i].second;
			}
		}
		break;
	}
}

void Graphics::newMessage(std::string pmsg, int psize, int ptime, SDL_Color pcolor, Direction pdir, Direction pside, bool pshow)
{
	messages.push_back(new Message(pmsg, psize, ptime, pcolor, pdir, pside, pshow));
}

void Graphics::handleGameOverlay(int time, int score, bool reset)
{
	static int timeIndex = -1, scoreIndex = -1;
	static int oldTime = 0, oldScore = 0;
	if (reset)
	{
		clearMessages();
		timeIndex = scoreIndex = -1;
		oldTime = oldScore = 0;
		return;
	}
	if (timeIndex == -1 || scoreIndex == -1)
	{
		timeIndex = messages.size();
		newMessage(std::to_string(time), 36, -1, { 0, 0, 0, 255 }, UP, RIGHT, true);
		scoreIndex = messages.size();
		newMessage(std::to_string(score), 36, -1, { 0, 0, 0, 255 }, DOWN, RIGHT, true);
	}
	else
	{
		if(oldTime != time)
			messages[timeIndex]->setText(std::to_string(time));
		if(oldScore != score)
			messages[scoreIndex]->setText(std::to_string(score));
	}
	oldTime = time;
	oldScore = score;
}

SDL_Rect Graphics::getViewport()
{
	return viewport;
}