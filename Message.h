#pragma once
#include "Include.h"

class Message
{
public:
	Message(std::string, int, int, SDL_Color, Direction, Direction, bool);
	~Message();

	void render(void);
	void setText(std::string);

	std::string msg;
	Texture* text;
	int displayTime;
	int scroll;
	int side;
	SDL_Color textColor;
	int fontSize;
	TTF_Font* font;
	bool showBG;
};

