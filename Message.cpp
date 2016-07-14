#include "Message.h"
#include "Graphics.h"
#include "Texture.h"
#include "Game.h"
#include "Window.h"

// initialize members
Message::Message(std::string pmsg, int psize, int ptime, SDL_Color pcolor, Direction pdir, Direction pside, bool pshow)
{
	// std::string name = Graphics::rDir + Graphics::fontName;
	font = Graphics::gFont;
	msg = pmsg;
	fontSize = psize;
	displayTime = ptime;
	textColor = pcolor;
	scroll = pdir;
	side = pside;
	showBG = pshow;
	text = new Texture(0, 0, 0, 0);
	text->txLoadT(pmsg, font, pcolor);
	if (pdir == DOWN)
		text->txRect.y = -text->txRect.h;
	else if (pdir == UP)
		text->txRect.y = Window::geth();
	if (pside == LEFT)
		text->txRect.x = 0;
	else if (pside == RIGHT)
		text->txRect.x = Window::getw() - text->txRect.w;
}

// unload text and close font
Message::~Message()
{
	if (text != NULL)
	{
		delete text;
		text = NULL;
	}
	TTF_CloseFont(font);
}

// render message and background (if applicable), and handle scrolling and displaytime
void Message::render()
{
	if (showBG)
	{
		SDL_Rect bgRect = { text->txRect.x - Graphics::GFX_OFFSET * 4, text->txRect.y - Graphics::GFX_OFFSET * 4,
			text->txRect.w + Graphics::GFX_OFFSET * 2, text->txRect.h + Graphics::GFX_OFFSET * 2 };
		Graphics::textBG->txRect = bgRect;
		Graphics::textBG->txRender();
	}
	text->txRender();

	if (scroll == DOWN)
	{
		if (((displayTime > text->txRect.h && text->txRect.y < 0) || (displayTime <= text->txRect.h && text->txRect.y + text->txRect.h >= Window::geth())) || 
			(displayTime == -1 && text->txRect.y < 0))
			text->txRect.y++;
	}
	else if (scroll == UP)
	{
		if (((displayTime > text->txRect.h && text->txRect.y + text->txRect.h > Window::geth()) || (displayTime <= text->txRect.h && text->txRect.y <= 0)) || 
			(displayTime == -1 && text->txRect.y + text->txRect.h > Window::geth()))
			text->txRect.y--;
	}

	if(displayTime != -1)
		if ((displayTime--) == text->txRect.h)
			scroll = invertDir(scroll);
}

void Message::setText(std::string ptext)
{
	text->txRect.w = text->txRect.h = 0;
	text->txLoadT(ptext, font, textColor);
	if (side == RIGHT)
		text->txRect.x = Window::getw() - text->txRect.w;
}