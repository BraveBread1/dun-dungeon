#include"Level.h"

Level::Level(int LEVEL_WIDTH, int LEVEL_HEIGHT, int LEVEL_ROWS, int LEVEL_COLS)
{
	isPlaying = false;
	win = false;
	mWindow = NULL;
	mRenderer = NULL;
	mPlayer = NULL;
	mMap = NULL;
	mObjectLayer = NULL;
	
	mEntLayer = NULL;

	fogOfWar = NULL;

	this->LEVEL_WIDTH = LEVEL_WIDTH;
	this->LEVEL_HEIGHT = LEVEL_HEIGHT;
	this->LEVEL_ROWS = LEVEL_ROWS;
	this->LEVEL_COLS = LEVEL_COLS;

	scale = 2;
	hasSolid = new bool* [LEVEL_ROWS];
	for (int i = 0; i < LEVEL_ROWS; ++i)
	{
		hasSolid[i] = new bool[LEVEL_COLS];
	}

	mLevelSate = PLAYER_ACTION;
	playerState = PLAYER_NO_ATTACK;
	monsState = MONS_NO_ATTACK;
	monsAttackStart = 0;
	playerMoveStart = 0;
}

Level::~Level()
{
	close();
}

void Level::close()
{

	mMap->free();
	mMap = NULL;

	mPlayer = NULL;

	mObjectLayer->free();
	mObjectLayer = NULL;

	mEntLayer->free();
	mEntLayer = NULL;

	fogOfWar->free();
	fogOfWar = NULL;

	clearSolidArr();

	mFont = NULL;
	mRenderer = NULL;
	mWindow = NULL;
}

Map* Level::createMap(int lv)
{
	Map* map = new Map(LEVEL_ROWS, LEVEL_COLS, LEVEL_LAYERS);
	std::string* layerPathLevel1 = new std::string[LEVEL_LAYERS];
	if (lv == 1)
	{
		layerPathLevel1[0] = "assests/map/tileMap/level1/ground.map";
		layerPathLevel1[1] = "assests/map/tileMap/level1/water.map";
		layerPathLevel1[2] = "assests/map/tileMap/level1/decor.map";
		layerPathLevel1[3] = "assests/map/tileMap/level1/wall1.map";
		layerPathLevel1[4] = "assests/map/tileMap/level1/wall2.map";
	}
	else if (lv == 2)
	{
		layerPathLevel1[0] = "assests/map/tileMap/level2/ground.map";
		layerPathLevel1[1] = "assests/map/tileMap/level2/water.map";
		layerPathLevel1[2] = "assests/map/tileMap/level2/decor.map";
		layerPathLevel1[3] = "assests/map/tileMap/level2/wall1.map";
		layerPathLevel1[4] = "assests/map/tileMap/level2/wall2.map";
	}
	else if (lv == 3)
	{
		layerPathLevel1[0] = "assests/map/tileMap/level3/ground.map";
		layerPathLevel1[1] = "assests/map/tileMap/level3/water.map";
		layerPathLevel1[2] = "assests/map/tileMap/level3/decor.map";
		layerPathLevel1[3] = "assests/map/tileMap/level3/wall1.map";
		layerPathLevel1[4] = "assests/map/tileMap/level3/wall2.map";
	}


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

ObjectLayer* Level::creatObjectLayer(int lv)
{
	ObjectLayer* objLayer = new ObjectLayer(LEVEL_ROWS, LEVEL_COLS);
	if (objLayer->setObjClips("assests/map/tiles_sewers.png", mRenderer) == false)
	{
		std::cerr << "unable to set object clips" << std::endl;
	}
	if (lv == 1)
	{
		if (objLayer->setPosition("assests/map/tileMap/level1/object.map") == false)
		{
			std::cerr << "unable to load object set" << std::endl;
		}
	}
	else if (lv == 2)
	{
		if (objLayer->setPosition("assests/map/tileMap/level2/object.map") == false)
		{
			std::cerr << "unable to load object set" << std::endl;
		}
	}
	else if (lv == 3)
	{
		if (objLayer->setPosition("assests/map/tileMap/level3/object.map") == false)
		{
			std::cerr << "unable to load object set" << std::endl;
		}
	}


	return objLayer;
}

void Level::handleEvent(SDL_Event e)
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
			else if (scale >= 3.5) scale = 3.5;
		}

		if (mPlayer->getTurn())
		{
			doPlayer(e);
			handleObjectEvent();
		}
	}
}

void Level::handleObjectEvent()
{
	if (mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->getType() == 18)
	{
		win = true;
	}
	else if (mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->getType() == 70)
	{
		mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->setType(3);
	}
	else if (mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->getType() >= 214 && mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->getType() <= 216)
	{
		mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->setType(0);
		mObjectLayer->getObjSet()[mPlayer->getPosI() - 1][mPlayer->getPosJ()]->setType(0);
	}
	else if (mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->getType() == 113)
	{
		mObjectLayer->getObjSet()[mPlayer->getPosI()][mPlayer->getPosJ()]->setType(114);
		mObjectLayer->getObjSet()[mPlayer->getPosI() - 1][mPlayer->getPosJ()]->setType(226);
	}
}

void Level::render()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, RENDER_DRAW_COLOR);
	SDL_RenderClear(mRenderer);

	if (win == false)
	{
		mPlayer->setCamera(camera, scale);

		for (int i = 0; i < LEVEL_LAYERS; ++i)
		{
			mMap->renderLayer(camera, mRenderer, i, scale);
			if (i == 3)
			{
				mObjectLayer->render(camera, mRenderer, scale);
				if (playerState == PLAYER_ATTACK)
				{
					mPlayer->render(mRenderer, camera, SDL_GetTicks() - playerAttackStart, scale);
				}
				else if(playerAttackStart == PLAYER_MOVE)
				{
					mPlayer->render(mRenderer, camera, SDL_GetTicks() - playerMoveStart, scale);
				}
				else
				{
					mPlayer->render(mRenderer, camera, 0, scale);
				}
				
				mEntLayer->render(camera, mRenderer, SDL_GetTicks() - monsAttackStart, scale);
			}
		}
		fogOfWar->render(camera, mRenderer, scale);
		mPlayer->renderStatus(mRenderer);
	}
	else
	{
		/*winText.render((SCREEN_WIDTH - winText.getWidth()) / 2, (SCREEN_HEIGHT - winText.getHeight()) / 2, mRenderer);*/
	}



	SDL_RenderPresent(mRenderer);
}

void Level::clearSolidArr()
{
	for (int i = 0; i < LEVEL_ROWS; ++i)
	{
		delete[] hasSolid[i];
		hasSolid[i] = NULL;
	}
	delete[] hasSolid;
	hasSolid = NULL;
}

void Level::run(SDL_Event* e, bool &play)
{
	isPlaying = true;
	while (isPlaying && win == false)
	{
		if (mLevelSate == PLAYER_ACTION)
		{
			if (playerState == PLAYER_NO_ATTACK)
			{
				updateHasSolid();
				handleEvent(*e);
				if (playerState == PLAYER_ATTACK)
				{
					updateHasSolid();
					handleAttackingEvent(*e);
					Uint32 currentTime = SDL_GetTicks();
					player_melee_attack.playOnce();
					if (currentTime > playerAttackStart + ATTACK_TIME)
					{
						playerState = PLAYER_NO_ATTACK;
						player_melee_attack.reset();
						mLevelSate = MONSTER_ACTION;
					}
				}
				else
				{
					mLevelSate = MONSTER_ACTION;
				}
			}
			else if(playerState == PLAYER_ATTACK)
			{
				updateHasSolid();
				handleAttackingEvent(*e);
				Uint32 currentTime = SDL_GetTicks();
				player_melee_attack.playOnce();
				if (currentTime > playerAttackStart + ATTACK_TIME)
				{
					playerState = PLAYER_NO_ATTACK;
					player_melee_attack.reset();
					mLevelSate = MONSTER_ACTION;
				}
			}
			fogOfWar->updateSolid(mMap->getTileSet(), mObjectLayer->getObjSet());
			fogOfWar->update(mPlayer->getPosI(), mPlayer->getPosJ());
			mPlayer->update(mRenderer, mFont);
			render();
		}
		else if (mLevelSate == MONSTER_ACTION)
		{
			if (monsState == MONS_NO_ATTACK)
			{
				if (mPlayer->getTurn() == 0)
				{
					doEntity();
					mPlayer->setTurn(1);
				}
				if (monsState == MONS_ATTACK)
				{
					handleAttackingEvent(*e);
					updateHasSolid();
					Uint32 currentTime = SDL_GetTicks();
					mons_melee_attack.playOnce();
					if (currentTime > monsAttackStart + ATTACK_TIME)
					{
						monsState = MONS_NO_ATTACK;
						mons_melee_attack.reset();
						mLevelSate = PLAYER_ACTION;
					}
				}
				else
				{
					mLevelSate = PLAYER_ACTION;
				}
			}
			else if (monsState == MONS_ATTACK)
			{
				handleAttackingEvent(*e);
				updateHasSolid();
				Uint32 currentTime = SDL_GetTicks();
				mons_melee_attack.playOnce();
				if (currentTime > monsAttackStart + ATTACK_TIME)
				{
					monsState = MONS_NO_ATTACK;
					mons_melee_attack.reset();
					mLevelSate = PLAYER_ACTION;
				}
			}
			fogOfWar->updateSolid(mMap->getTileSet(), mObjectLayer->getObjSet());
			fogOfWar->update(mPlayer->getPosI(), mPlayer->getPosJ());
			mPlayer->update(mRenderer, mFont);
			render();
		}

	}
	play = isPlaying;
}

bool Level::levelInit(SDL_Window* window, SDL_Renderer* screen, TTF_Font* font, Player* player, int lv)
{
	bool flag = true;

	mWindow = window;
	mRenderer = screen;
	mFont = font;
	mPlayer = player;
	currentLv = lv;

	mMap = createMap(lv);

	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	
	mObjectLayer = creatObjectLayer(lv);



	mEntLayer = new EntityLayer(LEVEL_ROWS, LEVEL_COLS);

	if (lv == 1)
	{
		if (mEntLayer->setPosition("assests/map/tileMap/level1/entity.map") == false)
		{
			flag = false;
			std::cerr << "unable to load entity map" << std::endl;
		}
	}
	else if (lv == 2)
	{
		if (mEntLayer->setPosition("assests/map/tileMap/level2/entity.map") == false)
		{
			flag = false;
			std::cerr << "unable to load entity map" << std::endl;
		}
	}
	else if (lv == 3)
	{
		if (mEntLayer->setPosition("assests/map/tileMap/level3/entity.map") == false)
		{
			flag = false;
			std::cerr << "unable to load entity map" << std::endl;
		}
	}

	std::string* entPath = new std::string[MAX_MONSTER_TYPE];
	entPath[0] = "assests/sprite/rat.png";
	entPath[1] = "assests/sprite/king.png";
	if (mEntLayer->loadEntTextureSet(entPath, mRenderer) == false)
	{
		flag = false;
		std::cerr << "unable to load entity texture" << std::endl;
		std::cerr << "unable to load entity texture" << std::endl;
	}
	if (mEntLayer->loadHpTexture("assests/img/green_hp.png", "assests/img/red_hp.png", mRenderer) == false)
	{
		flag = false;
		std::cerr << "unable to load entity's hp texture" << std::endl;
	}
	delete[] entPath;
	entPath = NULL;

	fogOfWar = new FogOfWar(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_ROWS, LEVEL_COLS);
	if (fogOfWar->loadFogTexture("assests/img/fog.png", mRenderer) == false)
	{
		flag = false;
	}

	timer.start();

	if (mons_melee_attack.loadFromFile("assests/sounds/melee_attack.mp3") == false)
	{
		flag = false;
	}
	mons_melee_attack.setVolume(20);

	if (player_melee_attack.loadFromFile("assests/sounds/melee_attack.mp3") == false)
	{
		flag = false;
	}
	player_melee_attack.setVolume(20);

	if (player_moving.loadFromFile("assests/sounds/move.mp3") == false)
	{
		flag = false;
	}
	player_moving.setVolume(20);

	return flag;
}

void Level::updateHasSolid()
{
	int j, i;
	for (int l = 0; l < LEVEL_LAYERS; ++l)
	{
		for (i = 0; i < LEVEL_ROWS; i++)
		{
			for (j = 0; j < LEVEL_COLS; j++)
			{
				hasSolid[i][j] = 0;
			}
		}
	}

	for (int l = 0; l < LEVEL_LAYERS; ++l)
	{
		for (i = 0; i < LEVEL_ROWS; i++)
		{
			for (j = 0; j < LEVEL_COLS; j++)
			{
				if (mMap->tileSet[l][i][j]->getSolid())
				{
					hasSolid[i][j] = 1;
				}
			}
		}
	}
	for (int i = 0; i < LEVEL_ROWS; ++i)
	{
		for (int j = 0; j < LEVEL_COLS; ++j)
		{
			if ((mObjectLayer->getObjSet()[i][j]->getType() >= 213 && mObjectLayer->getObjSet()[i][j]->getType() <= 224) || (mObjectLayer->getObjSet()[i][j]->getType() >= 113 && mObjectLayer->getObjSet()[i][j]->getType() <= 116 && mObjectLayer->getObjSet()[i][j]->getType() != 114))
			{
				hasSolid[i][j] = 1;
			}
		}
	}
}

int Level::hasLOS(int i2, int j2)
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

void Level::doPlayer(SDL_Event& e)
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
	if (i >= LEVEL_ROWS || j >= LEVEL_COLS) isPath = false;
	else if(mMap->tileSet[0][i][j]->getType() > 21 || mMap->tileSet[0][i][j]->getType() < 1) isPath = false;

	bool isMonster = false;
	Entity* target = mEntLayer->checkEntCollision(i, j);
	if (target != NULL)
	{
		isMonster = true;
		mPlayer->attack(target);
		mPlayer->setCrStatus(1);
		playerState = PLAYER_ATTACK;
		playerAttackStart = SDL_GetTicks();
		mLevelSate = PLAYER_ACTION;
		if (target->getDead())
		{
			mPlayer->earnExp(target->getExp());
			mEntLayer->delEnt(target);
		}

	}


	if (isPath && isMonster == false)
	{
		mPlayer->setPosI(i);
		mPlayer->setPosJ(j);
		//mPlayer->setCrStatus(2);
		//playerState = PLAYER_MOVE;
		//playerMoveStart = SDL_GetTicks();
		//mLevelSate = PLAYER_ACTION;
	}

}

void Level::doEntity()
{
	Entity* currentMonster = mEntLayer->getHead();
	while (currentMonster != NULL)
	{
		entThink(currentMonster);
		currentMonster = currentMonster->next;
	}
}

void Level::pathFinding(Entity* monster, int* di, int* dj, int pi, int pj)
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
		if (monster->isBlocked(monsJ, monsI - 1, hasSolid, pj, pi, mEntLayer->getHead()) == false)
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
	else
	{
		*di = 0;
		*dj = 0;
	}

}

void Level::entThink(Entity* currentEnt)
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

void Level::moveToPlayer(Entity* currentEnt)
{
	int di, dj;
	if (currentEnt->nextToPlayer(mPlayer->getPosI(), mPlayer->getPosJ()))
	{
		mPlayer->attacked(currentEnt->getDame());
		currentEnt->setStatus(Entity::Status::M_ATTACK);
		monsState = MONS_ATTACK;
		mLevelSate = MONSTER_ACTION;
		monsAttackStart = SDL_GetTicks();
	}
	else
	{
		pathFinding(currentEnt, &di, &dj, mPlayer->getPosI(), mPlayer->getPosJ());
		currentEnt->move(di, dj, hasSolid);
	}

	currentEnt->setPatrolDest(mPlayer->getPosJ(), mPlayer->getPosI());
}

void Level::moveToLastSaw(Entity* currentEnt)
{
	int di, dj;
	pathFinding(currentEnt, &di, &dj, currentEnt->getSawI(), currentEnt->getSawJ());
	currentEnt->move(di, dj, hasSolid);

	/*currentEnt->setPatrolDest(mPlayer->getPosJ(), mPlayer->getPosI());*/
}

void Level::patrol(Entity* currentEnt)
{
	int di = rand() % 3 - 1;
	int dj = rand() % 3 - 1;
	if (currentEnt->isBlocked(dj + currentEnt->getPosJ(), di + currentEnt->getPosI(), hasSolid, mPlayer->getPosJ(), mPlayer->getPosI(), mEntLayer->getHead()) == false)
	{
		currentEnt->move(di, dj, hasSolid);
	}
}

void Level::handleAttackingEvent(SDL_Event e)
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
			else if (scale >= 3.5) scale = 3.5;
		}
	}
}

bool Level::getWin()
{
	return win;
}