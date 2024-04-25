#include"Text.h"

Text::Text()
{

}

Text::~Text()
{
	free();
}

bool Text::loadText(std::string text, TTF_Font* font, SDL_Renderer* screen, int wrappingSize)
{
	bool success = true;
	this->mText = text;
	TTF_SetFontSize(font, this->size);
	if (textTexture.loadFromRenderedText(mText, mColor, font, screen, wrappingSize) == false)
	{
		success = false;
		std::cout << "ko load dc text" << std::endl;
	}
	return success;
}

void Text::render(SDL_Renderer* screen)
{
	textTexture.render(this->x, this->y, screen);
}

void Text::initText(int x, int y, int size, SDL_Color color)
{
	this->x = x;
	this->y = y;
	this->size = size;
	this->mColor = color;
}

void Text::free()
{
	textTexture.free();
}