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
	void manageCollisionsX(void);
	void manageCollisionsY(void);
	// void emptyCollidersX(void);
	// void emptyCollidersY(void);
	// static void doDefaultActions(void);

private:
	SDL_Rect bounds;
	int targetMoveSpeed;
	int targetAerialSpeed;
};

