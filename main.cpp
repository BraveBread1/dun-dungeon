#include"CommonFunc.h"
#include"LTexture.h"
#include"Player.h"
#include"Tile.h"
#include"Map.h"
#include"Game.h"

int main(int argc, char* argv[])
{
	Game* level1 = new Game;
	level1->gameInit();
	level1->run();
	level1->close();
	level1 = NULL;

	return 0;
}