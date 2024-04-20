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
	void run();
	void render();
	void handleEvent(SDL_Event e);
	bool loadText(std::string path, int fontSize);
	void clearSolidArr();
	void updateHasSolid();
	int hasLOS(int x2, int y2);
	void doPlayer(SDL_Event& e);
	void doEntity();
	void entThink(Entity* currentEnt);
	void pathFinding(Entity* monster, int *di, int *dj, int pi, int pj);

	void moveToPlayer(Entity* currentEnt);
	void patrol(Entity* currentEnt);
	void moveToLastSaw(Entity* currentEnt);


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
	LTexture winText;
	EntityLayer* mEntLayer;
	LTimer timer;
	FogOfWar* fogOfWar;
	Astar aStar;
};

#endif