#pragma once
#include "Include.h"

class Menu
{
public:
	Menu();
	~Menu();

	static void init(void);
	static void clear(void);
	static bool loop(int, SDL_Rect, SDL_Event*);
	static int handleEvent(SDL_Event*);
	static void render(void);
	static void playMusic(void);

private:
	static int type;
	static std::vector<std::pair<Texture*, std::string>> options;
	static std::vector<std::pair<Texture*, std::string>> notes;
	static Texture* titleBG;
	static std::string titleBGPrefix;
	static std::string titleMusicPrefix;
	static SDL_Rect rect;
};

