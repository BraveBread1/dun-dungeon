#include"Hub.h"

Hub::Hub()
{
	maxTexts = 5;
	currentText = 0;
	color = { 255, 255, 255 };
	texts = new Text[maxTexts];
}

Hub::~Hub()
{
}

void Hub::addText(std::string text, SDL_Renderer* screen, TTF_Font* font)
{
	Text tmp;
	tmp.initText(100, 100, 50, color);
	if (tmp.loadText(text, font, screen, 300) == false)
	{
		std::cout << "unable to load text in hub" << std::endl;
	}
	if (currentText < maxTexts)
	{
		texts[currentText - 1] = tmp;
		++currentText;
		for (int i = 0; i < currentText; ++i)
		{
			texts[i].setPos(0, 612 - (currentText - i) * 12);
			std::cout << texts[i].getText() << std::endl;
		}
	}
	else
	{
		for (int i = 0; i < maxTexts - 1; ++i)
		{
			texts[i] = texts[i + 1];
			texts[i].setPos(0, 612 - (currentText - i) * 12);
		}
		texts[maxTexts - 1] = tmp;
		++currentText;
	}
	tmp.free();
}

void Hub::render(SDL_Renderer* screen)
{
	for (int i = 0; i < currentText; ++i)
	{
		texts[i].render(screen);
	}
}