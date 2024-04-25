#ifndef Text_h
#define Text_h

#include"CommonFunc.h"
#include"LTexture.h"

struct Text
{
	Text();
	~Text();
	LTexture textTexture;
	std::string mText;
	int x, y;
	int size;
	SDL_Color mColor;

	void free();
	void initText(int x, int y, int size, SDL_Color color);
	bool loadText(std::string text, TTF_Font* font, SDL_Renderer* screen, int wrappingSize);
	void render(SDL_Renderer* screen);
};

#endif