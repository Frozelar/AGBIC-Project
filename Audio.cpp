#include "Audio.h"

const int Audio::TOTAL_MUSIC = 1;
const int Audio::TOTAL_SFX = 1;
std::string Audio::mDir = "Resources/";
std::string Audio::sDir = "Resources/";
std::string Audio::mExt = ".wav";
std::string Audio::sExt = ".wav";
std::vector<Mix_Music*> Audio::music;
std::vector<Mix_Chunk*> Audio::sfx;

Audio::Audio()
{
	init();
}

Audio::~Audio()
{
	close();
}

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
		ID = mDir + "music" + std::to_string(i) + mExt;
		music.push_back(Mix_LoadMUS(ID.c_str()));
	}
	for (int i = 0; i < TOTAL_SFX; i++)
	{
		ID = sDir = "sfx" + std::to_string(i) + mExt;
		sfx.push_back(Mix_LoadWAV(ID.c_str()));
	}
	return true;
}

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