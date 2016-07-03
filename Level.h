#pragma once
#include "Include.h"

class Level
{
public:
	Level();
	~Level();

	static bool generateLevel(int);
	static void closeLevel(void);
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

