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
	void updateFogOfWar();
	int hasLOS(int x2, int y2);
	void doPlayer(SDL_Event& e);
	void doEntity();
	Entity::Dest pathFinding(Entity* monster);


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
};

#endif