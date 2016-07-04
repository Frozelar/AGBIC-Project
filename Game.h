#pragma once
#include "Include.h"

class Game
{
public:
	Game();
	~Game();

	static bool init(void);
	static void close(void);
	static bool checkCollision(PhysicsEntity*, bool = true, int = -1);
	static bool checkCollision(SDL_Rect, SDL_Rect);
	static int findCollision(PhysicsEntity*, SDL_Rect);
	static bool newEntity(SDL_Rect, int, int);
	static void clearEntities(void);

	static SDL_Event inputEvent;
	static std::vector<StaticEntity*> allEntities;
	static std::vector<StaticEntity*> staticEntities;
	static std::vector<PhysicsEntity*> dynamicEntities;
	static std::vector<StaticEntity*> renderedEntities;
	static std::vector<StaticEntity*> collisionEntities;
	static Player* gPlayer;
	static std::vector<std::string> blockTypes;
	static const int UNIT_W;
	static const int UNIT_H;
	static const float GRAVITY_START;
	static const float GRAVITY_MULT;
	static const float GRAVITY_MAX;
	static const float JUMP_MAX;
	static const float JUMP_MULT;
	static const float JUMP_START;
	static const int MOVE_SPEED;
	static const float ROTATION_SPEED;
	static std::map<std::string, int> Controls;
	static std::vector<int> entityOffset;
};

