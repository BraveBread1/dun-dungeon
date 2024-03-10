#include"Player.h"

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

void Player::move()
{
	mPosX += mVelX;

	if ((mPosX < 0) || (mPosX + getClip().w > LEVEL_WIDTH))
	{
		mPosX -= mVelX;
	}

	mPosY += mVelY;

	if ((mPosY < 0) || (mPosY + getClip().h > LEVEL_HEIGHT))
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