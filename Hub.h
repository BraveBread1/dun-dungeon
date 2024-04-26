#ifndef Hub_h
#define Hub_h

#include"CommonFunc.h"
#include"Text.h"

struct Hub
{
	Hub();
	~Hub();

	void free();
	void render(SDL_Renderer* screen);
	void addText(std::string, SDL_Renderer* screen, TTF_Font* font);
	Text* texts;
	int maxTexts;
	int currentText;
	SDL_Color color;

};


#endif // !Hub_h
