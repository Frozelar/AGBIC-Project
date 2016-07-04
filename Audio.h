#pragma once
#include "Include.h"

class Audio
{
public:
	Audio();
	~Audio();

	static bool init(void);
	static bool play(int, char);
	static void close(void);

	static const int TOTAL_MUSIC;
	static const int TOTAL_SFX;
private:
	static std::string mDir;
	static std::string sDir;
	static std::string mExt;
	static std::string sExt;
	static std::vector<Mix_Chunk*> sfx;
	static std::vector<Mix_Music*> music;
};

