#include"Menu.h"

Menu::Menu()
{
	enter = new Button(402, 400, 696, 60);
	gameNameClip.x = 0;
	gameNameClip.y = 0;
	gameNameClip.w = 100;
	gameNameClip.h = 55;
	x = 0;
}

Menu::~Menu()
{
	free();
}

bool Menu::loadMenuTexture(std::string path, SDL_Renderer* screen)
{
	return menuTexture.loadFromFile(path, screen) && enter->loadButtonTexture("assests/img/enter_button.png", screen) && gameName.loadFromFile("assests/img/game_name.png", screen);
}

void Menu::render(SDL_Renderer* screen)
{
	menuTexture.render(0, 0, screen);
	enter->render(screen);
	gameName.render(550, 150, screen, &gameNameClip, 1, 4);
}

void Menu::free()
{
	menuTexture.free();
	enter->free();
}

void Menu::handleEvent(SDL_Event e, int &mGameSate)
{
	enter->hanldEvent(e);
	//if (enter->getPressed() == 2)
	//{
	//	mGameSate = 1
	//}
}