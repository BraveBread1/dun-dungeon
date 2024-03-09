#include"BaseObject.h"

BaseObject::BaseObject()
{
	mObject = NULL;
	rect.x = 0;
	rect.y = 0;
	rect.w = 0;
	rect.h = 0;
}

BaseObject::~BaseObject()
{
	free();
}

bool BaseObject::loadImg(std::string path, SDL_Renderer* screen)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface != NULL)
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, COLOR_KEY_R, COLOR_KEY_G, CORLOR_KEY_B));
		newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface);
		if (newTexture != NULL)
		{
			rect.w = loadedSurface->w;
			rect.h = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	mObject = newTexture;

	return mObject != NULL;
}

void BaseObject::render(SDL_Renderer* des, const SDL_Rect* clip /* = NULL */ )
{
	SDL_Rect renderQuad = { rect.x, rect.y, rect.w, rect.h };
	SDL_RenderCopy(des, mObject, clip, &renderQuad);
}

void BaseObject::free()
{
	if (mObject != NULL)
	{
		SDL_DestroyTexture(mObject);
		mObject = NULL;
		rect.w = 0;
		rect.h = 0;
	}
}