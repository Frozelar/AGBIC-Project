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