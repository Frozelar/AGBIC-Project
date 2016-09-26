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
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <math.h>
#include <random>
#include <time.h>
#include <unordered_map>
#include <algorithm>

class Window;
class Texture;
class Graphics;
class Game;
class Entity;
class StaticEntity;
class PhysicsEntity;
class Player;
class Collectible;
class Level;
class Audio;
class Message;
class Menu;
class Enemy;

enum Direction {
	UP, DOWN, LEFT, RIGHT, TOTAL_DIRECTIONS
};

int invertDir(int);

/*
struct DirContainer {
	int up = -1;
	int down = -1;
	int left = -1;
	int right = -1;
};
*/