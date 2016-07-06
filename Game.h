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

class Game
{
public:
	Game();
	~Game();

	static bool init(void);
	static void close(void);
	static bool checkCollision(PhysicsEntity*, bool = true, int = -1);
	static bool checkCollision(SDL_Rect, SDL_Rect);
	static int findCollision(PhysicsEntity*, SDL_Rect);
	static bool newEntity(SDL_Rect, int, int);
	static void clearEntities(void);
	static void process(void);
	// static void manageBuffers(void);

	static SDL_Event inputEvent;
	static int score;
	static std::vector<StaticEntity*> allEntities;
	static std::vector<StaticEntity*> staticEntities;
	static std::vector<PhysicsEntity*> dynamicEntities;
	static std::vector<StaticEntity*> renderedEntities;
	static std::vector<StaticEntity*> collisionEntities;
	static std::vector<Collectible*> collectibles;
	static Player* gPlayer;
	static std::vector<std::string> blockIDs;
	static std::vector<std::string> collectibleIDs;
	static std::vector<std::string> particleIDs;
	static const int UNIT_W;
	static const int UNIT_H;
	static const float GRAVITY_START;
	static const float GRAVITY_MULT;
	static const float GRAVITY_MAX;
	static const float JUMP_MAX;
	static const float JUMP_MULT;
	static const float JUMP_START;
	static const int MOVE_SPEED;
	static const float ROTATION_SPEED;
	static const int BOB_SPEED;
	static std::map<std::string, int> Controls;
	static std::vector<int> entityOffset;
	// static std::vector<StaticEntity*> destroyBuffer;
};

