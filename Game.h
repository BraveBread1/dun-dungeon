#ifndef Game_h_

#define Game_h_

#include"CommonFunc.h"
#include"Player.h"
#include"Map.h"
#include"ObjectLayer.h"

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

private:
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
};

#endif