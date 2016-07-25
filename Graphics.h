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

#pragma once
#include "Include.h"

enum Background {
	MOUNTAINS
};

enum Particles {
	SNOW
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	static bool init(void);
	static void close(void);
	static void closeLevelGFX(void);
	static void renderAll(bool = true);
	static void manageCamera(void);
	static void loadBG(int);
	static void closeBG(void);
	static void manageBG(void);
	static void createBGObjects(void);
	static void manageParticles(int);
	static void spawnParticle(int, SDL_Rect* = NULL);
	static void newMessage(std::string, int = 36, int = 300, SDL_Color = { 255, 255, 255, 255 }, Direction = DOWN, Direction = LEFT, bool = true);
	static void clearMessages(void);
	static void handleGameOverlay(int, int, bool = false);
	static void playGameEnd(bool = false);
	static SDL_Rect getViewport(void);

	static const int GFX_OFFSET;
	static const int BG_ALPHA_BASE;
	static const SDL_Rect MENU_RECT;
	static const float DEFAULT_PARTICLE_DENSITY;
	static float particleDensity;
	static std::vector<std::string> bgIDs;
	static std::vector<std::string> bgObjectIDs;
	static std::string rDir;
	static std::string rExt;
	static Texture* textBG;
	static std::string fontName;
	static TTF_Font* gFont;
	static TTF_Font* gSmallFont;
	// static SDL_Color gTextColor;
	static SDL_Color white;
	static SDL_Color black;
	static int gFontSize;
	static int gSmallFontSize;

private:
	static SDL_Rect viewport;
	static std::vector<Texture*> blockGFX;
	static std::vector<Texture*> collectibleGFX;
	static std::vector<Texture*> enemyGFX;
	static std::vector<std::pair<Texture*, int>> bgObjects;
	static std::vector<std::pair<Texture*, int>> particles;
	static std::vector<Message*> messages;
	// static std::vector<std::string> collectiblePickupMsgs;
	// static std::vector<Texture*> collectiblePickupTextures;
	static Texture* bg;
	static Texture* playerGFX;
	static std::pair<Texture*, std::string> winText;
	static std::vector<std::string> creditsText;
	static std::vector<Texture*> credits;
	static SDL_Color renderColor;
	static std::string blockPrefix;
	static std::string collectiblePrefix;
	static std::string enemyPrefix;
	static std::string playerPrefix;
	static std::string bgPrefix;
	static std::string bgObjectPrefix;
	static std::string particlePrefix;
	static std::string textBGPrefix;
};

