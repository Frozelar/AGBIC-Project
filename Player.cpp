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

// 
Player::Player(SDL_Rect box) : PhysicsEntity(box, PLAYER, -1)
{
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
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.sym == Game::Controls["Move Left"])
		{
			if (moveSpeed >= 0)
				moveSpeed = -Game::MOVE_SPEED;
			else if (moveSpeed == -Game::MOVE_SPEED)
				moveSpeed = -Game::MOVE_SPEED * 2;
		}
		else if (e->key.keysym.sym == Game::Controls["Move Right"])
		{
			if (moveSpeed <= 0)
				moveSpeed = Game::MOVE_SPEED;
			else if (moveSpeed == Game::MOVE_SPEED)
				moveSpeed = Game::MOVE_SPEED * 2;
		}
		else if (e->key.keysym.sym == Game::Controls["Jump"])
		{
			if (aerialSpeed == 0)
				aerialSpeed = Game::JUMP_START;
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
			if (aerialSpeed < 0)
				aerialSpeed *= Game::JUMP_MULT / 4;
		}
		else
			return false;

	}
	return true;
}

// move player horizontally/vertically, manage collisions
void Player::handleMovements()
{
	if (moveSpeed != 0)
		rect.x += moveSpeed;
	if (Game::checkCollision(this))
	{
		if (collisions[LEFT] != NULL)
		{
			if (collisions[LEFT]->getType() != COLLECTIBLE)
			{
				while (Game::checkCollision(rect, collisions[LEFT]->rect))
					rect.x++;
			}
			else
			{
				collisions[LEFT]->destroy = true;
			}
			collisions[LEFT] = NULL;
		}
		if (collisions[RIGHT] != NULL)
		{
			if (collisions[RIGHT]->getType() != COLLECTIBLE)
			{
				while (Game::checkCollision(rect, collisions[RIGHT]->rect))
					rect.x--;
			}
			else
			{
				collisions[RIGHT]->destroy = true;
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
			if (collisions[UP]->getType() != COLLECTIBLE)
			{
				aerialSpeed = 0;
				while (Game::checkCollision(rect, collisions[UP]->rect))
					rect.y++;
			}
			else
			{
				collisions[UP]->destroy = true;
			}
			collisions[UP] = NULL;
		}
		if (collisions[DOWN] != NULL)
		{
			if (collisions[DOWN]->getType() != COLLECTIBLE)
			{
				aerialSpeed = 0;
				while (Game::checkCollision(rect, collisions[DOWN]->rect))
					rect.y--;
			}
			else
			{
				collisions[DOWN]->destroy = true;
			}
			// collisions[DOWN] = NULL;
		}
	}
}