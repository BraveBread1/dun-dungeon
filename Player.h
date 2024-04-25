#ifndef Player_h_

#define Player_h_

#include"CommonFunc.h"
#include"LTexture.h"
#include"Tile.h"
#include"Entity.h"
#include"Text.h"

struct hero_status
{
	hero_status();
	~hero_status();
	void free();
	LTexture statusPane;
	SDL_Rect statusPaneClip;
	SDL_Rect hpClip;
	SDL_Rect expClip;
	LTexture avata;
	SDL_Rect avataClip;
	Text lvText;
	Text hpText;
	Text expText;

	bool loadText(int lv, int hp, int maxHp, int exp, int maxExp, SDL_Renderer* screen, TTF_Font* font);
	void setHpClip(int w);
	void setExpClip(int w);

	void render(SDL_Renderer* screen, int hpPercnt, int expPercent);
	bool loadImg(std::string path, SDL_Renderer* screen);
	void update(int lv, int hp, int maxHp, int exp, int maxExp, SDL_Renderer* screen, TTF_Font* font);

};

class Player
{
public:
	Player(int i, int j);
	~Player();

	void FreePlayer();
	void handleEvent(SDL_Event& e, Tile**** tiles);
	void render(SDL_Renderer* screen, SDL_FRect& camera, Uint32 time, float scale = 1);
	/*void renderHp(SDL_Renderer* screen, SDL_FRect& camera, float scale = 1);*/
	void renderStatus(SDL_Renderer* screen);
	void setCamera(SDL_FRect& camera, float scale);
	void attack(Entity* target);

	void setClip(int x, int y, int w, int h);
	SDL_Rect getClip();

	int getPosI();
	int getPosJ();
	void setPosI(int i);
	void setPosJ(int j);
	/*void setGreenHpClip(int x, int y, int w, int h);
	void setRedHpClip(int x, int y, int w, int h);*/

	bool loadPlayerTexture(std::string path, SDL_Renderer* screen);
	/*bool loadHpTexture(std::string path1, std::string path2, SDL_Renderer* screen);*/
	bool loadStatusTexture(std::string path, SDL_Renderer* screen, TTF_Font* font);
	void attacked(int dame);
	void setFacing(int direct);

	void setTurn(int t);
	int getTurn();
	void setCrStatus(int s);

	void earnExp(int e);
	void update(SDL_Renderer* screen, TTF_Font* font);
	enum Facing
	{
		LEFT,
		RIGHT
	};
	enum Status
	{
		NO_ATTACK, ATTACK, MOVING
	};

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

	int minDame;
	int maxDame;

	bool moved;
	int turn;
	int exp;
	int maxExp;
	int level;

	hero_status status;

	Facing facing;
	int crStatus;
	
};

#endif