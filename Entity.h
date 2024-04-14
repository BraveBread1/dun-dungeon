#ifndef ENTITY_H_

#define ENTITY_H_

#include"CommonFunc.h"
#include"LTexture.h"
//#include"Player.h"

class Entity
{
public:
	enum Dest
	{
		NONE, UP, DOWN, LEFT, RIGHT
	};

	Entity(int i, int j, int type);
	~Entity();

	void freeEntity();
	void render(SDL_FRect& camera, SDL_Renderer* screen, LTexture& entTexture, LTexture& greenHpTexture, LTexture& redHpTexture, Uint32 time, float scale);
	int getType();
	SDL_FRect getBox();
	void setEntClip(int x = 0, int y = 0);
	Entity* next;
	friend void addLast(Entity*& a, Entity* tmp);
	void attacked(int dame);

	void setGreenHpClip(int x, int y, int w, int h);
	void setRedHpClip(int x, int y, int w, int h);
	void renderHp(SDL_Renderer* screen, LTexture& greenHpTexture, LTexture& redHpTexture, SDL_FRect& camera, float scale = 1);
	void move();
	bool nextToPlayer(int i, int j);

	//void attack(Player* player);
	void setPath(Dest go);
	void setBox();

	int getPosI();
	int getPosJ();
	int getDame();

	bool getDead();

private:
	int i, j;
	SDL_FRect mBox;

	SDL_Rect entClip;
	SDL_Rect greenHpClip;
	SDL_Rect redHpClip;

	int mType;
	
	int maxHP;
	int currentHp;

	int dame;
	bool dead;

	Dest goDest;
	
};



#endif