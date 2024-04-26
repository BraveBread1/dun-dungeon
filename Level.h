#ifndef Level_h_

#define Level_h_

#include"CommonFunc.h"
#include"Player.h"
#include"Map.h"
#include"ObjectLayer.h"
#include"Entity.h"
#include"EntityLayer.h"
#include"LTimer.h"
#include"FogOfWar.h"
#include"Astar.h"
#include"Menu.h"
#include"SoundEffect.h"
#include"Hub.h"

enum LevelState
{
	PLAYER_ACTION, MONSTER_ACTION
};

enum PlayerState
{
	PLAYER_NO_ATTACK, PLAYER_ATTACK, PLAYER_MOVE
};

enum MonsterState
{
	MONS_NO_ATTACK, MONS_ATTACK
};

class Level
{
public:
	Level(int LEVEL_WIDTH, int LEVEL_HEIGHT, int LEVEL_ROWS, int LEVEL_COLS);
	~Level();

	bool levelInit(SDL_Window* window, SDL_Renderer* screen, TTF_Font* font, Player* player, int lv);
	void close();
	Map* createMap(int lv);
	ObjectLayer* creatObjectLayer(int lv);
	void run(SDL_Event *e, int &play);
	void render();
	void handleEvent(SDL_Event e);
	void clearSolidArr();
	void updateHasSolid();
	int hasLOS(int x2, int y2);
	void doPlayer(SDL_Event& e);
	void doEntity();
	void handleObjectEvent();
	void entThink(Entity* currentEnt);
	void pathFinding(Entity* monster, int *di, int *dj, int pi, int pj);

	void moveToPlayer(Entity* currentEnt);
	void patrol(Entity* currentEnt);
	void moveToLastSaw(Entity* currentEnt);

	void handleAttackingEvent(SDL_Event e);

	bool getWin();



private:
	bool** hasSolid;
	bool isPlaying;
	bool win;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Player* mPlayer;
	SDL_FRect camera;
	Map* mMap;
	ObjectLayer* mObjectLayer;
	float scale;
	EntityLayer* mEntLayer;
	LTimer timer;
	FogOfWar* fogOfWar;
	/*Hub* mHub;*/

	int mLevelSate;
	int playerState;
	int monsState;
	

	Uint32 monsAttackStart;
	Uint32 playerAttackStart;
	Uint32 playerMoveStart;
	SoundEffect mons_melee_attack;
	SoundEffect player_melee_attack;
	SoundEffect player_moving;

	TTF_Font* mFont;
	TTF_Font* hubFont;
	int currentLv;

	int LEVEL_WIDTH;
	int LEVEL_HEIGHT;
	int LEVEL_ROWS;
	int LEVEL_COLS;
};

#endif