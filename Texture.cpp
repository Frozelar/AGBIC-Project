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

#include "Texture.h"
#include "Window.h"

Texture::Texture(int px, int py, int pw, int ph)
{
	// set properties
	rect = { px, py, pw, ph };
}

Texture::~Texture()
{
	// free everything
	txFree();
}

void Texture::txSetColor(Uint8 r, Uint8 g, Uint8 b)
{
	// set colors to the texture
	SDL_SetTextureColorMod(texture, r, g, b);
}

void Texture::txSetBlendMode(SDL_BlendMode b)
{
	// set the blend mode
	SDL_SetTextureBlendMode(texture, b);
}

void Texture::txSetAlpha(Uint8 a)
{
	// set alpha to the texture
	SDL_SetTextureAlphaMod(texture, a);
}

bool Texture::txLoadF(std::string path)
{
	// keeps track of whether a failure occurred
	bool success = true;

	// free the current texture (if it exists)
	txFree();

	// the texture that we will (hopefully) end up with
	SDL_Texture* createdTexture = NULL;

	// the surface that the texture's image will be loaded onto
	SDL_Surface* imageSurface = IMG_Load(path.c_str());

	if (imageSurface == NULL)
	{
		// output a message if the surface failed to load
		std::cout << "Error loading image at the specified path: " << path << std::endl;
		std::cout << "IMG_GetError: " << IMG_GetError() << std::endl;
		success = false;
	}
	else
	{
		// set the color key
		// SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 0, 255, 255));

		// create the texture itself from the surface
		createdTexture = SDL_CreateTextureFromSurface(Window::renderer, imageSurface);

		if (createdTexture == NULL)
		{
			// output an error
			std::cout << "Error creating texture from surface.  SDL_GetError: " << SDL_GetError() << std::endl;
		}
		else
		{
			if (rect.w == NULL || rect.h == NULL)
			{
				rect.w = imageSurface->w;
				rect.h = imageSurface->h;
			}
		}
	}

	// the surface isn't needed anymore
	SDL_FreeSurface(imageSurface);

	// make the texture member point to the newly created texture
	texture = createdTexture;

	// return whether everything ran successfully or not
	return success;
}

bool Texture::txLoadT(std::string text, TTF_Font* font, SDL_Color color)
{
	// get rid of preexisting texture
	txFree();

	// render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 1024);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(Window::renderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			if (rect.w == NULL || rect.h == NULL)
			{
				rect.w = textSurface->w;
				rect.h = textSurface->h;
			}
		}

		// get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		std::cout << "Unable to render text surface! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	// return success
	return texture != NULL;
}

void Texture::txRender(SDL_Rect *clip, SDL_Point *center, double rotation, SDL_RendererFlip flip)
{
	// rect to render to
	SDL_Rect renderRect{ rect.x, rect.y, rect.w, rect.h };

	// if clip is given, set properties
	if (clip != NULL)
	{
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}

	// render to the screen
	SDL_RenderCopyEx(Window::renderer, texture, clip, &renderRect, rotation, center, flip);
}

void Texture::txFree()
{
	// destroy the texture
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
	}
}