#pragma once
#include "Include.h"

enum PauseType {
	PSTORE, MAIN, SETTINGS, CONTROLS, GRAPHICS, AUDIO
};

class Menu
{
	friend class Graphics;
public:
	Menu();
	~Menu();

	static void init(void);
	static void clear(void);
	static bool loop(int, SDL_Rect, SDL_Event*);
	static int handleEvent(SDL_Event*);
	static void render(void);
	static void playMusic(void);
	static void createStore(void);

private:
	static int type;
	static int pauseType;
	static int pausePos;
	static std::vector<std::pair<Texture*, std::string>> options;
	static std::vector<std::pair<Texture*, std::string>> notes;
	static std::vector<std::pair<Texture*, std::string>> inventory;
	static std::vector<std::pair<Texture*, std::string>> store;
	static std::vector<Texture*> invImages;
	static Texture* titleBG;
	static std::string titleBGPrefix;
	static std::string titleMusicPrefix;
	static SDL_Rect rect;
};

