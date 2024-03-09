#include"CommonFunc.h"
#include"LTexture.h"

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

//bool loadMedia()
//{
//	bool success = true;
//}

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

	LTexture background;
	background.loadFromFile("assests/img/background.png", mRenderer);

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
		}

		SDL_SetRenderDrawColor(mRenderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(mRenderer);

		background.render(0, 0, mRenderer);

		SDL_RenderPresent(mRenderer);
	}

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	close();

	return 0;
}