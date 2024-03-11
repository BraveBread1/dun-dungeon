#include"Player.h"


Player::Player(int i, int j)
{
	mPosI = i;
	mPosJ = j;
}

Player::~Player()
{
	FreePlayer();
}

void Player::FreePlayer()
{
	LTexture::free();
	mPosI = 0;
	mPosJ = 0;
}

void Player::handleEvent(SDL_Event& e, Tile*** tiles)
{
	int i = mPosI;
	int j = mPosJ;
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: i -= 1; break;
		case SDLK_DOWN: i += 1; break;
		case SDLK_LEFT: j -= 1; break;
		case SDLK_RIGHT: j += 1; break;
		}
	}

	bool isPath = true;
	if (i < 0 || i >= LEVEL_HEIGHT_CELL) isPath = false;
	else if (j < 0 || j >= LEVEL_HEIGHT) isPath = false;
	else if (tiles[i][j]->getType() != TILE_ROAD) isPath = false;

	if (isPath)
	{
		mPosI = i;
		mPosJ = j;
	}
}

void Player::render(SDL_Renderer* screen, int camX, int camY)
{
	LTexture::render(mPosJ * TILE_LENG - camX, mPosI * TILE_LENG  - camY, screen, &clip);
}

void Player::setClip(int x, int y, int w, int h)
{
	clip.x = x;
	clip.y = y;
	clip.w = w;
	clip.h = h;
}

SDL_Rect Player::getClip()
{
	return clip;
}

int Player::getPosI()
{
	return mPosI;
}

int Player::getPosJ()
{
	return mPosJ;
}

void Player::setCamera(SDL_Rect& camera)
{
	camera.x = (mPosJ * TILE_LENG + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (mPosI * TILE_LENG + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}