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

// set properties
Texture::Texture(int px, int py, int pw, int ph)
{
	txRect = { px, py, pw, ph };
}

// free everything
Texture::~Texture()
{
	txFree();
}

// set colors to the texture
void Texture::txSetColor(Uint8 r, Uint8 g, Uint8 b /*, Uint8 a */)
{
	SDL_SetTextureColorMod(txTexture, r, g, b);
	// SDL_SetTextureAlphaMod(txTexture, a);
}
void Texture::txSetColor(SDL_Color c)
{
	SDL_SetTextureColorMod(txTexture, c.r, c.g, c.b);
}

// set the blend mode
void Texture::txSetBlendMode(SDL_BlendMode b)
{
	SDL_SetTextureBlendMode(txTexture, b);
}

// set alpha to the texture
void Texture::txSetAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(txTexture, a);
}

// load a texture from the given file path
bool Texture::txLoadF(std::string path)
{
	bool success = true;

	txFree();

	SDL_Texture* createdTexture = NULL;
	SDL_Surface* imageSurface = IMG_Load(path.c_str());

	if (imageSurface == NULL)
	{
		std::cout << "Error loading image at the specified path: " << path << std::endl;
		std::cout << "IMG_GetError: " << IMG_GetError() << std::endl;
		success = false;
	}
	else
	{
		// SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 0, 255, 255));
		createdTexture = SDL_CreateTextureFromSurface(Window::renderer, imageSurface);

		if (createdTexture == NULL)
		{
			std::cout << "Error creating texture from surface.  SDL_GetError: " << SDL_GetError() << std::endl;
		}
		else
		{
			if (txRect.w == NULL || txRect.h == NULL)
			{
				txRect.w = imageSurface->w;
				txRect.h = imageSurface->h;
			}
		}
	}

	SDL_FreeSurface(imageSurface);
	txTexture = createdTexture;

	return success;
}

// load a texture from given text, font, and color
bool Texture::txLoadT(std::string text, TTF_Font* font, SDL_Color color)
{
	txFree();
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 1024);
	if (textSurface != NULL)
	{
		txTexture = SDL_CreateTextureFromSurface(Window::renderer, textSurface);
		if (txTexture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			if (txRect.w == NULL || txRect.h == NULL)
			{
				txRect.w = textSurface->w;
				txRect.h = textSurface->h;
			}
		}
		SDL_FreeSurface(textSurface);
	}
	else
	{
		std::cout << "Unable to render text surface! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	return txTexture != NULL;
}

// render the texture
// SDL_Rect* = clip, SDL_Point* = center of rotation, double = rotation amount, SDL_RendererFlip = flip type
void Texture::txRender(SDL_Rect* clip, SDL_Point* center, double rotation, SDL_RendererFlip flip)
{
	SDL_Rect renderRect{ txRect.x, txRect.y, txRect.w, txRect.h };
	if (clip != NULL)
	{
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}

	SDL_RenderCopyEx(Window::renderer, txTexture, clip, &renderRect, rotation, center, flip);
}

// destroy the texture
void Texture::txFree()
{
	if (txTexture != NULL)
	{
		SDL_DestroyTexture(txTexture);
	}
}