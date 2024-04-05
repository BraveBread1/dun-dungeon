#include"Game.h"

Game::Game()
{
	isPlaying = false;
	win = false;
	mWindow = NULL;
	mRenderer = NULL;
	mPlayer = NULL;
	mMap = NULL;
	mObjectLayer = NULL;
	music = NULL;
	scale = 1;
	hasSolid = new bool* [LEVEL1_ROWS];
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		hasSolid[i] = new bool[LEVEL1_COLS];
	}
}

Game::~Game()
{
	close();
}

void Game::close()
{
	Mix_FreeMusic(music);
	music = NULL;

	mMap->free();
	mMap = NULL;

	mPlayer->FreePlayer();
	mPlayer = NULL;

	mObjectLayer->free();
	mObjectLayer = NULL;

	winText.free();

	clearSolidArr();

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	SDL_DestroyWindow(mWindow);
	mWindow = NULL;


	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

Player* Game::createPlayer(int startI, int startJ, std::string path)
{
	Player* player = new Player(startI, startJ);
	if (player->loadPlayerTexture(path.c_str(), mRenderer) == false)
	{
		std::cout << "Failed to load sprite sheet texture!\n";
	}
	else
	{
		player->setClip(0, 15, PLAYER_WIDTH, PLAYER_HEIGHT);
	}
	return player;
}

Map* Game::createMap()
{
	Map* map = new Map(LEVEL1_ROWS, LEVEL1_COLS, LEVEL1_LAYERS);
	std::string* layerPathLevel1 = new std::string[LEVEL1_LAYERS];
	layerPathLevel1[0] = "assests/map/tileMap/level1/layer1.map";
	layerPathLevel1[1] = "assests/map/tileMap/level1/layer2.map";
	layerPathLevel1[2] = "assests/map/tileMap/level1/layer3.map";

	if (map->setTileClips("assests/map/tiles_sewers.png", mRenderer) == false)
	{
		std::cerr << "unable to set tile clips" << std::endl;
	}
	if (map->setTiles(layerPathLevel1) == false)
	{
		std::cerr << "unable to load tile set" << std::endl;
	}
	return map;
}

ObjectLayer* Game::creatObjectLayer()
{
	ObjectLayer* objLayer = new ObjectLayer(LEVEL1_ROWS, LEVEL1_COLS);
	if (objLayer->setObjClips("assests/map/tiles_sewers.png", mRenderer) == false)
	{
		std::cerr << "unable to set object clips" << std::endl;
	}
	if (objLayer->setPosition("assests/map/tileMap/level1/object.map") == false)
	{
		std::cerr << "unable to load object set" << std::endl;
	}
	return objLayer;
}

void Game::handleEvent(SDL_Event e)
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			isPlaying = false;
		}
		else if (e.type == SDL_MOUSEWHEEL)
		{
			if (e.wheel.y != 0)
			{
				scale += e.wheel.preciseY * 0.1;
			}
			if (scale < 1) scale = 1;
			/*else if (scale >= 2.5) scale = 2.5;*/
		}

		mPlayer->handleEvent(e, mMap->tileSet);
		
		int i = mPlayer->getPosI();
		int j = mPlayer->getPosJ();
		if (mObjectLayer->getObjSet()[i][j]->getType() == 18)
		{
			win = true;
		}
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, RENDER_DRAW_COLOR);
	SDL_RenderClear(mRenderer);

	if (win == false)
	{
		mPlayer->setCamera(camera, scale);

		for (int i = 0; i < LEVEL1_LAYERS; ++i)
		{
			mMap->renderLayer(camera, mRenderer, i, scale);
			if (i == 0)
			{
				mObjectLayer->render(camera, mRenderer, scale);
				mPlayer->render(mRenderer, camera, scale);
			}
		}
	}
	else
	{
		winText.render((SCREEN_WIDTH - winText.getWidth()) / 2, (SCREEN_HEIGHT - winText.getHeight()) / 2, mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::clearSolidArr()
{
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		delete[] hasSolid[i];
		hasSolid[i] = NULL;
	}
	delete[] hasSolid;
	hasSolid = NULL;
}

void Game::run()
{
	isPlaying = true;
	SDL_Event* e = new SDL_Event;
	Mix_PlayMusic(music, -1);
	while (isPlaying)
	{
		handleEvent(*e);
		updateFogOfWar();
		render();
	}
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
		mWindow = SDL_CreateWindow("Dun Dungeon Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	mPlayer = createPlayer(6, 3, "assests/sprite/warrior.png");
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	mMap = createMap();
	mObjectLayer = creatObjectLayer();

	if (loadText("assest/font/pixel_font.ttf", 28) == false)
	{
		std::cerr << "unable to load win text" << std::endl;
	}

	music = Mix_LoadMUS("assests/music/core_src_main_assets_music_sewers_1.ogg");

	return flag;
}

bool Game::loadText(std::string path, int fontSize)
{
	bool success = true;
	TTF_Font* font = NULL;
	font = TTF_OpenFont("assests/font/pixel_font.ttf", 56);
	if (font == NULL)
	{
		std::cerr << "Failed to load font! SDL_ttf Error: %s\n" << TTF_GetError();
		success = false;
	}
	else
	{
		SDL_Color textColor = { 122, 0, 0 };
		if (!winText.loadFromRenderedText("YOU WIN! OR NOT?", textColor, font, mRenderer))
		{
			std::cerr << "Failed to render text texture!\n";
			success = false;
		}
	}
	font = NULL;
	return success;
}

void Game::updateFogOfWar()
{
	int j, i;
	int mi, mj;
	for (int l = 0; l < LEVEL1_LAYERS; ++l)
	{
		for (i = 0; i < LEVEL1_ROWS; i++)
		{
			for (j = 0; j < LEVEL1_COLS; j++)
			{
				mMap->tileSet[l][i][j]->setVisible(0);
				hasSolid[i][j] = 0;
			}
		}
	}

	for (int l = 0; l < LEVEL1_LAYERS; ++l)
	{
		for (i = 0; i < LEVEL1_ROWS; i++)
		{
			for (j = 0; j < LEVEL1_COLS; j++)
			{
				if (mMap->tileSet[l][i][j]->getSolid())
				{
					hasSolid[i][j] = 1;
				}
			}
		}
	}


	for (i = -VIS_DISTANCE; i <= VIS_DISTANCE; i++)
	{
		for (j = -VIS_DISTANCE; j <= VIS_DISTANCE; j++)
		{
			mi = mPlayer->getPosI() + i;
			mj = mPlayer->getPosJ() + j;

			if (getDistance(mPlayer->getPosJ(), mPlayer->getPosI(), mj, mi) <= VIS_DISTANCE)
			{
				if (mi >= 0 && mj >= 0 && mj < LEVEL1_COLS && mi < LEVEL1_ROWS)
				{
					for (int l = 0; l < LEVEL1_LAYERS; ++l)
					{
						if (!mMap->tileSet[l][mi][mj]->getVisible() && hasLOS(mi, mj))
						{
							mMap->tileSet[l][mi][mj]->setRevealed(1);
							mMap->tileSet[l][mi][mj]->setVisible(1);
						}
					}
				}
			}
		}
	}
}

int Game::hasLOS(int i2, int j2)
{
	int i1, j1, di, dj, si, sj, err, e2;
	i1 = mPlayer->getPosI();
	j1 = mPlayer->getPosJ();

	di = abs(i2 - i1);
	dj = abs(j2 - j1);

	si = (i1 < i2) ? 1 : -1;
	sj = (j1 < j2) ? 1 : -1;
	err = dj - di;

	while (1)
	{
		e2 = 2 * err;

		if (e2 > -di)
		{
			err -= di;
			j1 += sj;
		}

		if (e2 < dj)
		{
			err += dj;
			i1 += si;
		}

		if (j1 == j2 && i1 == i2)
		{
			return 1;
		}

		if (hasSolid[i1][j1])
		{
			return 0;
		}

	}


	return 0;
}