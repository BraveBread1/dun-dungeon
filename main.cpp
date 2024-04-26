#include"CommonFunc.h"
#include"Game.h"

int main(int argc, char* argv[])
{
	Game* game = new Game;
	game->gameInit();
	game->run();
	game->close();
	game = NULL;

	return 0;
}