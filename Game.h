#ifndef Game_h_

#define Game_h_

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

enum GameState
{
	MENU, DYING, PLAYER_ACTION, MONSTER_ACTION
};

enum PlayerState
{
	PLAYER_NO_ATTACK, PLAYER_ATTACK, PLAYER_MOVE
};

enum MonsterState
{
	MONS_NO_ATTACK, MONS_ATTACK
};

class Game
{
public:
	Game();
	~Game();

	bool gameInit();
	void close();
	Player* createPlayer(int startI, int startJ, std::string path);
	Map* createMap();
	ObjectLayer* creatObjectLayer();
	Menu* createMenu();
	void run();
	void render();
	void handleEvent(SDL_Event e);
	bool loadFont(std::string path, int fontSize);
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

	void renderMenu();
	void handleMenuEvent(SDL_Event e);

	void handleAttackingEvent(SDL_Event e);



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
	Mix_Music* music;
	/*LTexture winText;*/
	EntityLayer* mEntLayer;
	LTimer timer;
	FogOfWar* fogOfWar;

	int mGameSate;
	int playerState;
	int monsState;
	Menu* mMenu;

	Uint32 monsAttackStart;
	Uint32 playerAttackStart;
	Uint32 playerMoveStart;
	SoundEffect mons_melee_attack;
	SoundEffect player_melee_attack;
	SoundEffect player_moving;

	TTF_Font* mFont;
};

#endif