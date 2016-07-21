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

class Level
{
public:
	Level();
	~Level();

	static bool generateLevel(int);
	static void closeLevel(void);
	static bool begin(void);
	static bool end(void);
	static void setCollisions(void);
	// static void moveLevel(int = 0, int = 0);
	static int getw(char = 'u');
	static int geth(char = 'u');
	static int getTotal(char = 'u');
	static int getID(void);
	static int getBGID(void);
	static int getMusicID(void);

	// static int moveX;
	// static int moveY;
private:
	static std::string lDir;
	static std::string lExt;
	static int levelID;
	static int bgID;
	static int musicID;
	static int w;
	static int h;
	static int wPixels;
	static int hPixels;
	static int totalUnits;
	static int totalPixels;
};

