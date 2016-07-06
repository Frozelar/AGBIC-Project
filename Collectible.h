#pragma once
#include "Entities.h"
#include "Include.h"

class Collectible : public StaticEntity
{
public:
	Collectible(SDL_Rect, int);
	~Collectible();

	// void collect(void);
	// void bob(void);
	void onProcess(void);
	void onDestroy(void);

private:
	int bobSpeed;
	int direction;
};