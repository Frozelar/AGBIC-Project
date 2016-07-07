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

#include "Window.h"
#include "Graphics.h"

// default window dimensions
const int Window::DEFAULT_W = 1280;
const int Window::DEFAULT_H = 720;

// window object
SDL_Window* Window::window = NULL;

// renderer object
SDL_Renderer* Window::renderer = NULL;

// actual window dimensions
int Window::w = 0;
int Window::h = 0;

// call init()
Window::Window()
{
	init();
}

// call close()
Window::~Window()
{
	close();
}

// initialize dimensions, create window and renderer objects, return success
bool Window::init()
{
	w = DEFAULT_W;
	h = DEFAULT_H;
	window = SDL_CreateWindow("Ice Age", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return window != NULL && renderer != NULL;
}

// destroy window and renderer
void Window::close()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;
}

// handle given window-related event
bool Window::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_WINDOWEVENT)
	{
		switch (e->window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			w = e->window.data1;
			h = e->window.data2;
			SDL_RenderSetLogicalSize(renderer, w, h);
			SDL_RenderPresent(renderer);
			break;
		default:
			return false;
		}
	}
	/*	ADD LATER
	else if (e->type == SDL_KEYUP)
	{
		switch (e->key.keysym.sym)
		{
		case Game::Controls["Fullscreen"]:

			break;
		}
	}
	*/
	return true;
}

/*
SDL_Renderer* Window::getRenderer()
{
	return renderer;
}
*/

// return window width
int Window::getw()
{
	return w;
}

// return window height
int Window::geth()
{
	return h;
}