#include"CommonFunc.h"
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