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

#include "Audio.h"

// number of elements
const int Audio::TOTAL_MUSIC = 1;
const int Audio::TOTAL_SFX = 1;

// directories
std::string Audio::mDir = "Resources/";
std::string Audio::sDir = "Resources/";

// prefixes in file names
std::string Audio::mPrefix = "mus";
std::string Audio::sPrefix = "sfx";

// file extensions
std::string Audio::mExt = ".ogg";
std::string Audio::sExt = ".wav";

// store music/sfx
std::vector<Mix_Music*> Audio::music;
std::vector<Mix_Chunk*> Audio::sfx;

// identifiers (in file names)
std::vector<std::string> Audio::musicIDs = { "GlaciersPeak" };
std::vector<std::string> Audio::sfxIDs = { "Jump" };

// call init()
Audio::Audio()
{
	init();
}

// call close()
Audio::~Audio()
{
	close();
}

// initialize SDL_Mixer and load music and sfx
bool Audio::init()
{
	std::string ID = "";
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < NULL)
	{
		std::cout << "SDL_Mixer error: " << std::endl;
		std::cout << Mix_GetError() << std::endl;
		return false;
	}
	for (int i = 0; i < TOTAL_MUSIC; i++)
	{
		ID = mDir + mPrefix + musicIDs[i] + mExt;
		music.push_back(Mix_LoadMUS(ID.c_str()));
	}
	for (int i = 0; i < TOTAL_SFX; i++)
	{
		ID = sDir + sPrefix + sfxIDs[i] + sExt;
		sfx.push_back(Mix_LoadWAV(ID.c_str()));
	}
	return true;
}

// play specified audio
// int = audio ID, char = 'm' for music or 's' for sound effect
bool Audio::play(int which, char type)
{
	if (type == 'm')
	{
		if (Mix_PlayMusic(music[which], -1) < 0)
		{
			std::cout << "Error playing music: " << Mix_GetError() << std::endl;
			return false;
		}
	}
	else if (type == 's')
	{
		if (Mix_PlayChannel(-1, sfx[which], 0) < 0)
		{
			std::cout << "Error playing sound effect: " << Mix_GetError() << std::endl;
			return false;
		}
	}
	else
		return false;
	return true;
}

// free everything in music and sfx vectors
void Audio::close()
{
	for (int i = 0; i < music.size(); i++)
	{
		Mix_FreeMusic(music[i]);
		music[i] = NULL;
		music.pop_back();
	}
	for (int i = 0; i < sfx.size(); i++)
	{
		Mix_FreeChunk(sfx[i]);
		sfx[i] = NULL;
		sfx.pop_back();
	}
}