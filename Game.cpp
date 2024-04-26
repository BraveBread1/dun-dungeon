#include"Game.h"

Game::Game()
{
	isPlaying = false;
	win = false;
	mWindow = NULL;
	mRenderer = NULL;
	mPlayer = NULL;
	mMenu = NULL;
	music = NULL;
	mFont = NULL;
	gameState = MENU;
	currentLv = NULL;

	LEVEL_WIDTH = 512;
	LEVEL_HEIGHT = 736;
	LEVEL_ROWS = 46;
	LEVEL_COLS = 32;
}

Game::~Game()
{
	close();
}

void Game::close()
{
	mMenu->free();
	mMenu = NULL;
	mPlayer->FreePlayer();
	mPlayer = NULL;

	Mix_FreeMusic(music);
	music = NULL;

	TTF_CloseFont(mFont);
	mFont = NULL;

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	SDL_DestroyWindow(mWindow);
	mWindow = NULL;


	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::gameInit()
{
	bool flag = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL Error: %s\n" << SDL_GetError();
		flag = false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cerr << "Warning: Linear texture filtering not enabled!";
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: %s\n" << IMG_GetError();
		flag = false;
	}
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: %s\n" << TTF_GetError();
		flag = false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError();
		flag = false;
	}

	if (flag)
	{
		mWindow = SDL_CreateWindow("Dun Dungeon Level", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			std::cout << "Window could not be created! SDL Error: %s\n" << SDL_GetError();
			flag = false;
		}

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (mRenderer == NULL)
		{
			std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError();
			flag = false;
		}
	}

	srand(time(NULL));

	if (loadFont("assests/font/pixel_font.ttf", 28) == false)
	{
		std::cerr << "unable to load win text" << std::endl;
	}

	mMenu = createMenu();


	mPlayer = createPlayer(3, 6, "assests/sprite/warrior.png");
	music = Mix_LoadMUS("assests/music/core_src_main_assets_music_sewers_1.ogg");

	return flag;

}

bool Game::loadFont(std::string path, int fontSize)
{
	bool success = true;
	mFont = TTF_OpenFont(path.c_str(), 12);
	if (mFont == NULL)
	{
		std::cerr << "Failed to load font! SDL_ttf Error: %s\n" << TTF_GetError();
		success = false;
	}
	return success;
}

Menu* Game::createMenu()
{
	Menu* menu = new Menu;
	if (menu->loadMenuTexture("assests/img/background.png", mRenderer) == false)
	{
		std::cerr << "unable to load menu's background" << std::endl;
	}
	return menu;
}

Player* Game::createPlayer(int startI, int startJ, std::string path)
{
	Player* player = new Player(startI, startJ, LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_ROWS, LEVEL_COLS);
	if (player->loadPlayerTexture(path.c_str(), mRenderer) == false)
	{
		std::cout << "Failed to load sprite sheet texture!\n";
	}
	else
	{
		player->setClip(0, 15, PLAYER_WIDTH, PLAYER_HEIGHT);
	}

	if (player->loadStatusTexture("assests/img/hero_status.png", mRenderer, mFont) == false)
	{
		std::cerr << "Failed to load hp texture" << std::endl;
	}
	return player;
}

void Game::run()
{
	isPlaying = true;
	SDL_Event* e = new SDL_Event;
	Mix_PlayMusic(music, -1);
	while (isPlaying)
	{
		if (gameState == MENU)
		{
			handleMenuEvent(*e);
			renderMenu();
		}
		else
		{
			int lv = 3;
			while (lv <= 3 && lv != -1)
			{
				if (lv != -1)
				{
					if (lv == 1)
					{
						mPlayer->setPosI(6);
						mPlayer->setPosJ(3);
						LEVEL_WIDTH = 512;
						LEVEL_HEIGHT = 736;
						LEVEL_ROWS = 46;
						LEVEL_COLS = 32;
					}
					else if (lv == 2)
					{
						mPlayer->setPosI(3);
						mPlayer->setPosJ(6);
						LEVEL_WIDTH = 480;
						LEVEL_HEIGHT = 496;
						LEVEL_ROWS = 31;
						LEVEL_COLS = 30;
					}
					else if(lv == 3)
					{
						mPlayer->setPosI(11);
						mPlayer->setPosJ(4);
						LEVEL_WIDTH = 656;
						LEVEL_HEIGHT = 384;
						LEVEL_ROWS = 24;
						LEVEL_COLS = 41;

					}
					currentLv = new Level(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_ROWS, LEVEL_COLS);
					currentLv->levelInit(mWindow, mRenderer, mFont, mPlayer, lv);
					currentLv->run(e, isPlaying);
					if (currentLv->getWin())
					{
						++lv;
					}
					else lv = -1;
					currentLv->close();
					currentLv = NULL;
				}
			}
			isPlaying = false;
		}
	}
}

void Game::renderMenu()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, RENDER_DRAW_COLOR);
	SDL_RenderClear(mRenderer);
	mMenu->render(mRenderer);
	SDL_RenderPresent(mRenderer);
}

void Game::handleMenuEvent(SDL_Event e)
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			isPlaying = false;
		}
		else
		{
			mMenu->handleEvent(e);
			if (mMenu->enter->getPressed() == 2)
			{
				gameState = PLAYER_ACTION;
			}
			if (mMenu->quit->getPressed() == 2)
			{
				isPlaying = false;
			}
		}
	}
}