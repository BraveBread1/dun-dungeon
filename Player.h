#ifndef Player_h_

#define Player_h_

#include"CommonFunc.h"
#include"LTexture.h"
#include"Tile.h"

class Player
{
public:
	Player(int i, int j);
	~Player();

	void FreePlayer();
	void handleEvent(SDL_Event& e, Tile**** tiles);
	void render(SDL_Renderer* screen, SDL_FRect& camera, float scale = 1);
	void setCamera(SDL_FRect& camera, float scale);

	void setClip(int x, int y, int w, int h);
	SDL_Rect getClip();

	int getPosI();
	int getPosJ();
	bool loadPlayerTexture(std::string path, SDL_Renderer* screen);

private:
	LTexture player;
	int mPosI;
	int mPosJ;

	SDL_Rect clip;
	
};

#endif