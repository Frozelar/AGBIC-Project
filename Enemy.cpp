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

	colorMod = new SDL_Color({ Uint8(rand() % 255), 0 /*Uint8(rand() % 255)*/, Uint8(rand() % 255) });
	if (Level::getID() < 3)
	{
		if (colorMod->r > colorMod->b)
			colorMod->r /= rand() % 8 + 1;
	}
	power += colorMod->r / 2;
	power -= colorMod->b / 2;
	if (power < 0)
		power = 1;
	moveSpeed = power / 2;
	if (moveSpeed > Game::MOVE_SPEED * 4)
		moveSpeed = Game::MOVE_SPEED * 4;
	if (moveSpeed == 0)
		moveSpeed++;
	bounds = { pbox.x - (Game::UNIT_W * 4), pbox.y - (Game::UNIT_H * 4), pbox.w + (Game::UNIT_W * 4), pbox.h + (Game::UNIT_H * 4) };
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
			if (eyeIndex >= Game::enemies.size() || Game::enemies[eyeIndex] == NULL || Game::enemies[eyeIndex]->getSubtype() != EYE)
			{
				bool found = false;
				for (int i = 0; i < Game::enemies.size(); i++)
				{
					if (Game::enemies[i] != NULL)
					{
						if (Game::enemies[i]->getSubtype() == EYE)
						{
							eyeIndex = i;
							found = true;
						}
					}
				}
				if (!found)
				{
					SDL_Rect* r;
					for (int i = 0; i < 128; i++)
					{
						r = new SDL_Rect;
						r->x = rect.x + (rand() % rect.w);
						r->y = rect.y + (rand() % rect.h);
						r->w = rand() % 4 + 1;
						r->h = r->w;
						Graphics::spawnParticle(SNOW_PART, r);
						delete r;
						r = NULL;
					}
					Game::gScore += 100;
					Game::Mode = GAME_END;
					Game::enemySpawnChance = 0;
					Audio::play(EXPLODE, 's');
					eye = false;
					destroy = true;
					break;
				}
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
	int MAX_MOVESPEED = (2 * power > 8 ? 8 : 2 * power);
	int MAX_AERIALSPEED = (2 * power > 8 ? 8 : 2 * power);
	static int skip = 0;
	int targetSkip = (2 * power > 4 ? 4 : 2 * power);
	bool checked = false;
	switch (subtype)
	{
	case ICE:
		if (collisions[DOWN] == NULL && aerialSpeed == 0)
			aerialSpeed = Game::GRAVITY_START;
		if (moveSpeed != 0)
			rect.x += moveSpeed;
		if (aerialSpeed != 0)
			rect.y += aerialSpeed;
		cycleAerials();
		break;
	case TRIANGLE:
		//SDL_Rect check = { 0, 0, 0, 0 };
		if (moveSpeed == 0)
			moveSpeed = Game::MOVE_SPEED / 2;
		if (moveSpeed != 0)
			rect.x += moveSpeed;

		if (aerialSpeed == 0)
		{
			rect.h--;
			checked = true;
		}
		if (Game::checkCollision(this))
		{
			if (collisions[DOWN] != NULL && ((collisions[LEFT] != NULL && moveSpeed < 0) || (collisions[RIGHT] != NULL && moveSpeed > 0)))
				aerialSpeed = Game::JUMP_START;
			manageCollisionsX();
			//emptyCollidersX();
		}
		if(checked)
			rect.h++;
		if (aerialSpeed != 0)
			rect.y += aerialSpeed;
		cycleAerials();
		if (Game::checkCollision(this))
		{
			manageCollisionsY();
			//emptyCollidersY();
		}
		//check = { rect.x + (moveSpeed > 0 ? rect.w + Game::UNIT_W : (moveSpeed < 0 ? -Game::UNIT_W : 0)), rect.y, rect.w, rect.h };
		//for (int i = 0; i < Game::collisionEntities.size(); i++)
		//	if (aerialSpeed == 0 && Game::checkCollision(check, Game::collisionEntities[i]->rect))
		//		aerialSpeed = Game::JUMP_START;
		break;
	case CIRCLE:
		if (moveSpeed == 0)
			moveSpeed++;
		if (aerialSpeed == 0)
			aerialSpeed++;
		if (moveSpeed <= 1 && moveSpeed >= -1)
		{
			// moveSpeed = Game::MOVE_SPEED * (moveSpeed == 0 ? 1 : -moveSpeed);
			moveSpeed = -moveSpeed * (rand() % 8);
			targetMoveSpeed = (moveSpeed < 0 ? -1 : 1) * MAX_MOVESPEED;
		}
		else if (abs(moveSpeed) < abs(targetMoveSpeed))
		{
			if(skip == targetSkip)
				moveSpeed *= 2;
			if ((rect.x < bounds.x && moveSpeed < 0) || (rect.x + rect.w > bounds.x + bounds.w && moveSpeed > 0))
			{
				moveSpeed = (moveSpeed < 0 ? 2 : -2);
				targetMoveSpeed = -targetMoveSpeed * 4;
			}
		}
		else
		{
			if (skip == targetSkip)
			{
				targetMoveSpeed = 0;
				moveSpeed /= 2;
			}
		}
		rect.x += moveSpeed;
		if (aerialSpeed <= 1 && aerialSpeed >= -1)
		{
			// aerialSpeed = Game::GRAVITY_START * 2 * (aerialSpeed == 0 ? 1 : -aerialSpeed);
			aerialSpeed = -aerialSpeed * (rand() % 8);
			targetAerialSpeed = (aerialSpeed < 0 ? -1 : 1) * MAX_MOVESPEED;
		}
		else if (abs(aerialSpeed) < abs(targetAerialSpeed))
		{
			if (skip == targetSkip)
				aerialSpeed *= 2;
			if ((rect.y < bounds.y && aerialSpeed < 0) || (rect.y + rect.h > bounds.y + bounds.h && aerialSpeed > 0))
			{
				aerialSpeed = (aerialSpeed < 0 ? 2 : -2);
				targetAerialSpeed = -targetAerialSpeed * 4;
			}
		}
		else
		{
			if (skip == targetSkip)
			{
				targetAerialSpeed = 0;
				aerialSpeed /= 2;
			}
		}
		rect.y += aerialSpeed;
		if (skip == targetSkip)
			skip = -1;
		skip++;
		break;
	case SQUARE:
		rect.x += moveSpeed;

		if (aerialSpeed == 0)
		{
			rect.h--;
			checked = true;
		}
		if (Game::checkCollision(this))
		{
			if (collisions[DOWN] != NULL && ((collisions[LEFT] != NULL && moveSpeed < 0) || (collisions[RIGHT] != NULL && moveSpeed > 0)))
			{
				moveSpeed = -moveSpeed;
				rect.x += 2 * (moveSpeed > 0 ? 1 : -1);
			}
			manageCollisionsX();
			//emptyCollidersX();
		}
		if(checked)
			rect.h++;
		if (aerialSpeed != 0)
			rect.y += aerialSpeed;
		cycleAerials();
		if (Game::checkCollision(this))
		{
			manageCollisionsY();
			//emptyCollidersY();
		}
		break;
	case HALFC:
		if (rect.h != Game::UNIT_H / 2)
			rect.h = Game::UNIT_H / 2;
		rect.x += moveSpeed;

		if (aerialSpeed == 0)
		{
			rect.h--;
			checked = true;
		}
		if (Game::checkCollision(this))
		{
			if (skip++ > 70 && collisions[DOWN] != NULL)
			{
				aerialSpeed = Game::JUMP_MAX * 2;
				skip = 0;
			}
			if (collisions[DOWN] != NULL && ((collisions[LEFT] != NULL && moveSpeed < 0) || (collisions[RIGHT] != NULL && moveSpeed > 0)))
			{
				moveSpeed = -moveSpeed;
				rect.x += 2 * (moveSpeed > 0 ? 1 : -1);
			}
			manageCollisionsX();
			//emptyCollidersX();
		}
		if (checked)
			rect.h++;
		if (aerialSpeed != 0)
			rect.y += aerialSpeed;
		cycleAerials();
		if (Game::checkCollision(this))
		{
			manageCollisionsY();
			//emptyCollidersY();
		}
		break;
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

void Enemy::manageCollisionsX()
{
	// if (Game::checkCollision(this))
	// {
		if (collisions[LEFT] != NULL)
		{
			rect.y--;
			if(Game::checkCollision(rect, collisions[LEFT]->rect))
				if (collisions[LEFT]->getType() == BLOCK)
					while (Game::checkCollision(rect, collisions[LEFT]->rect))
						rect.x++;
			rect.y++;
			collisions[LEFT] = NULL;
			//if (aerialSpeed == 0)
			//	aerialSpeed = Game::JUMP_START;
		}
		if (collisions[RIGHT] != NULL)
		{
			rect.y--;
			if(Game::checkCollision(rect, collisions[RIGHT]->rect))
				if (collisions[RIGHT]->getType() == BLOCK)
					while (Game::checkCollision(rect, collisions[RIGHT]->rect))
						rect.x--;
			rect.y++;
			collisions[RIGHT] = NULL;
			//if (aerialSpeed == 0)
			//	aerialSpeed = Game::JUMP_START;
		}
	// }
		
	if (collisions[DOWN] != NULL)
	{
		rect.y++;
		if (!Game::checkCollision(rect, collisions[DOWN]->rect))
			collisions[DOWN] = NULL;
		//else
		//	aerialSpeed = 0;
		rect.y--;
	}
	else if (collisions[DOWN] == NULL && aerialSpeed == 0)
		aerialSpeed = Game::GRAVITY_START;
}

void Enemy::manageCollisionsY()
{
	// if (Game::checkCollision(this))
	// {
		if (collisions[UP] != NULL)
		{
			if (collisions[UP]->getType() == BLOCK)
			{
				// aerialSpeed = 0;
				aerialSpeed = Game::GRAVITY_START;
				while (Game::checkCollision(rect, collisions[UP]->rect))
					rect.y++;
			}
			collisions[UP] = NULL;
		}
		if (collisions[DOWN] != NULL)
		{
			if (collisions[DOWN]->getType() == BLOCK)
			{
				aerialSpeed = 0;
				while (Game::checkCollision(rect, collisions[DOWN]->rect))
					rect.y--;
			}
			// collisions[DOWN] = NULL;
		}
	// }
}
/*
void Enemy::emptyCollidersX()
{
	collisions[LEFT] = NULL;
	collisions[RIGHT] = NULL;
}

void Enemy::emptyCollidersY()
{
	collisions[UP] = NULL;
	// collisions[DOWN] = NULL;
}
*/

/*
void Enemy::doDefaultActions()
{

}
*/