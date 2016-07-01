#pragma once
#include "Include.h"
#include "Entities.h"

class Player : public PhysicsEntity
{
public:
	Player(SDL_Rect);
	~Player();

	// void handleMovements(void);
	// void handleAerials(void);
	bool handleInput(SDL_Event*);
};

