#ifndef Game_h
#define Game_h

#include"CommonFunc.h"
#include"Level.h"
#include"Menu.h"

enum GameState
{
	MENU, LEVEL1
};

class Game
{
public:
	Game();
	~Game();

	bool gameInit();
	void close();
	bool loadFont(std::string path, int fontSize);
	Menu* createMenu();
	Player* createPlayer(int startI, int startJ, std::string path);

	void run();

	void renderMenu();
	void handleMenuEvent(SDL_Event e);

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Player* mPlayer;
	Menu* mMenu;
	bool isPlaying;
	bool win;
	Mix_Music* music;
	TTF_Font* mFont;

	int gameState;

	Level* currentLv;

	int LEVEL_WIDTH;
	int LEVEL_HEIGHT;
	int LEVEL_ROWS;
	int LEVEL_COLS;

};


#endif // !Game_h
