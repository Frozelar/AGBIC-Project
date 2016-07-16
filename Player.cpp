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

#include "Player.h"
#include "Game.h"
#include "Audio.h"
#include "Level.h"

// initialize members
Player::Player(SDL_Rect box) : PhysicsEntity(box, PLAYER, -1)
{
	abilities["Sprint"] = false;
	abilities["High Jump"] = false;
	abilities["Double Jump"] = false;
	abilities["Key"] = false;
	jumps = 0;
}

// 
Player::~Player()
{
}

/*
void Player::handleMovements()
{

}

void Player::handleAerials()
{

}
*/

// process any inputs in given event
bool Player::handleInput(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == NULL)
	{
		if (e->key.keysym.sym == Game::Controls["Move Left"])
		{
			moveSpeed = -Game::MOVE_SPEED;
		}
		else if (e->key.keysym.sym == Game::Controls["Move Right"])
		{
			moveSpeed = Game::MOVE_SPEED;
		}
		else if (e->key.keysym.sym == Game::Controls["Jump"])
		{
			if (aerialSpeed == 0 || (abilities["Double Jump"] && jumps == 1))
			{
				aerialSpeed = Game::JUMP_START;
			}
		}
		else
			return false;
	}
	else if (e->type == SDL_KEYUP)
	{
		if (e->key.keysym.sym == Game::Controls["Move Left"] && moveSpeed < 0)
			moveSpeed = 0;
		else if (e->key.keysym.sym == Game::Controls["Move Right"] && moveSpeed > 0)
			moveSpeed = 0;
		else if (e->key.keysym.sym == Game::Controls["Jump"])
		{
			if(aerialSpeed < 0)
				aerialSpeed *= Game::JUMP_MULT / 4;
		}
		else if (e->key.keysym.sym == Game::Controls["Pause"])
		{
			Game::Mode = PAUSE;
		}
		else
			return false;
	}
	return true;
}

// move player horizontally/vertically, manage collisions
void Player::handleMovements()
{
	static bool gotEnd = false;
	static int plFrameCount = 0;

	if (abilities["Sprint"] && plFrameCount >= Game::WARMUP_DURATION && abs(moveSpeed) == Game::MOVE_SPEED)
		moveSpeed = (moveSpeed > 0 ? 1 : -1) * Game::MOVE_SPEED * 2;
	else if (abilities["Sprint"] && moveSpeed == 0 && plFrameCount != 0)
		plFrameCount = 0;
	if (moveSpeed != 0)
	{
		plFrameCount++;
		rect.x += moveSpeed;
	}
	if (Game::checkCollision(this))
	{
		if (collisions[LEFT] != NULL)
		{
			if (collisions[LEFT]->getType() == BLOCK)
			{
				while (Game::checkCollision(rect, collisions[LEFT]->rect))
					rect.x++;
			}
			else if (collisions[LEFT]->getType() == COLLECTIBLE)
			{
				collisions[LEFT]->destroy = true;
			}
			else if (collisions[LEFT]->getType() == ENEMY)
			{
				destroy = true;
			}
			collisions[LEFT] = NULL;
		}
		if (collisions[RIGHT] != NULL)
		{
			if (collisions[RIGHT]->getType() == BLOCK)
			{
				while (Game::checkCollision(rect, collisions[RIGHT]->rect))
					rect.x--;
			}
			else if (collisions[RIGHT]->getType() == COLLECTIBLE)
			{
				collisions[RIGHT]->destroy = true;
			}
			else if (collisions[RIGHT]->getType() == ENEMY)
			{
				destroy = true;
			}
			collisions[RIGHT] = NULL;
		}
	}
	if (aerialSpeed != 0)
		rect.y += aerialSpeed;
	cycleAerials();
	if (collisions[DOWN] != NULL)
	{
		rect.y++;
		if (!Game::checkCollision(rect, collisions[DOWN]->rect))
			collisions[DOWN] = NULL;
		rect.y--;
	}
	else if (collisions[DOWN] == NULL && aerialSpeed == 0)
		aerialSpeed = Game::GRAVITY_START;
	if (Game::checkCollision(this))
	{
		if (collisions[UP] != NULL)
		{
			if (collisions[UP]->getType() == BLOCK)
			{
				// aerialSpeed = 0;
				aerialSpeed = Game::GRAVITY_START;
				while (Game::checkCollision(rect, collisions[UP]->rect))
					rect.y++;
			}
			else if (collisions[UP]->getType() == COLLECTIBLE)
			{
				collisions[UP]->destroy = true;
			}
			else if (collisions[UP]->getType() == ENEMY)
			{
				destroy = true;
			}
			collisions[UP] = NULL;
		}
		if (collisions[DOWN] != NULL)
		{
			if (collisions[DOWN]->getType() == BLOCK)
			{
				if (collisions[DOWN]->getSubtype() == GOAL && abilities["Key"])
				{
					if(!gotEnd)
						Game::score += 100;
					gotEnd = true;
					Game::Mode = LEVEL_END;
				}

				aerialSpeed = 0;
				while (Game::checkCollision(rect, collisions[DOWN]->rect))
					rect.y--;
			}
			else if(collisions[DOWN]->getType() == COLLECTIBLE)
			{
				collisions[DOWN]->destroy = true;
			}
			else if (collisions[DOWN]->getType() == ENEMY)
			{
				destroy = true;
			}
			// collisions[DOWN] = NULL;
		}
	}
	while (rect.x < 0)
	{
		rect.x++;
		moveSpeed = 0;
	}
	while (rect.x + rect.w >= Level::getw('p'))
	{
		rect.x--;
		moveSpeed = 0;
	}
}

// manage aerialSpeed
void Player::cycleAerials()
{
	if (aerialSpeed == Game::JUMP_START)
	{
		Audio::play(JUMP, 's');
		jumps++;
	}
	if (aerialSpeed < 0)
	{
		aerialSpeed *= (Game::JUMP_MULT) + (!abilities["High Jump"] ? 0 : 0.02);
		if (aerialSpeed >= (Game::JUMP_MAX + (abilities["High Jump"] ? 0.1 : 0)))
			aerialSpeed = Game::GRAVITY_START;
	}
	else if (aerialSpeed > 0)
	{
		aerialSpeed *= Game::GRAVITY_MULT;
		if (aerialSpeed > Game::GRAVITY_MAX)
			aerialSpeed = Game::GRAVITY_MAX;
	}
	else if (aerialSpeed == 0)
		jumps = 0;

	if (aerialSpeed != 0 && jumps == 0)
		jumps++;
}