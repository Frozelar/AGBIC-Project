#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

class Window;
class Texture;
class Graphics;
class Game;
class Entity;
class StaticEntity;
class PhysicsEntity;
class Player;

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