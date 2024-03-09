#ifndef BaseObject_h_
#define BaseObject_h_

#include"CommonFunc.h"

class BaseObject
{
public:
	BaseObject();
	~BaseObject();
	void setRect(int x, int y) { rect.x = x; rect.y = y; }
	SDL_Rect getRect() { return rect; }
	SDL_Texture* getObject() const { return mObject; }

	bool loadImg(std::string path, SDL_Renderer* screen);
	void render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void free();


protected:
	SDL_Texture* mObject = NULL;
	SDL_Rect rect;
};

#endif