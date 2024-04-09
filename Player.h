#ifndef Player_h_

#define Player_h_

#include"CommonFunc.h"
#include"LTexture.h"
#include"Tile.h"
#include"Entity.h"

class Player
{
public:
	Player(int i, int j);
	~Player();

	void FreePlayer();
	void handleEvent(SDL_Event& e, Tile**** tiles);
	void render(SDL_Renderer* screen, SDL_FRect& camera, float scale = 1);
	void renderHp(SDL_Renderer* screen, SDL_FRect& camera, float scale = 1);
	void setCamera(SDL_FRect& camera, float scale);
	void attack(Entity* target);

	void setClip(int x, int y, int w, int h);
	SDL_Rect getClip();

	int getPosI();
	int getPosJ();
	void setPosI(int i);
	void setPosJ(int j);
	void setGreenHpClip(int x, int y, int w, int h);
	void setRedHpClip(int x, int y, int w, int h);

	bool loadPlayerTexture(std::string path, SDL_Renderer* screen);
	bool loadHpTexture(std::string path1, std::string path2, SDL_Renderer* screen);

private:
	LTexture player;
	LTexture greenHp;
	SDL_Rect greenHpClip;
	LTexture redHp;
	SDL_Rect redHpClip;

	int mPosI;
	int mPosJ;

	SDL_Rect clip;

	int maxHP;
	int currentHp;

	int dame;
	
};

#endif