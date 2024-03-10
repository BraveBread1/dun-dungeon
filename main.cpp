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

bool setTiles(Tile* tiles[], SDL_Rect tileClips[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("assests/map/lazy.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			tileClips[TILE_RED].x = 0;
			tileClips[TILE_RED].y = 0;
			tileClips[TILE_RED].w = TILE_WIDTH;
			tileClips[TILE_RED].h = TILE_HEIGHT;

			tileClips[TILE_GREEN].x = 0;
			tileClips[TILE_GREEN].y = 80;
			tileClips[TILE_GREEN].w = TILE_WIDTH;
			tileClips[TILE_GREEN].h = TILE_HEIGHT;

			tileClips[TILE_BLUE].x = 0;
			tileClips[TILE_BLUE].y = 160;
			tileClips[TILE_BLUE].w = TILE_WIDTH;
			tileClips[TILE_BLUE].h = TILE_HEIGHT;

			tileClips[TILE_TOPLEFT].x = 80;
			tileClips[TILE_TOPLEFT].y = 0;
			tileClips[TILE_TOPLEFT].w = TILE_WIDTH;
			tileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

			tileClips[TILE_LEFT].x = 80;
			tileClips[TILE_LEFT].y = 80;
			tileClips[TILE_LEFT].w = TILE_WIDTH;
			tileClips[TILE_LEFT].h = TILE_HEIGHT;

			tileClips[TILE_BOTTOMLEFT].x = 80;
			tileClips[TILE_BOTTOMLEFT].y = 160;
			tileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
			tileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

			tileClips[TILE_TOP].x = 160;
			tileClips[TILE_TOP].y = 0;
			tileClips[TILE_TOP].w = TILE_WIDTH;
			tileClips[TILE_TOP].h = TILE_HEIGHT;

			tileClips[TILE_CENTER].x = 160;
			tileClips[TILE_CENTER].y = 80;
			tileClips[TILE_CENTER].w = TILE_WIDTH;
			tileClips[TILE_CENTER].h = TILE_HEIGHT;

			tileClips[TILE_BOTTOM].x = 160;
			tileClips[TILE_BOTTOM].y = 160;
			tileClips[TILE_BOTTOM].w = TILE_WIDTH;
			tileClips[TILE_BOTTOM].h = TILE_HEIGHT;

			tileClips[TILE_TOPRIGHT].x = 240;
			tileClips[TILE_TOPRIGHT].y = 0;
			tileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			tileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			tileClips[TILE_RIGHT].x = 240;
			tileClips[TILE_RIGHT].y = 80;
			tileClips[TILE_RIGHT].w = TILE_WIDTH;
			tileClips[TILE_RIGHT].h = TILE_HEIGHT;

			tileClips[TILE_BOTTOMRIGHT].x = 240;
			tileClips[TILE_BOTTOMRIGHT].y = 160;
			tileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			tileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
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

	Player mPlayer;
	if (mPlayer.loadFromFile("assests/sprite/avatars.png", mRenderer) == false)
	{
		std::cout << "Failed to load sprite sheet texture!\n";
	}
	else
	{
		mPlayer.setClip(0, 0, mPlayer.getWidth() / 5, mPlayer.getHeight());
	}

	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	Tile* tileSet[TOTAL_TILES];
	LTexture tileTexture;
	if (tileTexture.loadFromFile("assests/map/tiles.png", mRenderer) == false)
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

			mPlayer.handleEvent(e);
		}

		mPlayer.move(tileSet);
		mPlayer.setCamera(camera);

		SDL_SetRenderDrawColor(mRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(mRenderer);

		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			tileSet[i]->render(camera, tileTexture, tileClips[tileSet[i]->getType()], mRenderer);
		}

		mPlayer.render(mRenderer, camera.x, camera.y);

		SDL_RenderPresent(mRenderer);
	}

	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tileSet[i] != NULL)
		{
			delete tileSet[i];
			tileSet[i] = NULL;
		}
	}

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	close();

	return 0;
}