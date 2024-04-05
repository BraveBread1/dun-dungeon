#ifndef ENTITY_H_

#define ENTITY_H_

#include"CommonFunc.h"
#include"LTexture.h"

class Entity
{
public:
	Entity();
	~Entity();

	void render(SDL_FRect& camera, LTexture& objTexture, SDL_Rect clip, SDL_Renderer* screen, float scale);
	int getType();
	SDL_FRect getBox();


private:
	int i, j;
	SDL_FRect mBox;

	int mType;
	
};

#endif