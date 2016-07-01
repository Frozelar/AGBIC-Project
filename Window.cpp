#include "Window.h"
#include "Graphics.h"

const int Window::DEFAULT_W = 1280;
const int Window::DEFAULT_H = 720;

SDL_Window* Window::window = NULL;
SDL_Renderer* Window::renderer = NULL;
int Window::w = 0;
int Window::h = 0;

Window::Window()
{
	init();
}

Window::~Window()
{
	close();
}

bool Window::init()
{
	w = DEFAULT_W;
	h = DEFAULT_H;
	window = SDL_CreateWindow("Ice Age", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return window != NULL && renderer != NULL;
}

void Window::close()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;
}

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

int Window::getw()
{
	return w;
}

int Window::geth()
{
	return h;
}