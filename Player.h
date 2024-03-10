#ifndef Player_h_

#define Player_h_

#include"CommonFunc.h"
#include"LTexture.h"

class Player : public LTexture
{
public:
	Player();
	~Player();

	void move();
	void FreePlayer();
	void handleEvent(SDL_Event& e);
	void render(SDL_Renderer* screen, int camX, int camY);

	void setClip(int x, int y, int w, int h);
	SDL_Rect getClip();

	int getPosX();
	int getPosY();

private:
	int mPosX;
	int mPosY;

	SDL_Rect clip;

	int mVelX, mVelY;
};

#endif