#include "Menu.h"
#include "Window.h"
#include "Game.h"
#include "Texture.h"
#include "Graphics.h"
#include "Audio.h"
#include "Player.h"
#include "FileSystem.h"

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
std::vector<SDL_Rect> origStore;

// displayed on main menu (note: not part of Menu class)
std::vector<std::string> strings = { "Ice Age", "Copyright (C) 2016 Frozelar", "You may want to read TUTORIAL.txt before playing!" };

// player inventory, displayed when menu is open
std::vector<std::pair<Texture*, std::string>> Menu::inventory;
std::vector<Texture*> Menu::invImages;

// vector for store items
std::vector<std::pair<Texture*, std::string>> Menu::store;

// store stuff
std::unordered_map<std::string, int> Menu::allPrices;
static bool resetStore = true;  // <--- not part of menu class

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

	if (type == TITLE)
	{
		playMusic();
	}
	else if (type == STORE)
	{
		pauseType = PSTORE;
		if (resetStore)
		{
			createStore();
			resetStore = false;
		}
	}
	else
	{
		pauseType = MAIN;
		pausePos = title.size();
	}
	while (!quit && !done)
	{
		if(!Window::isVSync())
			fps = SDL_GetTicks();
		while (SDL_PollEvent(e) != NULL && !done)
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
	
	if (type == PAUSE || type == STORE)
	{
		if (e->type == SDL_KEYUP)
		{
			if (e->key.keysym.sym == Game::Controls["Pause"])
			{
				type = PAUSE;
				pauseType = MAIN;
				pausePos = title.size();
				Game::Mode = GAME;
				FileSystem::saveSettings();
				return true;
			}
		}
	}
	if (e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION)
	{
		if (type == STORE)
		{
			for (int i = 0; i < store.size(); i++)
			{
				if (Game::checkCollision(mrect, store[i].first->txRect))
				{
					if (e->type == SDL_MOUSEMOTION)
					{
						if (store[i].first->txRect.x == origStore[i].x)
						{
							store[i].first->txRect.x -= Game::UNIT_W / 4;
							store[i].first->txRect.y -= Game::UNIT_H / 4;
							store[i].first->txRect.w += Game::UNIT_W / 2;
							store[i].first->txRect.h += Game::UNIT_H / 2;
						}
						break;
					}
					else if (e->type == SDL_MOUSEBUTTONDOWN)
					{
						store[i].first->txRect = origStore[i];
					}
					else if (e->type == SDL_MOUSEBUTTONUP)
					{
						if (Game::gScore >= allPrices[store[i].second])
						{
							resetStore = true;
							Game::gPlayer->abilities[store[i].second] = true;
							Game::gScore -= allPrices[store[i].second];
							type = PAUSE;
							pauseType = MAIN;
							pausePos = title.size();
							Game::Mode = GAME;
							return true;
						}
					}
				}
			}
		}
		else
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
									FileSystem::saveSettings();
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
									FileSystem::saveSettings();
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
	}
	if (Game::Mode == STORE)
	{
		for (int i = 0; i < store.size(); i++)
			if (!Game::checkCollision(mrect, store[i].first->txRect))
				store[i].first->txRect = origStore[i];
	}
	else
	{
		for (int i = 0; i < options.size(); i++)
			if (!Game::checkCollision(mrect, options[i].first->txRect))
				options[i].first->txRect = origOptions[i];
	}
	return false;
}

void Menu::render()
{
	static float rot = 0;
	rot += Game::ROTATION_SPEED;
	if (rot >= 360)
		rot = 0;

	SDL_RenderClear(Window::renderer);
	if (type == PAUSE || type == STORE)
	{
		Graphics::renderAll(false);
		// Graphics::textBG->txRect = { 0, 0, static_cast<int>(Window::getw() * 0.25), Window::geth() };
		Graphics::textBG->txRect = { 0, 0, Window::getw(), Window::geth() };
		Graphics::textBG->txRender();
		// Graphics::textBG->txRect = { static_cast<int>(Window::getw() * 0.75), 0, static_cast<int>(Window::getw() * 0.25), Window::geth() };
		// Graphics::textBG->txRender();
	}
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
	if (type == STORE)
	{
		for (int i = 0; i < store.size(); i++)
		{
			store[i].first->txRender();

		}
	}
	else
	{
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
				if (pauseType == MAIN)
				{
					for (int i = 0; i < inventory.size(); i++)
					{
						if (Game::gPlayer->abilities[inventory[i].second])
						{
							inventory[i].first->txRender();
							invImages[i]->txRender();
						}
					}
				}
			}
		}
	}
	SDL_RenderPresent(Window::renderer);
}

void Menu::createStore(void)
{
	static int numOptions = 4;
	int randi = 0;
	std::stringstream opt;
	std::vector<bool> choices;
	int needtobreak = 0;
	for (int i = 0; i < inventory.size(); i++)
		choices.push_back(false);

	for (int i = 0; i < store.size(); i++)
	{
		if (store[i].first != NULL)
		{
			delete store[i].first;
			store[i].first = NULL;
		}
		store.pop_back();
		origStore.pop_back();
	}
	if (store.size() > 0)
	{
		store.resize(0);
		origStore.resize(0);
	}
	for (int i = 0; i < numOptions; i++)
	{
		do {
			randi = rand() % inventory.size();
			if (needtobreak++ > Game::gPlayer->abilities.size() * 2)
				return;
		} while (choices[randi] == true || Game::gPlayer->abilities[Game::playerIDs[randi]]);
		choices[randi] = true;

		store.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), inventory[randi].second));
		opt << store[i].second << " - " << allPrices[store[i].second] << " pts";
		store[i].first->txLoadT(opt.str(), Graphics::gFont, Graphics::black);
		opt.str("");

		if (i == 0)
		{
			store[i].first->txRect.x = Game::UNIT_W;
			store[i].first->txRect.y = Game::UNIT_H;
		}
		else
		{
			store[i].first->txRect.x = store[i - 1].first->txRect.x;
			store[i].first->txRect.y = store[i - 1].first->txRect.y + store[i - 1].first->txRect.h + Game::UNIT_H;
		}
		origStore.push_back(store[i].first->txRect);
	}
}

void Menu::init(void)
{
	int offset = 0;
	int MAXPRICE = 10, MINPRICE = 1;

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
	for (int i = 0; i < Game::gPlayer->abilities.size(); i++)
	{
		inventory.push_back(std::pair<Texture*, std::string>(new Texture(0, 0, 0, 0), ""));
		invImages.push_back(new Texture(0, 0, 0, 0));

		/*
		if (i < Game::playerIDs.size())
		{
		*/
		inventory.back().first->txLoadT(Game::playerIDs[i], Graphics::gFont, Graphics::black);
		inventory.back().second = Game::playerIDs[i];
		invImages.back()->txLoadF(Graphics::rDir + Graphics::playerPrefix + Graphics::plrGFXIDs[i].str() + Graphics::rExt);
		/*
		}
		else if (i - Game::playerIDs.size() < Game::collectibleIDs.size())
		{
			inventory.back().first->txLoadT(Game::collectibleIDs[i - Game::playerIDs.size()], Graphics::gFont, Graphics::black);
			inventory.back().second = Game::collectibleIDs[i - Game::playerIDs.size()];
			invImages.back()->txLoadF(Graphics::rDir + Graphics::collectiblePrefix + Graphics::colGFXIDs[i - Game::playerIDs.size()].str() + Graphics::rExt);
		}
		*/

		inventory.back().first->txRect.x = Window::getw() - Game::UNIT_W - inventory.back().first->txRect.w;
		inventory.back().first->txRect.y = Game::UNIT_H + (i == 0 ? 0 : inventory[i - 1].first->txRect.y + inventory[i - 1].first->txRect.h);
		invImages.back()->txRect.y = inventory.back().first->txRect.y;
		invImages.back()->txRect.x = inventory.back().first->txRect.x - invImages.back()->txRect.w - Game::UNIT_W;
	}
	for (int i = 0; i </* Game::collectibleIDs.size() + */Game::playerIDs.size(); i++)
	{
		/*
		if (i < Game::collectibleIDs.size())
			allPrices[Game::collectibleIDs[i]] = rand() % MAXPRICE + MINPRICE;
		else if (i - Game::collectibleIDs.size() < Game::playerIDs.size())
		*/
			allPrices[Game::playerIDs[i/* - Game::collectibleIDs.size()*/]] = rand() % MAXPRICE + MINPRICE;
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
	for (int i = 0; i < store.size(); i++)
	{
		if (store[i].first != NULL)
		{
			delete store[i].first;
			store[i].first = NULL;
		}
		store.pop_back();
	}
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
	for (int i = inventory.size() - 1; i >= 0; i--)
	{
		if (inventory[i].first != NULL)
		{
			delete inventory[i].first;
			inventory[i].first = NULL;
			inventory.pop_back();
		}
		if (invImages[i] != NULL)
		{
			delete invImages[i];
			invImages[i] = NULL;
			invImages.pop_back();
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