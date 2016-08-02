#include "Menu.h"
#include "Window.h"
#include "Game.h"
#include "Texture.h"
#include "Graphics.h"
#include "Audio.h"

// which menu
int Menu::type = -1;

// all menu options
std::vector<std::pair<Texture*, std::string>> Menu::options;

// everything else displayed on the title screen
std::vector<std::pair<Texture*, std::string>> Menu::notes;

// background that is displayed on the title screen
Texture* Menu::titleBG = NULL;

// used in file loading
std::string Menu::titleBGPrefix = "titleBG";
std::string Menu::titleMusicPrefix = "titleMusic";

// current menu rect
SDL_Rect Menu::rect;

// option strings (note: not part of Menu class)
std::vector<std::string> title = { "Play Game", "Exit" };
std::vector<std::string> pause = { "Resume", "Exit to Title" };
std::vector<SDL_Rect> origOptions;

// displayed on main menu (note: not part of Menu class)
std::vector<std::string> strings = { "Ice Age", "Copyright (C) 2016 Frozelar", "You may want to read TUTORIAL.txt before playing!" };

Menu::Menu()
{
	init();
}

Menu::~Menu()
{
	clear();
}

bool Menu::loop(int ptype, SDL_Rect prect, SDL_Event* e)
{
	// clear();
	type = ptype;
	rect = prect;
	// init();
	bool quit = false;
	int done = false;

	if(type == TITLE)
		playMusic();
	while (!quit && !done)
	{
		while (SDL_PollEvent(e) != NULL)
		{
			switch (e->type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			//case SDL_WINDOWEVENT:
			//	Window::handleEvent(e);
			//	break;
			default:
				done = handleEvent(e);
				break;
			}
		}
		render();

		if (done == -1)
			quit = true;
	}
	return quit;
}

int Menu::handleEvent(SDL_Event* e)
{
	static int mx = 0, my = 0;
	SDL_Rect mrect = { mx, my, 1, 1 };

	if (e->type == SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&mx, &my);
		mrect = { mx, my, 1, 1 };
	}
	// else if (e->type == SDL_MOUSEBUTTONDOWN)
	// {

	// }
	if (e->type == SDL_KEYUP)
		if (e->key.keysym.sym == Game::Controls["Fullscreen"])
			Window::toggleFullscreen();
	
	if (e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION)
	{
		for (int i = 0; i < options.size(); i++)
		{
			if (type == TITLE)
			{
				if (Game::checkCollision(mrect, options[i].first->txRect))
				{
					if (options[i].second == "Play Game")
					{
						if (e->type == SDL_MOUSEBUTTONUP)
						{
							Game::Mode = LEVEL_BEGIN;
							return true;
						}
						else if (e->type == SDL_MOUSEMOTION)
						{
							if (options[i].first->txRect.x == origOptions[i].x)
							{
								options[i].first->txRect.x -= Game::UNIT_W / 4;
								options[i].first->txRect.y -= Game::UNIT_H / 4;
								options[i].first->txRect.w += Game::UNIT_W / 2;
								options[i].first->txRect.h += Game::UNIT_H / 2;
							}
						}
						else if (e->type == SDL_MOUSEBUTTONDOWN)
						{
							options[i].first->txRect = origOptions[i];
						}
					}
					if (options[i].second == "Exit")
					{
						if (e->type == SDL_MOUSEBUTTONUP)
						{
							return -1;
						}
						else if (e->type == SDL_MOUSEMOTION)
						{
							if (options[i].first->txRect.x == origOptions[i].x)
							{
								options[i].first->txRect.x -= Game::UNIT_W / 4;
								options[i].first->txRect.y -= Game::UNIT_H / 4;
								options[i].first->txRect.w += Game::UNIT_W / 2;
								options[i].first->txRect.h += Game::UNIT_H / 2;
							}
						}
						else if (e->type == SDL_MOUSEBUTTONDOWN)
						{
							options[i].first->txRect = origOptions[i];
						}
					}
				}
			}
			else if (type == PAUSE)
			{
				if (i < title.size())
					i = title.size();
				if (Game::checkCollision(mrect, options[i].first->txRect))
				{
					if (options[i].second == "Resume")
					{
						if (e->type == SDL_MOUSEBUTTONUP)
						{
							Game::Mode = GAME;
							return true;
						}
						else if (e->type == SDL_MOUSEMOTION)
						{
							if (options[i].first->txRect.x == origOptions[i].x)
							{
								options[i].first->txRect.x -= Game::UNIT_W / 4;
								options[i].first->txRect.y -= Game::UNIT_H / 4;
								options[i].first->txRect.w += Game::UNIT_W / 2;
								options[i].first->txRect.h += Game::UNIT_H / 2;
							}
						}
						else if (e->type == SDL_MOUSEBUTTONDOWN)
						{
							options[i].first->txRect = origOptions[i];
						}
					}
					if (options[i].second == "Exit to Title")
					{
						if (e->type == SDL_MOUSEBUTTONUP)
						{
							Game::Mode = TITLE;
							return true;
						}
						else if (e->type == SDL_MOUSEMOTION)
						{
							if (options[i].first->txRect.x == origOptions[i].x)
							{
								options[i].first->txRect.x -= Game::UNIT_W / 4;
								options[i].first->txRect.y -= Game::UNIT_H / 4;
								options[i].first->txRect.w += Game::UNIT_W / 2;
								options[i].first->txRect.h += Game::UNIT_H / 2;
							}
						}
						else if (e->type == SDL_MOUSEBUTTONDOWN)
						{
							options[i].first->txRect = origOptions[i];
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < options.size(); i++)
		if (!Game::checkCollision(mrect, options[i].first->txRect))
			options[i].first->txRect = origOptions[i];
	return false;
}

void Menu::render()
{
	static float rot = 0;
	rot += Game::ROTATION_SPEED;
	if (rot >= 360)
		rot = 0;

	SDL_RenderClear(Window::renderer);
	if (type == PAUSE)
		Graphics::renderAll(false);
	if (type == TITLE)
	{
		titleBG->txRender();
		for (int i = 0; i < notes.size(); i++)
		{
			if (i == 0)
				notes[i].first->txRender(NULL, NULL, rot, SDL_FLIP_NONE);
			else
				notes[i].first->txRender();
		}
	}
	for (int i = 0; i < options.size(); i++)
	{
		if (type == TITLE && i < title.size())
			options[i].first->txRender();
		else if (type == PAUSE && i < pause.size())
			options[i + title.size()].first->txRender();
	}
	SDL_RenderPresent(Window::renderer);
}

void Menu::init(void)
{
	for (int i = 0; i < title.size() + pause.size(); i++)
	{
		if (i < title.size())
		{
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), title[i]));
			options[i].first->txLoadT(title[i], Graphics::gFont, Graphics::black);
			options[i].first->txRect.x = (i > 0 ? options[i - 1].first->txRect.x : rect.x + Game::UNIT_W);
			options[i].first->txRect.y = (i > 0 ? options[i - 1].first->txRect.y + options[i - 1].first->txRect.h + Game::UNIT_H : rect.y + Game::UNIT_H);
			origOptions.push_back(options[i].first->txRect);
		}
		else if (i - title.size() < pause.size())
		{
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), pause[i - title.size()]));
			options[i].first->txLoadT(pause[i - title.size()], Graphics::gFont, Graphics::black);
			options[i].first->txRect.x = (i > title.size() ? options[i - 1].first->txRect.x : rect.x + Game::UNIT_W);
			options[i].first->txRect.y = (i > title.size() ? options[i - 1].first->txRect.y + options[i - 1].first->txRect.h + Game::UNIT_H : rect.y + Game::UNIT_H);
			origOptions.push_back(options[i].first->txRect);
		}
	}
	for (int i = 0; i < strings.size(); i++)
	{
		notes.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), strings[i]));

		// may want to change this later to be more automated or something I don't know
		if (i == 0)
		{
			notes[i].first->txLoadT(strings[i], Graphics::gLargeFont, Graphics::black);
			notes[i].first->txRect.x = Window::getw() - notes[i].first->txRect.w - Game::UNIT_W * 4;
			notes[i].first->txRect.y = Game::UNIT_H * 4;
		}
		else if (i == 1)
		{
			notes[i].first->txLoadT(strings[i], Graphics::gSmallFont, Graphics::black);
			notes[i].first->txRect.x = Window::getw() - notes[i].first->txRect.w - Game::UNIT_W;
			notes[i].first->txRect.y = Window::geth() - notes[i].first->txRect.h - Game::UNIT_H;
		}
		else if (i == 2)
		{
			notes[i].first->txLoadT(strings[i], Graphics::gSmallFont, Graphics::black);
			notes[i].first->txRect.x = options[title.size() - 1].first->txRect.x;
			notes[i].first->txRect.y = options[title.size() - 1].first->txRect.y + options[title.size() - 1].first->txRect.h + Game::UNIT_H;
		}
	}
	titleBG = new Texture(0, 0, 0, 0);
	titleBG->txLoadF(Graphics::rDir + titleBGPrefix + Graphics::rExt);
}

void Menu::clear()
{
	for (int i = options.size() - 1; i >= 0; i--)
	{
		if (options[i].first != NULL)
		{
			delete options[i].first;
			options[i].first = NULL;
			options.pop_back();
		}
	}
	for (int i = notes.size() - 1; i >= 0; i--)
	{
		if (notes[i].first != NULL)
		{
			delete notes[i].first;
			notes[i].first = NULL;
			notes.pop_back();
		}
	}
	if (titleBG != NULL)
	{
		delete titleBG;
		titleBG = NULL;
	}
	rect = { 0, 0, 0, 0 };
	type = -1;
}

void Menu::playMusic()
{
	Audio::play(TITLE, 'm');
}