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

Player::Player(SDL_Rect box) : PhysicsEntity(box, PLAYER, -1)
{
}

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

bool Player::handleInput(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.sym == Game::Controls["Move Left"])
			moveSpeed = -Game::MOVE_SPEED;
		else if (e->key.keysym.sym == Game::Controls["Move Right"])
			moveSpeed = Game::MOVE_SPEED;
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