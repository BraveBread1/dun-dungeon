﻿#include"Game.h"

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
	
	mEntLayer = NULL;

	fogOfWar = NULL;

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

	mEntLayer->free();
	mEntLayer = NULL;

	fogOfWar->free();
	fogOfWar = NULL;

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

	if (player->loadHpTexture("assests/img/green_hp.png", "assests/img/red_hp.png", mRenderer) == false)
	{
		std::cerr << "Failed to load hp texture" << std::endl;
	}
	return player;
}

Map* Game::createMap()
{
	Map* map = new Map(LEVEL1_ROWS, LEVEL1_COLS, LEVEL1_LAYERS);
	std::string* layerPathLevel1 = new std::string[LEVEL1_LAYERS];
	layerPathLevel1[0] = "assests/map/tileMap/level1/ground.map";
	layerPathLevel1[1] = "assests/map/tileMap/level1/water.map";
	layerPathLevel1[2] = "assests/map/tileMap/level1/decor.map";
	layerPathLevel1[3] = "assests/map/tileMap/level1/wall1.map";
	layerPathLevel1[4] = "assests/map/tileMap/level1/wall2.map";

	if (map->setTileClips("assests/map/tile.png", mRenderer) == false)
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


		if (mPlayer->getTurn())
		{
			doPlayer(e);
		}
		
		if (mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->getType() == 18)
		{
			win = true;
		}
		if (mPlayer->getTurn() == 0)
		{
			doEntity();
			mPlayer->setTurn(1);
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
			if (i == 3)
			{
				mObjectLayer->render(camera, mRenderer, scale);
				mPlayer->render(mRenderer, camera, scale);
				mEntLayer->render(camera, mRenderer, timer.getTicks(), scale);
				
			}
		}
		
	}
	else
	{
		winText.render((SCREEN_WIDTH - winText.getWidth()) / 2, (SCREEN_HEIGHT - winText.getHeight()) / 2, mRenderer);
	}

	fogOfWar->render(camera, mRenderer, scale);

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
		updateHasSolid();
		handleEvent(*e);
		
		fogOfWar->updateSolid(mMap->getTileSet());
		fogOfWar->update(mPlayer->getPosI(), mPlayer->getPosJ());
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

	mEntLayer = new EntityLayer(LEVEL1_ROWS, LEVEL1_COLS);

	if (mEntLayer->setPosition("assests/map/tileMap/level1/entity.map") == false)
	{
		flag = false;
		std::cerr << "unable to load entity map" << std::endl;
	}
	else
	{
		std::string entPath[1];
		entPath[0] = "assests/sprite/rat.png";
		if (mEntLayer->loadEntTextureSet(entPath, mRenderer) == false)
		{
			flag = false;
			std::cerr << "unable to load entity texture" << std::endl;
		}

		if (mEntLayer->loadHpTexture("assests/img/green_hp.png", "assests/img/red_hp.png", mRenderer) == false)
		{
			flag = false;
			std::cerr << "unable to load entity's hp texture" << std::endl;
		}
	}

	fogOfWar = new FogOfWar;
	if (fogOfWar->loadFogTexture("assests/img/fog.png", mRenderer) == false)
	{
		flag = false;
	}

	timer.start();


	music = Mix_LoadMUS("assests/music/core_src_main_assets_music_sewers_1.ogg");

	srand(time(NULL));

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

void Game::updateHasSolid()
{
	int j, i;
	for (int l = 0; l < LEVEL1_LAYERS; ++l)
	{
		for (i = 0; i < LEVEL1_ROWS; i++)
		{
			for (j = 0; j < LEVEL1_COLS; j++)
			{
				//mMap->tileSet[l][i][j]->setVisible(0);
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

void Game::doPlayer(SDL_Event& e)
{
	int i = mPlayer->getPosI();
	int j = mPlayer->getPosJ();
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: i -= 1; mPlayer->setTurn(0); break;
		case SDLK_DOWN: i += 1; mPlayer->setTurn(0); break;
		case SDLK_LEFT:
		{
			mPlayer->setFacing(0);
			mPlayer->setTurn(0);
			j -= 1; break;
		}
		case SDLK_RIGHT:
		{
			mPlayer->setFacing(1);
			mPlayer->setTurn(0);
			j += 1; break;
		}
		}
	}

	bool isPath = true;
	if (mMap->tileSet[0][i][j]->getType() > 21 || mMap->tileSet[0][i][j]->getType() < 1) isPath = false;

	bool isMonster = false;
	Entity* target = mEntLayer->checkEntCollision(i, j);
	if (target != NULL)
	{
		isMonster = true;
		mPlayer->attack(target);
		if (target->getDead())
		{
			mEntLayer->delEnt(target);
		}

	}


	if (isPath && isMonster == false)
	{
		mPlayer->setPosI(i);
		mPlayer->setPosJ(j);

	}

}

void Game::doEntity()
{
	Entity* currentMonster = mEntLayer->getHead();
	while (currentMonster != NULL)
	{
		//if (hasLOS(currentMonster->getPosI(), currentMonster->getPosJ()))
		//{
		//	if (currentMonster->nextToPlayer(mPlayer->getPosI(), mPlayer->getPosJ()))
		//	{
		//		mPlayer->attacked(currentMonster);
		//	}
		//	else
		//	{
		//		currentMonster->setPath(pathFinding(currentMonster));
		//		currentMonster->movte();
		//	}
		//}

		entThink(currentMonster);
		currentMonster = currentMonster->next;
	}
}

void Game::pathFinding(Entity* monster, int* di, int* dj, int pi, int pj)
{

	int monsI = monster->getPosI();
	int monsJ = monster->getPosJ();

	*di = 0;
	*dj = 0;

	if (pi < monsI && pj < monsJ)
	{
		if (monster->isBlocked(monsJ - 1, monsI - 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = -1;
			*dj = -1;
		}
		else if (monster->isBlocked(monsJ - 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = -1;
		}
		else if (monster->isBlocked(monsJ, monsI - 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = -1;
			*dj = 0;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi == monsI && pj < monsJ)
	{
		if (monster->isBlocked(monsJ - 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = -1;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi > monsI && pj < monsJ)
	{
		if (monster->isBlocked(monsJ - 1, monsI + 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 1;
			*dj = -1;
		}
		else if (monster->isBlocked(monsJ - 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = -1;
		}
		else if (monster->isBlocked(monsJ, monsI + 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 1;
			*dj = 0;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi > monsI && pj == monsJ)
	{
		if (monster->isBlocked(monsJ, monsI + 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 1;
			*dj = 0;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi > monsI && pj > monsJ)
	{
		if (monster->isBlocked(monsJ + 1, monsI + 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 1;
			*dj = 1;
		}
		else if (monster->isBlocked(monsJ, monsI + 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 1;
			*dj = 0;
		}
		else if (monster->isBlocked(monsJ + 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = 1;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi == monsI && pj > monsJ)
	{
		if (monster->isBlocked(monsJ + 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = 1;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi < monsI && pj > monsJ)
	{
		if (monster->isBlocked(monsJ + 1, monsI - 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = -1;
			*dj = 1;
		}
		else if (monster->isBlocked(monsJ, monsI - 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = -1;
			*dj = 0;
		}
		else if (monster->isBlocked(monsJ + 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = 1;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else if (pi < monsI && pj == monsJ)
	{
		if (monster->isBlocked(monsJ - 1, monsI, hasSolid, pj, pi, mEntLayer->getHead()) == false)
		{
			*di = 0;
			*dj = -1;
		}
		else
		{
			*di = 0;
			*dj = 0;
		}
	}
	else
	{
		*di = 0;
		*dj = 0;
	}

}

//void Game::entThink(Entity* currentEnt)
//{
//	if (currentEnt->getAlert() == false)
//	{
//		currentEnt->lookForPlayer(mPlayer->getPosI(), mPlayer->getPosJ(), hasSolid);
//		//if (currentEnt->getAlert() == true)
//		//{
//		//	moveToPlayer(currentEnt);
//		//}
//	}
//	else if (hasLOS(currentEnt->getPosI(), currentEnt->getPosJ()))
//	{
//		moveToPlayer(currentEnt);
//	}
//	else
//	{
//		patrol(currentEnt);
//	}
//}

void Game::entThink(Entity* currentEnt)
{
	if (currentEnt->getAlert() == 0)
	{
		currentEnt->lookForPlayer(mPlayer->getPosI(), mPlayer->getPosJ(), hasSolid);
		if (currentEnt->getAlert() != 0)
		{
			moveToPlayer(currentEnt);
			currentEnt->setSaw(mPlayer->getPosI(), mPlayer->getPosJ());
		}
	}
	else if (hasLOS(currentEnt->getPosI(), currentEnt->getPosJ()))
	{
		currentEnt->setHunt(3);
		moveToPlayer(currentEnt);
		currentEnt->setSaw(mPlayer->getPosI(), mPlayer->getPosJ());
	}
	else
	{
		if (currentEnt->getHunt() != 0)
		{
			if (currentEnt->gotLastSaw() == false && currentEnt->getSawI() != -1 && currentEnt->getSawJ() != -1)
			{
				moveToLastSaw(currentEnt);
				currentEnt->setHunt(currentEnt->getHunt() - 1);
				if (currentEnt->getHunt() == 0)
				{
					currentEnt->setSaw(-1, -1);
				}
			}
			else
			{
				patrol(currentEnt);
			}
		}
		else
		{
			patrol(currentEnt);
		}
	}
}

void Game::moveToPlayer(Entity* currentEnt)
{
	int di, dj;
	if (currentEnt->nextToPlayer(mPlayer->getPosI(), mPlayer->getPosJ()))
	{
		mPlayer->attacked(currentEnt->getDame());
	}
	else
	{
		pathFinding(currentEnt, &di, &dj, mPlayer->getPosI(), mPlayer->getPosJ());
		currentEnt->move(di, dj, hasSolid);
	}

	currentEnt->setPatrolDest(mPlayer->getPosJ(), mPlayer->getPosI());
}

void Game::moveToLastSaw(Entity* currentEnt)
{
	int di, dj;
	pathFinding(currentEnt, &di, &dj, currentEnt->getSawI(), currentEnt->getSawJ());
	currentEnt->move(di, dj, hasSolid);

	/*currentEnt->setPatrolDest(mPlayer->getPosJ(), mPlayer->getPosI());*/
}

void Game::patrol(Entity* currentEnt)
{
	std::cout << "patrol" << std::endl;
	int di = rand() % 3 - 1;
	int dj = rand() % 3 - 1;
	if (currentEnt->isBlocked(dj + currentEnt->getPosJ(), di + currentEnt->getPosI(), hasSolid, mPlayer->getPosJ(), mPlayer->getPosI(), mEntLayer->getHead()) == false)
	{
		currentEnt->move(di, dj, hasSolid);
	}
}