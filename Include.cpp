#include "Include.h"

int invertDir(int dir)
{
	switch (dir)
	{
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	default:
		return dir;
	}
}