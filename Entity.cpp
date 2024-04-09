#include"Entity.h"

Entity::Entity(int i, int j, int type)
{
	this->i = i;
	this->j = j;
	mType = type;

	entClip.x = 0;
	entClip.y = 0;
	entClip.w = TILE_SIZE;
	entClip.h = TILE_SIZE;

	mBox.x = j * TILE_SIZE;
	mBox.y = i * TILE_SIZE;
	mBox.w = TILE_SIZE;
	mBox.h = TILE_SIZE;

	greenHpClip.x = 0;
	greenHpClip.y = 0;
	greenHpClip.w = 16;
	greenHpClip.h = 2;

	redHpClip.x = 0;
	redHpClip.y = 0;
	redHpClip.w = 16;
	redHpClip.h = 2;

	next = NULL;
}

void Entity::freeEntity()
{
	if (next != NULL)
	{
		next = NULL;
	}
}

void Entity::render(SDL_FRect& camera, SDL_Renderer* screen, LTexture& entTexture, LTexture& greenHpTexture, LTexture& redHpTexture, float scale)
{
	if (checkCollision(camera, mBox))
	{
		entTexture.render(mBox.x - camera.x, mBox.y - camera.y - 6, screen, &entClip, scale);
		renderHp(screen, greenHpTexture, redHpTexture, camera, scale);
	}
}

Entity::~Entity()
{
	freeEntity();
}

int Entity::getType()
{
	return mType;
}

SDL_FRect Entity::getBox()
{
	return mBox;
}

void Entity::setEntClip(int x, int y)
{
	entClip.x = x;
	entClip.y = y;
	entClip.w = 16;
	entClip.h = 16;
}

int Entity::getPosI()
{
	return i;
}

int Entity::getPosJ()
{
	return j;
}

void Entity::attacked(int dame)
{
	currentHp -= dame;
}

void Entity::setGreenHpClip(int x, int y, int w, int h)
{
	greenHpClip.x = x;
	greenHpClip.y = y;
	greenHpClip.w = w;
	greenHpClip.h = h;
}

void Entity::setRedHpClip(int x, int y, int w, int h)
{
	redHpClip.x = x;
	redHpClip.y = y;
	redHpClip.w = w;
	redHpClip.h = h;
}

void Entity::renderHp(SDL_Renderer* screen, LTexture& greenHpTexture, LTexture& redHpTexture, SDL_FRect& camera, float scale)
{
	redHpTexture.render(j * TILE_SIZE - camera.x, i * TILE_SIZE - camera.y - 8, screen, &redHpClip, scale);
	greenHpTexture.render(j * TILE_SIZE - camera.x, i * TILE_SIZE - camera.y - 8, screen, &greenHpClip, scale);
}