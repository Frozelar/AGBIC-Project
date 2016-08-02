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

class Window
{
public:
	Window();
	~Window();

	static bool init(void);
	static void close(void);
	static bool handleEvent(SDL_Event* e);
	static void toggleFullscreen(void);
	static int getw(void);
	static int geth(void);

	static const int DEFAULT_W;
	static const int DEFAULT_H;
	static SDL_Renderer* renderer;

private:
	static SDL_Window* window;
	static bool fullscreen;
	static int w;
	static int h;
};

