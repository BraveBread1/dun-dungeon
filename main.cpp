#include"CommonFunc.h"
#include"LTexture.h"
#include"Player.h"
#include"Tile.h"
#include"Map.h"

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: %s\n" << SDL_GetError();
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!";
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: %s\n" << IMG_GetError();
		success = false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError();
		success = false;
	}

	return success;
}

void close()
{
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
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

	Player mPlayer(6, 3);
	if (mPlayer.loadFromFile("assests/sprite/warrior.png", mRenderer) == false)
	{
		std::cout << "Failed to load sprite sheet texture!\n";
	}
	else
	{
		mPlayer.setClip(0, 15, PLAYER_WIDTH, PLAYER_HEIGHT);
	}

	SDL_FRect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	Map map1(LEVEL1_ROWS, LEVEL1_COLS, LEVEL1_LAYERS);
	std::string* layerPathLevel1 = new std::string[LEVEL1_LAYERS];
	layerPathLevel1[0] = "assests/map/tileMap/level1/layer1.map";
	layerPathLevel1[1] = "assests/map/tileMap/level1/layer2.map";
	layerPathLevel1[2] = "assests/map/tileMap/level1/layer3.map";

	if (map1.setTileClips("assests/map/tiles_sewers.png", mRenderer) == false)
	{
		std::cerr << "unable to set tile clips" << std::endl;
	}
	if (map1.setTiles(layerPathLevel1) == false)
	{
		std::cerr << "unable to load tile set" << std::endl;
	}

	Mix_Music* music = Mix_LoadMUS("assests/music/core_src_main_assets_music_sewers_1.ogg");

	if (music == NULL)
	{
		std::cout << "failed to load music\n";
	}

	Mix_PlayMusic(music, -1);

	SDL_Event e;
	bool quit = false;

	float scale = 1;

	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_MOUSEWHEEL)
			{
				if (e.wheel.y != 0)
				{
					scale += e.wheel.preciseY * 0.1;
				}
				if (scale < 1) scale = 1;
				else if (scale >= 2) scale = 2;
			}

			mPlayer.handleEvent(e, map1.tileSet);
		}
		mPlayer.setCamera(camera, scale);



		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, RENDER_DRAW_COLOR);
		SDL_RenderClear(mRenderer);

		for (int i = 0; i < LEVEL1_LAYERS; ++i)
		{
			map1.renderLayer(camera, mRenderer, i, scale);
			if(i == 0) mPlayer.render(mRenderer, camera, scale);
		}

		SDL_RenderPresent(mRenderer);
	}

	Mix_FreeMusic(music);
	music = NULL;
	
	map1.free();

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	close();

	return 0;
}