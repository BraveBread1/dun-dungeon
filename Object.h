#ifndef OBJECT_H_
#define OBJECT_H_

#include"CommonFunc.h"
#include"LTexture.h"

class Object
{
public:
	Object(int i, int j, int type);
	void render(SDL_FRect& camera, LTexture& objTexture, SDL_Rect clip, SDL_Renderer* screen, float scale = 1.0);
	int getType();
	void setType(int t);
	SDL_FRect getBox();

private:
	int i, j;
	SDL_FRect mBox;

	int mType;
};

#endif