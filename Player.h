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
#include "Entities.h"

class Player : public PhysicsEntity
{
public:
	Player(SDL_Rect);
	~Player();

	void handleMovements(void);
	void cycleAerials(void);
	bool handleInput(SDL_Event*);
	void resetAbilities(void);

	int jumps;
	std::unordered_map<std::string, bool> abilities;
};

