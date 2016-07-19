#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "Window.h"
#include "Audio.h"
#include "Level.h"
#include "Graphics.h"

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
		static int eyeIndex = 0;
		if (Game::Mode != BOSS)
		{
			SDL_Rect check1 = { Game::gPlayer->rect.x + Game::gPlayer->rect.w / 2 - 1, 0, 2, Window::geth() };
			SDL_Rect check2 = { rect.x + rect.w / 2 - 1, 0, 2, Window::geth() };
			if (Game::checkCollision(check1, check2))
			{
				Game::Mode = BOSS;
				Game::newEntity({ (rect.x + rect.w / 2) - (Game::UNIT_W / 2), (rect.y + rect.h / 2) - (Game::UNIT_H / 2), Game::UNIT_W, Game::UNIT_H }, ENEMY, EYE);
				eyeIndex = Game::enemies.size() - 1;
			}
		}
		if (Game::Mode == BOSS)
		{
			int enemychance = 16;
			int eyechance = 256;
			static bool eye = false;
			if (Game::enemies[eyeIndex] == NULL || Game::enemies[eyeIndex]->getSubtype() != EYE)
			{
				SDL_Rect* r;
				for (int i = 0; i < 128; i++)
				{
					r = new SDL_Rect;
					r->x = rect.x + (rand() % rect.w);
					r->y = rect.y + (rand() % rect.h);
					r->w = rand() % 4 + 1;
					r->h = r->w;
					Graphics::spawnParticle(SNOW, r);
					delete r;
					r = NULL;
				}
				Game::Mode = GAME_END;
				Game::enemySpawnChance = 0;
				Audio::play(EXPLODE, 's');
				eye = false;
				destroy = true;
				break;
			}
			if (!eye)
			{
				Game::enemies[eyeIndex]->rect.x = (rect.x + rect.w / 2) - (Game::UNIT_W / 2);
				Game::enemies[eyeIndex]->rect.y = (rect.y + rect.h / 2) - (Game::UNIT_H / 2);
			}
			if (rand() % enemychance == 1)
			{
				int size = (rand() % 2 == 0 ? 2 : 1);
				Game::newEntity({ rect.x + rect.w / 2, rect.y + rect.h / 2, Game::UNIT_W / size, Game::UNIT_H / size }, ENEMY, ICE);
				Game::enemies.back()->moveSpeed += (rand() % 9) * (rand() % 2 == 0 ? -1 : 1);
				Game::enemies.back()->aerialSpeed = Game::JUMP_START - (rand() % ((int)Game::JUMP_START + 1));
			}
			if (!eye && rand() % eyechance == 1)
			{
				Game::enemies[eyeIndex]->moveSpeed = (rand() % 5) * (rand() % 2 == 0 ? -1 : 1);
				Game::enemies[eyeIndex]->aerialSpeed = Game::JUMP_START - (rand() % ((int)Game::JUMP_START + 1));
				eye = true;
			}
			if (eye)
			{
				//if (collisions[DOWN] == NULL && aerialSpeed == 0)
				//	aerialSpeed = Game::GRAVITY_START;
				if (Game::enemies[eyeIndex]->moveSpeed != 0)
					Game::enemies[eyeIndex]->rect.x += Game::enemies[eyeIndex]->moveSpeed;
				if (Game::enemies[eyeIndex]->aerialSpeed != 0)
					Game::enemies[eyeIndex]->rect.y += Game::enemies[eyeIndex]->aerialSpeed;
				Game::enemies[eyeIndex]->cycleAerials();
				if (Game::enemies[eyeIndex]->rect.y + Game::enemies[eyeIndex]->rect.h > Level::geth('p'))
					eye = false;
			}
		}
		break;
	}

	//if (rect.y > Level::geth('p'))
	//	destroy = true;
}

void Enemy::handleMovements()
{
	if (subtype != ICE_BOSS && subtype != EYE)
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
	// if (aerialSpeed == Game::JUMP_START)
	// 	Audio::play(JUMP, 's');
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