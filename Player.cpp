#include"Player.h"

bool touchesWall(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}


Player::Player()
{
	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;
}

Player::~Player()
{
	FreePlayer();
}

void Player::FreePlayer()
{
	LTexture::free();
	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;
}

void Player::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= PLAYER_VEL; break;
		case SDLK_DOWN: mVelY += PLAYER_VEL; break;
		case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
		case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += PLAYER_VEL; break;
		case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
		case SDLK_LEFT: mVelX += PLAYER_VEL; break;
		case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
		}
	}
}

void Player::move(Tile* tiles[])
{
	mPosX += mVelX;

	if ((mPosX < 0) || (mPosX + getClip().w > LEVEL_WIDTH) || touchesWall(clip, tiles))
	{
		mPosX -= mVelX;
	}

	mPosY += mVelY;

	if ((mPosY < 0) || (mPosY + getClip().h > LEVEL_HEIGHT) || touchesWall(clip, tiles))
	{
		mPosY -= mVelY;
	}
}

void Player::render(SDL_Renderer* screen, int camX, int camY)
{
	LTexture::render(mPosX - camX, mPosY - camY, screen, &clip);
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

int Player::getPosX()
{
	return mPosX;
}

int Player::getPosY()
{
	return mPosY;
}

void Player::setCamera(SDL_Rect& camera)
{
	camera.x = (mPosX + (getWidth() / 5) / 2) - SCREEN_WIDTH / 2;
	camera.y = (mPosY + getHeight() / 2) - SCREEN_HEIGHT / 2;

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