#include"Object.h"

Object::Object(int i, int j, int type)
{
	this->i = i;
	this->j = j;
	mType = type;

	mBox.x = j * TILE_SIZE;
	mBox.y = i * TILE_SIZE;
	mBox.w = TILE_SIZE;
	mBox.h = TILE_SIZE;
}

void Object::render(SDL_FRect& camera, LTexture& objTexture, SDL_Rect clip, SDL_Renderer* screen, float scale)
{
	if (checkCollision(camera, mBox))
	{
		objTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &clip, scale);
	}
}

int Object::getType()
{
	return mType;
}

SDL_FRect Object::getBox()
{
	return mBox;
}