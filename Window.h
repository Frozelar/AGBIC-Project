#pragma once
#include "Include.h"

class Window
{
public:
	Window();
	~Window();

	static bool init(void);
	static void close(void);
	static bool handleEvent(SDL_Event* e);
	static int getw(void);
	static int geth(void);

	static const int DEFAULT_W;
	static const int DEFAULT_H;
	static SDL_Renderer* renderer;

private:
	static SDL_Window* window;
	static int w;
	static int h;
};

