#include "Menu.h"
#include "Window.h"
#include "Game.h"
#include "Texture.h"
#include "Graphics.h"
#include "Audio.h"

// which menu
int Menu::type = -1;
int Menu::pauseType = -1;

// which position in options[] when in menu
int Menu::pausePos = -1;

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
std::vector<std::string> pause = { "Resume",  "Settings", "Exit to Title" };
std::vector<std::string> settings = { "Controls", "Graphics", "Audio", "Return" };
// std::vector<std::pair<std::string, std::vector<std::string>>> settings = { {"Controls", {  }}, {"Graphics", { "Toggle Fullscreen", "Toggle VSync" }}, {"Audio", {  }} };
std::vector<std::string> controls = { "Return" };
std::vector<std::string> graphics = { "Fullscreen: ", "VSync: ", "Return" };
std::vector<std::string> audio = { "Music: ", "Sound Effects: ", "Return" };
std::vector<SDL_Rect> origOptions;

// displayed on main menu (note: not part of Menu class)
std::vector<std::string> strings = { "Ice Age", "Copyright (C) 2016 Frozelar", "You may want to read TUTORIAL.txt before playing!" };

// used for various settings (note: not part of Menu class)
Texture enabled;
Texture disabled;
Texture musVolume;
Texture sfxVolume;

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
	Uint32 fps = 0;
	bool quit = false;
	int done = false;

	if(type == TITLE)
		playMusic();
	else
	{
		pauseType = MAIN;
		pausePos = title.size();
	}
	while (!quit && !done)
	{
		if(!Window::isVSync())
			fps = SDL_GetTicks();
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
		if(!Window::isVSync())
			if (1000 / Game::FPS > SDL_GetTicks() - fps)
				SDL_Delay((1000 / Game::FPS) - (SDL_GetTicks() - fps));
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
	//if (e->type == SDL_KEYUP)
	//	if (e->key.keysym.sym == Game::Controls["Fullscreen"])
	//		Window::toggleFullscreen();
	
	if (e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION)
	{
		for (int i = 0; i < options.size(); i++)
		{
			if (type == TITLE)
			{
				if (Game::checkCollision(mrect, options[i].first->txRect))
				{
					if (e->type == SDL_MOUSEMOTION)
					{
						if (options[i].first->txRect.x == origOptions[i].x)
						{
							options[i].first->txRect.x -= Game::UNIT_W / 4;
							options[i].first->txRect.y -= Game::UNIT_H / 4;
							options[i].first->txRect.w += Game::UNIT_W / 2;
							options[i].first->txRect.h += Game::UNIT_H / 2;
						}
						break;
					}
					else if (e->type == SDL_MOUSEBUTTONDOWN)
					{
						options[i].first->txRect = origOptions[i];
					}
					else if (e->type == SDL_MOUSEBUTTONUP)
					{
						if (options[i].second == "Play Game")
						{
							Game::Mode = LEVEL_BEGIN;
							return true;
						}
						else if (options[i].second == "Exit")
						{
							return -1;
						}
					}
				}
			}
			else if (type == PAUSE)
			{
				//if (i < title.size())
				//	i = title.size();
				if (i < pausePos)
					i = pausePos;
				if (Game::checkCollision(mrect, options[i].first->txRect))
				{
					if (e->type == SDL_MOUSEMOTION)
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
					else if (e->type == SDL_MOUSEBUTTONUP)
					{
						if (pauseType == MAIN)
						{
							if (options[i].second == "Resume")
							{
								Game::Mode = GAME;
								return true;
							}
							else if (options[i].second == "Settings")
							{
								pauseType = SETTINGS;
								pausePos = title.size() + pause.size();
								break;
							}
							else if (options[i].second == "Exit to Title")
							{
								Game::Mode = TITLE;
								return true;
							}
						}
						else if (pauseType == SETTINGS)
						{
							if (options[i].second == "Controls")
							{
								pauseType = CONTROLS;
								pausePos = title.size() + pause.size() + settings.size();
								break;
							}
							else if (options[i].second == "Graphics")
							{
								pauseType = GRAPHICS;
								pausePos = title.size() + pause.size() + settings.size() + controls.size();
								break;
							}
							else if (options[i].second == "Audio")
							{
								pauseType = AUDIO;
								pausePos = title.size() + pause.size() + settings.size() + controls.size() + graphics.size();
								break;
							}
							else if (options[i].second == "Return")
							{
								pauseType = MAIN;
								pausePos = title.size();
								break;
							}
						}
						else if (pauseType == CONTROLS)
						{
							if (options[i].second == "Return")
							{
								pauseType = SETTINGS;
								pausePos = title.size() + pause.size();
								break;
							}
						}
						else if (pauseType == GRAPHICS)
						{
							if (options[i].second == "Fullscreen: ")
							{
								Window::toggleFullscreen();
								break;
							}
							else if (options[i].second == "VSync: ")
							{
								Window::toggleVSync();
								break;
							}
							else if (options[i].second == "Return")
							{
								pauseType = SETTINGS;
								pausePos = title.size() + pause.size();
								break;
							}
						}
						else if (pauseType == AUDIO)
						{
							std::cout << options[i].second << std::endl;
							if (options[i].second == "Music: ")
							{
								if (Audio::getVolume('m') == 100)
									Audio::setVolume(0, 'm');
								else
									Audio::setVolume(Audio::getVolume('m') + 10, 'm');
								break;
							}
							else if (options[i].second == "Sound Effects: ")
							{
								if (Audio::getVolume('s') == 100)
									Audio::setVolume(0, 's');
								else
									Audio::setVolume(Audio::getVolume('s') + 10, 's');
								break;
							}
							else if (options[i].second == "Return")
							{
								pauseType = SETTINGS;
								pausePos = title.size() + pause.size();
								break;
							}
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
		else if (type == PAUSE)
		{
			if ((pauseType == MAIN && i < pause.size()) || (pauseType == SETTINGS && i < settings.size()) || (pauseType == CONTROLS && i < controls.size()) ||
				(pauseType == GRAPHICS && i < graphics.size()) || (pauseType == AUDIO && i < audio.size()))
			{
				options[i /* + title.size() */ + pausePos].first->txRender();
				if (options[i + pausePos].second == "Fullscreen: ")
				{
					if (Window::isFullscreen())
					{
						enabled.txRect.x = options[i + pausePos].first->txRect.x + options[i + pausePos].first->txRect.w + Game::UNIT_W;
						enabled.txRect.y = options[i + pausePos].first->txRect.y;
						enabled.txRender();
					}
					else
					{
						disabled.txRect.x = options[i + pausePos].first->txRect.x + options[i + pausePos].first->txRect.w + Game::UNIT_W;
						disabled.txRect.y = options[i + pausePos].first->txRect.y;
						disabled.txRender();
					}
				}
				else if (options[i + pausePos].second == "VSync: ")
				{
					if (Window::isVSync())
					{
						enabled.txRect.x = options[i + pausePos].first->txRect.x + options[i + pausePos].first->txRect.w + Game::UNIT_W;
						enabled.txRect.y = options[i + pausePos].first->txRect.y;
						enabled.txRender();
					}
					else
					{
						disabled.txRect.x = options[i + pausePos].first->txRect.x + options[i + pausePos].first->txRect.w + Game::UNIT_W;
						disabled.txRect.y = options[i + pausePos].first->txRect.y;
						disabled.txRender();
					}
				}
				else if (options[i + pausePos].second == "Music: ")
				{
					musVolume.txRect.w = musVolume.txRect.h = 0;
					musVolume.txLoadT(std::to_string(Audio::getVolume('m')), Graphics::gFont, Graphics::black);
					musVolume.txRect.x = options[i + pausePos].first->txRect.x + options[i + pausePos].first->txRect.w + Game::UNIT_W;
					musVolume.txRect.y = options[i + pausePos].first->txRect.y;
					musVolume.txRender();
				}
				else if (options[i + pausePos].second == "Sound Effects: ")
				{
					sfxVolume.txRect.w = sfxVolume.txRect.h = 0;
					sfxVolume.txLoadT(std::to_string(Audio::getVolume('s')), Graphics::gFont, Graphics::black);
					sfxVolume.txRect.x = options[i + pausePos].first->txRect.x + options[i + pausePos].first->txRect.w + Game::UNIT_W;
					sfxVolume.txRect.y = options[i + pausePos].first->txRect.y;
					sfxVolume.txRender();
				}
			}
		}
	}
	SDL_RenderPresent(Window::renderer);
}

void Menu::init(void)
{
	int offset = 0;
	for (int i = 0; i < title.size() + pause.size() + settings.size() + controls.size() + graphics.size() + audio.size(); i++)
	{
		if (i < title.size())
		{
			offset = 0;
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), title[i]));
			options[i].first->txLoadT(title[i], Graphics::gFont, Graphics::black);
		}
		else if (i - title.size() < pause.size())
		{
			offset = title.size();
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), pause[i - offset]));
			options[i].first->txLoadT(pause[i - offset], Graphics::gFont, Graphics::black);
		}
		else if (i - title.size() - pause.size() < settings.size())
		{
			offset = title.size() + pause.size();
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), settings[i - offset]));
			options[i].first->txLoadT(settings[i - offset], Graphics::gFont, Graphics::black);
		}
		else if (i - title.size() - pause.size() - settings.size() < controls.size())
		{
			offset = title.size() + pause.size() + settings.size();
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), controls[i - offset]));
			options[i].first->txLoadT(controls[i - offset], Graphics::gFont, Graphics::black);
		}
		else if (i - title.size() - pause.size() - settings.size() - controls.size() < graphics.size())
		{
			offset = title.size() + pause.size() + settings.size() + controls.size();
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), graphics[i - offset]));
			options[i].first->txLoadT(graphics[i - offset], Graphics::gFont, Graphics::black);
		}
		else if (i - title.size() - pause.size() - settings.size() - controls.size() - graphics.size() < audio.size())
		{
			offset = title.size() + pause.size() + settings.size() + controls.size() + graphics.size();
			options.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), audio[i - offset]));
			options[i].first->txLoadT(audio[i - offset], Graphics::gFont, Graphics::black);
		}
		options[i].first->txRect.x = (i > offset ? options[i - 1].first->txRect.x : rect.x + Game::UNIT_W);
		options[i].first->txRect.y = (i > offset ? options[i - 1].first->txRect.y + options[i - 1].first->txRect.h + Game::UNIT_H : rect.y + Game::UNIT_H);
		origOptions.push_back(options[i].first->txRect);
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
	enabled.txRect = { 0, 0, 0, 0 };
	enabled.txLoadT("Enabled", Graphics::gFont, Graphics::black);
	disabled.txRect = { 0, 0, 0, 0 };
	disabled.txLoadT("Disabled", Graphics::gFont, Graphics::black);
	musVolume.txRect = { 0, 0, 0, 0 };
	musVolume.txLoadT(std::to_string(Audio::getVolume('m')), Graphics::gFont, Graphics::black);
	sfxVolume.txRect = { 0, 0, 0, 0 };
	sfxVolume.txLoadT(std::to_string(Audio::getVolume('s')), Graphics::gFont, Graphics::black);
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