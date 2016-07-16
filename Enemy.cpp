#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "Window.h"
#include "Audio.h"

Enemy::Enemy(SDL_Rect pbox, int psubtype) : PhysicsEntity(pbox, ENEMY, psubtype)
{
	if (psubtype == ICE_BOSS)
	{
		rect.w *= 3;
		rect.h *= 3;
	}
}

Enemy::~Enemy()
{
}

void Enemy::onProcess()
{
	switch (subtype)
	{
	case ICE_BOSS:
		if (Game::Mode != BOSS)
		{
			SDL_Rect check1 = { Game::gPlayer->rect.x + Game::gPlayer->rect.w / 2 - 1, 0, 2, Window::geth() };
			SDL_Rect check2 = { rect.x + rect.w / 2 - 1, 0, 2, Window::geth() };
			if (Game::checkCollision(check1, check2))
				Game::Mode = BOSS;
		}
		break;
	}
}

void Enemy::handleMovements()
{
	if (subtype != ICE_BOSS)
	{
		if (collisions[DOWN] == NULL && aerialSpeed == 0)
			aerialSpeed = Game::GRAVITY_START;
		if (moveSpeed != 0)
			rect.x += moveSpeed;
		if (aerialSpeed != 0)
			rect.y += aerialSpeed;
		cycleAerials();
	}
}

void Enemy::cycleAerials()
{
	if (aerialSpeed == Game::JUMP_START)
		Audio::play(JUMP, 's');
	if (aerialSpeed < 0)
	{
		aerialSpeed *= Game::JUMP_MULT;
		if (aerialSpeed >= Game::JUMP_MAX)
			aerialSpeed = Game::GRAVITY_START;
	}
	else if (aerialSpeed > 0)
	{
		aerialSpeed *= Game::GRAVITY_MULT;
		if (aerialSpeed > Game::GRAVITY_MAX)
			aerialSpeed = Game::GRAVITY_MAX;
	}
}