#include"CommonFunc.h"
#include"LTexture.h"
#include"Player.h"
#include"Tile.h"

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!";
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		success = false;
	}

	return success;
}

void close()
{

	IMG_Quit();
	SDL_Quit();
}

bool setTiles(Tile*** tiles, SDL_Rect tileClips[])
{
	//Success flag
	bool tilesLoaded = true;

	//Open the map
	std::ifstream map("assests/map/map.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < LEVEL_HEIGHT_CELL; ++i)
		{
			for (int j = 0; j < LEVEL_WIDTH_CELL; ++j)
			{
				int tileType = -1;
				
				map >> tileType;
				if (map.fail())
				{
					std::cout << "Error loading map: Unexpected end of file!\n";
					tilesLoaded = false;
					break;
				}
				if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
				{
					tiles[i][j] = new Tile(j * TILE_LENG, i * TILE_LENG, tileType);
				}
				else
				{
					std::cout << "Error loading map: Invalid tile type at %d!\n" << i * 35 + j;
					tilesLoaded = false;
					break;
				}
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			tileClips[TILE_VOID].x = 0;
			tileClips[TILE_VOID].y = 144;
			tileClips[TILE_VOID].w = TILE_LENG;
			tileClips[TILE_VOID].h = TILE_LENG;

			tileClips[TILE_WALL].x = 128;
			tileClips[TILE_WALL].y = 64;
			tileClips[TILE_WALL].w = TILE_LENG;
			tileClips[TILE_WALL].h = TILE_LENG;

			tileClips[TILE_ROAD].x = 0;
			tileClips[TILE_ROAD].y = 0;
			tileClips[TILE_ROAD].w = TILE_LENG;
			tileClips[TILE_ROAD].h = TILE_LENG;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}


int main(int argc, char* argv[])
{
	if (init() == false)
	{
		return -1;
	}

	SDL_Window* mWindow = NULL;
	mWindow = SDL_CreateWindow("Dun Dungeon Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (mWindow == NULL)
	{
		std::cout << "Window could not be created! SDL Error: %s\n" << SDL_GetError();
		return -1;
	}

	SDL_Renderer* mRenderer = NULL;
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError();
		return -1;
	}

	Player mPlayer(7, 4);
	if (mPlayer.loadFromFile("assests/sprite/warrior1.png", mRenderer) == false)
	{
		std::cout << "Failed to load sprite sheet texture!\n";
	}
	else
	{
		mPlayer.setClip(0, 15, PLAYER_WIDTH, PLAYER_HEIGHT);
	}

	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


	Tile*** tileSet = new Tile** [LEVEL_HEIGHT_CELL];
	for (int i = 0; i < LEVEL_HEIGHT_CELL; ++i)
	{
		tileSet[i] = new Tile *[LEVEL_WIDTH_CELL];
	}
	LTexture tileTexture;
	if (tileTexture.loadFromFile("assests/map/tiles_sewers.png", mRenderer) == false)
	{
		std::cout << "Failed to load tile set texture!\n";
	}
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	if (setTiles(tileSet, tileClips) == false)
	{
		std::cout << "Failed to load tile set!\n";
	}

	SDL_Event e;
	bool quit = false;

	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			mPlayer.handleEvent(e, tileSet);
		}

		mPlayer.setCamera(camera);

		SDL_SetRenderDrawColor(mRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(mRenderer);

		for (int i = 0; i < LEVEL_HEIGHT_CELL; ++i)
		{
			for (int j = 0; j < LEVEL_WIDTH_CELL; ++j)
			{
				tileSet[i][j]->render(camera, tileTexture, tileClips[tileSet[i][j]->getType()], mRenderer);
			}
		}

		mPlayer.render(mRenderer, camera.x, camera.y);

		SDL_RenderPresent(mRenderer);
	}

	for (int i = 0; i < LEVEL_HEIGHT_CELL; ++i)
	{
		for (int j = 0; j < LEVEL_WIDTH_CELL; ++j)
		{
			if (tileSet[i][j] != NULL)
			{
				delete tileSet[i][j];
				tileSet[i][j] = NULL;
			}
		}
		delete[] tileSet[i];
		tileSet[i] = NULL;
	}
	delete[] tileSet;
	tileSet = NULL;

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	close();

	return 0;
}