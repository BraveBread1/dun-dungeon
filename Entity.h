#ifndef ENTITY_H_

#define ENTITY_H_

#include"CommonFunc.h"
#include"LTexture.h"

class Entity
{
public:
	enum Dest
	{
		NONE, UP, DOWN, LEFT, RIGHT
	};

	enum Status
	{
		M_NO_ATTACK, M_ATTACK
	};

	Entity(int i, int j, int type);
	~Entity();

	void freeEntity();
	void render(SDL_FRect& camera, SDL_Renderer* screen, LTexture& entTexture, LTexture& greenHpTexture, LTexture& redHpTexture, LTexture& boss_hp, Uint32 time, float scale);
	int getType();
	SDL_FRect getBox();
	void setEntClip(int x = 0, int y = 0);
	Entity* next;
	friend void addLast(Entity*& a, Entity* tmp);
	void attacked(int dame);

	void setGreenHpClip(int x, int y, int w, int h);
	void setRedHpClip(int x, int y, int w, int h);
	void renderHp(SDL_Renderer* screen, LTexture& greenHpTexture, LTexture& redHpTexture, LTexture& boss_hp, SDL_FRect& camera, float scale = 1);
	void move(int di, int dj, bool ** hasSolid);
	bool nextToPlayer(int pi, int pj);

	//void attack(Player* player);
	void setPath(Dest go);
	void setBox();

	int getPosI();
	int getPosJ();
	int getDame();
	bool getAlert();
	int getHunt();
	void setHunt(int h);

	int getSawI();
	int getSawJ();
	void setSaw(int i, int j);

	bool gotLastSaw();

	bool getDead();
	void lookForPlayer(int pi, int pj, bool ** hasSolid);
	void setPatrolDest(int j, int i);
	SDL_Point getPatrolDest();

	bool isBlocked(int j, int i, bool** hasSolid, int pj, int pi, Entity* head);

	void setStatus(Status status);
	int getExp();

private:
	int i, j;
	SDL_FRect mBox;

	SDL_Rect entClip;
	SDL_Rect greenHpClip;
	SDL_Rect redHpClip;

	int mType;
	
	int maxHP;
	int currentHp;
	int exp;

	int minDame;
	int maxDame;

	bool dead;
	bool alert;
	int hunt;

	int sawI;
	int sawJ;

	Dest goDest;
	SDL_Point patrolDest;

	Status monsStatus;
	
	
};



#endif