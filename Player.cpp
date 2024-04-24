#include"Player.h"

Player::Player(int i, int j)
{
	mPosI = i;
	mPosJ = j;

	this->maxHP = 20;
	this->currentHp = 20;
	this->minDame = 1;
	this->maxDame = 2;
	turn = 1;

	exp = 3;
	maxExp = 10;
	crStatus = NO_ATTACK;

}

Player::~Player()
{
	FreePlayer();
}

void Player::FreePlayer()
{
	player.free();
	mPosI = 0;
	mPosJ = 0;
}

void Player::handleEvent(SDL_Event& e, Tile**** tiles)
{
	int i = mPosI;
	int j = mPosJ;

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: i -= 1; break;
		case SDLK_DOWN: i += 1; break;
		case SDLK_LEFT: 
		{
			facing = LEFT;
			j -= 1; break;
		}
		case SDLK_RIGHT: 
		{
			facing = RIGHT;
			j += 1; break;
		}
		}
	}

	bool isPath = true;
	if (tiles[0][i][j]->getType() > 21 || tiles[0][i][j]->getType() < 1) isPath = false;

	if (isPath)
	{
		mPosI = i;
		mPosJ = j;
	}
}

void Player::render(SDL_Renderer* screen, SDL_FRect& camera, Uint32 time, float scale)
{
	if (crStatus == NO_ATTACK)
	{
		if (facing == RIGHT)
		{
			setClip(0, 15, 12, 15);
		}
		else if (facing == LEFT)
		{
			setClip(12, 15, 12, 15);
		}
		player.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 6, screen, &clip, scale);
	}
	else if (crStatus == ATTACK)
	{
		setClip(((time / 120) % 3) * 12 + 156, 15, 12, 15);
		player.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 6, screen, &clip, scale);
		if (time > ATTACK_TIME)
		{
			setCrStatus(0);
		}
	}
}

void Player::renderHp(SDL_Renderer* screen, SDL_FRect& camera, float scale)
{
	int hpPercent = (1.0 * currentHp / maxHP) * 16;

	setGreenHpClip(0, 0, hpPercent, 2);
	redHp.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 8, screen, &redHpClip, scale);
	greenHp.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 8, screen, &greenHpClip, scale);
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

void Player::setCamera(SDL_FRect& camera, float scale)
{
	float pCenterX = (mPosJ * TILE_SIZE + TILE_SIZE / 2) * scale;
	float pCenterY = (mPosI * TILE_SIZE + TILE_SIZE / 2) * scale;

	camera.x = (pCenterX - SCREEN_WIDTH / 2) / scale;
	camera.y = (pCenterY - SCREEN_HEIGHT / 2) / scale;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x + SCREEN_WIDTH > 1.0 * LEVEL_WIDTH * scale) camera.x = LEVEL_WIDTH * scale - SCREEN_WIDTH;
	if (camera.y + SCREEN_HEIGHT > 1.0 * LEVEL_HEIGHT * scale) camera.y = LEVEL_HEIGHT * scale - SCREEN_HEIGHT;
	if (SCREEN_WIDTH > LEVEL_WIDTH * scale)
	{
		camera.x = (LEVEL_WIDTH * scale) / 2 - SCREEN_WIDTH / 2;
	}
	if (SCREEN_HEIGHT > LEVEL_HEIGHT * scale)
	{
		camera.x = (LEVEL_HEIGHT * scale) / 2 - SCREEN_HEIGHT / 2;
	}
}

bool Player::loadPlayerTexture(std::string path, SDL_Renderer* screen)
{
	if (player.loadFromFile(path, screen) && status.loadImg("assests/img/hero_status.png", screen))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::setPosI(int i)
{
	mPosI = i;
}

void Player::setPosJ(int j)
{
	mPosJ = j;
}

void Player::attack(Entity* target)
{
	int dame = rand() % maxDame + minDame;
	target->attacked(dame);
	crStatus = ATTACK;
}

void Player::setGreenHpClip(int x, int y, int w, int h)
{
	greenHpClip.x = x;
	greenHpClip.y = y;
	greenHpClip.w = w;
	greenHpClip.h = h;
}

void Player::setRedHpClip(int x, int y, int w, int h)
{
	redHpClip.x = x;
	redHpClip.y = y;
	redHpClip.w = w;
	redHpClip.h = h;
}

bool Player::loadHpTexture(std::string path1, std::string path2, SDL_Renderer* screen)
{
	bool success = true;
	if (greenHp.loadFromFile(path1, screen) == false) success = false;
	setGreenHpClip(0, 0, 16, 2);
	if (redHp.loadFromFile(path2, screen) == false) success = false;
	setRedHpClip(0, 0, 16, 2);
	return success;
}

void Player::attacked(int dame)
{
	this->currentHp -= dame;
	if (currentHp <= 0) currentHp = 0;
}

void Player::setFacing(int direct)
{
	switch (direct)
	{
	case 0:
		facing = LEFT;
		break;
	case 1:
		facing = RIGHT;
		break;
	}
}

void Player::setTurn(int t)
{
	turn = t;
}

int Player::getTurn()
{
	return turn;
}

hero_status::hero_status()
{
	statusPaneClip.x = 0;
	statusPaneClip.y = 0;
	statusPaneClip.w = 159;
	statusPaneClip.h = 39;

	hpClip.x = 0;
	hpClip.y = 39;
	hpClip.w = 128;
	hpClip.h = 9;

	expClip.x = 0;
	expClip.y = 57;
	expClip.w = 128;
	expClip.h = 7;

	avataClip.x = 0;
	avataClip.y = 15;
	avataClip.w = 12;
	avataClip.h = 15;
}

hero_status::~hero_status()
{
	free();
}

void hero_status::free()
{
	statusPane.free();
}

bool hero_status::loadImg(std::string path, SDL_Renderer* screen)
{
	return statusPane.loadFromFile(path, screen) && avata.loadFromFile("assests/sprite/warrior.png", screen);
}

void hero_status::render(SDL_Renderer* screen, int hpPercnt, int expPercent)
{
	setHpClip(hpPercnt);
	setExpClip(expPercent);
	statusPane.render(0, LEVEL_HEIGHT - 104, screen, &statusPaneClip, 1, 3);
	statusPane.render(90, LEVEL_HEIGHT - 47, screen, &hpClip, 1, 3);
	statusPane.render(90, LEVEL_HEIGHT - 14, screen, &expClip, 1, 3);
	avata.render(25, LEVEL_HEIGHT - 80, screen, &avataClip, 1, 3);
}

void hero_status::setHpClip(int w)
{
	hpClip.x = 0;
	hpClip.y = 39;
	hpClip.w = w;
	hpClip.h = 9;
}

void hero_status::setExpClip(int w)
{
	expClip.x = 0;
	expClip.y = 57;
	expClip.w = w;
	expClip.h = 7;
}

bool Player::loadStatusTexture(std::string path, SDL_Renderer* screen)
{
	return status.loadImg(path, screen);
}

void Player::renderStatus(SDL_Renderer* screen)
{
	int hpPercent = (1.0 * currentHp / maxHP) * 128;
	int expPercent = (1.0 * exp / maxExp) * 128;
	status.render(screen, hpPercent, expPercent);
}

void Player::setCrStatus(int t)
{
	crStatus = t;
}