#pragma once
#include "Include.h"
#include "Entities.h"

class Enemy : public PhysicsEntity
{
public:
	Enemy(SDL_Rect, int);
	~Enemy();

	void onProcess(void);
	void handleMovements(void);
	void cycleAerials(void);

private:
	SDL_Rect bounds;
	int targetMoveSpeed;
	int targetAerialSpeed;
};

