#pragma once
#include "Include.h"

class Texture
{
public:

	// constructor to initialize properties
	Texture(int = 2, int = 2, int = 2, int = 2);

	// deconstructor to destroy everything
	~Texture();

	// render the texture to the screen
	void txRender(SDL_Rect* clip = NULL, SDL_Point* center = NULL,
		double rotation = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// load up a texture from a file
	bool txLoadF(std::string);

	//Creates image from font string
	bool txLoadT(std::string textureText, TTF_Font* textFont, SDL_Color textColor);

	// free everything
	void txFree(void);

	// set color using red, green, blue
	void txSetColor(Uint8 r, Uint8 g, Uint8 b);

	// set transparency (aka alpha)
	void txSetAlpha(Uint8 a);

	// set blend mode
	void txSetBlendMode(SDL_BlendMode b);

	SDL_Texture* texture = NULL;
	SDL_Rect rect;
};