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

enum SFXType {
	JUMP, COLLECT, EXPLODE
};

class Audio
{
public:
	Audio();
	~Audio();

	static bool init(void);
	static bool play(int, char);
	static void setVolume(int, char);
	static int getVolume(char);
	static void close(void);

	static std::vector<std::string> musicIDs;
	static std::vector<std::string> sfxIDs;

private:
	static int musVol;
	static int sfxVol;
	static std::string mDir;
	static std::string sDir;
	static std::string mPrefix;
	static std::string sPrefix;
	static std::string mExt;
	static std::string sExt;
	static std::vector<Mix_Chunk*> sfx;
	static std::vector<Mix_Music*> music;
};

