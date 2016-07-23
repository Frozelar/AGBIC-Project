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
const int Graphics::BG_ALPHA_BASE = 25;

// default menu size
const SDL_Rect Graphics::MENU_RECT = { Window::getw() / 4, Window::geth() / 4, Window::getw() / 2, Window::geth() / 2 };

// value used for determining density of particles
const float Graphics::DEFAULT_PARTICLE_DENSITY = 0;
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

// displayed after the final boss is defeated
std::pair<Texture*, std::string> Graphics::winText;
std::vector<Texture*> Graphics::credits;

std::vector<std::string> Graphics::creditsText = { 
	"Ice Age", "\n", 
	"Copyright (C) 2016 Frozelar", "\n", 
	"Made for the 2016 A Game By Its Cover game jam", "\n\n\n",
	"Programming, Graphics, and Sound Effects done by me (Frozelar)", "\n\n\n"
	"Music taken from OverClocked Remix (ocremix.org):", "\n",
	"\"The Glacier's Peak\", a Sonic Advance remix by jmr", "\n",
	"\"Crystalline Caverns\", a Super Mario World 2: Yoshi's Island remix by McVaffe", "\n\n\n", 
	"A full list of credits, relevant links, licensing information, and further elaborations can all be found in the README.", "\n\n\n",
	"Special Thanks:", "\n\n",
	"Kiwisauce", "\n",
	/* "for inspiring me to pursue game development, and for providing me with plenty of guidance along the way.", */
	"Unormal", "\n\n\n",
	/* "for teaching me superior programming practices and giving me a lot of advice.", */
	"Thanks so much for playing!\n\n\n" };

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

	winText.second = "YOU WIN";
	winText.first = new Texture(0, 0, 0, 0);
	winText.first->txLoadT(winText.second, gFont, black);
	winText.first->txRect.x = Window::getw();
	winText.first->txRect.y = Window::geth() / 2 - winText.first->txRect.h / 2;
	credits.resize(creditsText.size());
	for (int i = 0; i < credits.size(); i++)
	{
		credits[i] = new Texture(0, 0, 0, 0);
		credits[i]->txLoadT(creditsText[i], gFont, black);
		// credits[i]->txRect.x = Window::getw() / 2 - (credits[i]->txRect.x + credits[i]->txRect.w / 2);
		credits[i]->txRect.x = Game::UNIT_W;
		credits[i]->txRect.y = Window::geth();
	}
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
	if (winText.first != NULL)
	{
		delete winText.first;
		winText.first = NULL;
	}
	for (int i = 0; i < credits.size(); i++)
	{
		if (credits[i] != NULL)
		{
			delete credits[i];
			credits[i] = NULL;
		}
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
	// SDL_Rect part = { 0, 0, 0, 0 };
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

	bg->txRender();
	manageBG();
	// for (int i = 0; i < bgObjects.size(); i++)
	// 	if(Game::checkCollision(viewport, bgObjects[i].first->txRect))
	// 		bgObjects[i].first->txRender();

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

	manageParticles(SNOW /* Level::getID() */);

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

	if (Game::Mode == GAME_END)
		playGameEnd();

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

// play credits/end sequence
void Graphics::playGameEnd(bool reset)
{
	static bool showWin = true;
	static bool showCredits = false;
	static bool showInfo = false;
	static int delay = 180;
	static int winRot = 15;
	static int tscore = -1;
	static int ttime = -1;
	static int i = 0;
	static Texture timetx;
	static Texture scoretx;

	if (reset)
	{
		showWin = true;
		showCredits = false;
		showInfo = false;
		delay = 180;
		winRot = 15;
		tscore = -1;
		ttime = -1;
		i = 0;
		for (int i = 0; i < credits.size(); i++)
		{
			credits[i]->txRect.x = Game::UNIT_W;
			credits[i]->txRect.y = Window::geth();
		}
		winText.first->txRect.x = Window::getw();
		winText.first->txRect.y = Window::geth() / 2 - winText.first->txRect.h / 2;
		return;
	}
	if (showCredits)
	{
		for (int j = 0; j <= i; j++)
		{
			if (credits[j]->txRect.y + credits[j]->txRect.h >= 0)
			{
				credits[j]->txRect.y--;
				credits[j]->txRender();
			}
		}
		if (i < credits.size() - 1)
		{
			if (credits[i]->txRect.y + credits[i]->txRect.h + Game::UNIT_H < credits[i + 1]->txRect.y)
				i++;
		}
		else
		{
			if (credits[i]->txRect.y + credits[i]->txRect.h <= 0)
				showInfo = true;
		}
	}
	if (showWin)
	{
		if (tscore == -1 || ttime == -1)
		{
			tscore = Game::gScore;
			ttime = Game::gTime;
		}
		if (winText.first->txRect.x > Game::UNIT_W * 2)
		{
			winText.first->txRect.x -= winRot * 0.009;
			winRot += 2;
		}
		else
		{
			if (delay <= 0)
			{
				showWin = false;
				showCredits = true;
			}
			if (showWin && !showInfo)
				delay--;
		}
		winText.first->txRender(NULL, NULL, winRot, SDL_FLIP_NONE);
	}
	if (showInfo)
	{
		if (!showWin)
		{
			showWin = true;
			winRot = 0;
			winText.first->txRect.x = Game::UNIT_W;
			winText.first->txRect.y = Game::UNIT_H;
			timetx.txRect = { winText.first->txRect.x + winText.first->txRect.w + Game::UNIT_W * 3, winText.first->txRect.y, 0, 0 };
			timetx.txLoadT("Final Time: " + std::to_string(ttime), gFont, black);
			scoretx.txRect = { timetx.txRect.x + timetx.txRect.w + Game::UNIT_W * 3, winText.first->txRect.y, 0, 0 };
			scoretx.txLoadT("Final Score: " + std::to_string(tscore), gFont, black);
		}
		timetx.txRender();
		scoretx.txRender();
	}
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
		// manageBG();
		createBGObjects();
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
	static SDL_Rect vp = viewport;
	for (int i = 0; i < bgObjects.size(); i++)
	{
		if (viewport.x != vp.x)
			bgObjects[i].first->txRect.x -= bgObjects[i].second * (vp.x - viewport.x < 0 ? 1 : -1);
		// if (Game::checkCollision(viewport, bgObjects[i].first->txRect))
			bgObjects[i].first->txRender();
	}
	vp = viewport;

	/*
	SDL_Rect window{ 0, 0, Window::getw(), Window::geth() };
	static SDL_Rect vp = viewport;
	for (int i = 0; i < bgObjects.size(); i++)
	{
		if (/* bgObjects.size() > 2 &&  !Game::checkCollision(window, bgObjects[i].first->txRect))
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
			bgObjects.insert(bgObjects.begin(), std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2) + (Game::MOVE_SPEED/* * 0.5 )));
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
		bgObjects.push_back(std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2 + 1) + (Game::MOVE_SPEED/* * 0.5 )));
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
	*/
}

// place all background objects
void Graphics::createBGObjects()
{
	int maxGap = 64;
	for (int x = 0 - viewport.x, y = 0; x < Level::getw('p');)
	{
		bgObjects.push_back(std::pair<Texture*, int>(new Texture(x, y, 0, 0), rand() % (Game::MOVE_SPEED * 2) + (Game::MOVE_SPEED/* * 0.5*/)));
		bgObjects.back().first->txLoadF(rDir + bgObjectPrefix + bgObjectIDs[Level::getBGID()] + rExt);
		bgObjects.back().first->txSetAlpha((BG_ALPHA_BASE * bgObjects.back().second > 255 * 0.75 ? 255 * 0.75 : BG_ALPHA_BASE * bgObjects.back().second));
		bgObjects.back().first->txRect.w *= ceil((1 / (rand() % 3 + 1)));
		bgObjects.back().first->txRect.y += rand() % (Window::geth() / 4);
		x = bgObjects.back().first->txRect.x + bgObjects.back().first->txRect.w + (rand() % maxGap);
	}

}

// manage particle engine each frame; do not call until you want particles to begin spawning
void Graphics::manageParticles(int which)
{
	SDL_Rect part = { 0, 0, 0, 0 };
	// static int counter = 0;
	switch (which)
	{
	case SNOW:
		float loops = particleDensity;
		if (loops < 1 && loops > 0)
			loops = (rand() % abs(static_cast<int>(loops * 100 - 100)) == 1 ? 1 : 0);
		while (loops-- > 0)
		{
			spawnParticle(which);
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

				if (Game::checkCollision(viewport, particles[i].first->txRect))
				{
					part = particles[i].first->txRect;
					particles[i].first->txRect = { particles[i].first->txRect.x - viewport.x, particles[i].first->txRect.y - viewport.y, particles[i].first->txRect.w, particles[i].first->txRect.h };
					particles[i].first->txRender();
					particles[i].first->txRect = part;
				}
			}
		}
		break;
	}
}

void Graphics::spawnParticle(int which, SDL_Rect* prect)
{
	particles.push_back(std::pair<Texture*, int>(new Texture(0, 0, 0, 0), rand() % (Game::MOVE_SPEED * 2) + 1));
	particles[particles.size() - 1].first->txLoadF(rDir + particlePrefix + Game::particleIDs[which] + rExt);
	if (prect == NULL)
	{
		// particles[particles.size() - 1].first->rect.x = rand() % (viewport.x + viewport.w) + viewport.x;
		particles[particles.size() - 1].first->txRect.x = rand() % (Game::gPlayer->rect.x + viewport.w) + (Game::gPlayer->rect.x - viewport.w);
		particles[particles.size() - 1].first->txRect.w /= (rand() % 9 + 1);
		if (particles[particles.size() - 1].first->txRect.w <= 0)
			particles[particles.size() - 1].first->txRect.w = 1;
		particles[particles.size() - 1].first->txRect.h = particles[particles.size() - 1].first->txRect.w;
		particles[particles.size() - 1].first->txRect.y -= particles[particles.size() - 1].first->txRect.h;
	}
	else
		particles[particles.size() - 1].first->txRect = *prect;
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
		if (Game::Mode == GAME_END)
		{
			messages[timeIndex]->text->txRect.y = Window::geth();
			messages[scoreIndex]->text->txRect.y = Window::geth();
		}
	}
	oldTime = time;
	oldScore = score;
}

SDL_Rect Graphics::getViewport()
{
	return viewport;
}