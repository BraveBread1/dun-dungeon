#include"Player.h"

Player::Player(int i, int j)
{
	mPosI = i;
	mPosJ = j;

	this->maxHP = 20;
	this->currentHp = 20;
	this->minDame = 1;
	this->maxDame = 10;
	turn = 1;

	exp = 0;
	maxExp = 10;
	level = 1;
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
		if (facing == RIGHT)
		{
			player.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 6, screen, &clip, scale);
		}
		else if (facing == LEFT)
		{
			player.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 6, screen, &clip, scale,1, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
		if (time > ATTACK_TIME)
		{
			setCrStatus(0);
		}
	}
	else if (crStatus == MOVING)
	{
		setClip(((time / 120) % 4) * 12 + 24, 15, 12, 15);
		{
			player.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 6, screen, &clip, scale);
		}
		if (time > MOVE_TIME)
		{
			setCrStatus(0);
		}
	}
}

//void Player::renderHp(SDL_Renderer* screen, SDL_FRect& camera, float scale)
//{
//	int hpPercent = (1.0 * currentHp / maxHP) * 16;
//
//	setGreenHpClip(0, 0, hpPercent, 2);
//	redHp.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 8, screen, &redHpClip, scale);
//	greenHp.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 8, screen, &greenHpClip, scale);
//}

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
}

//void Player::setGreenHpClip(int x, int y, int w, int h)
//{
//	greenHpClip.x = x;
//	greenHpClip.y = y;
//	greenHpClip.w = w;
//	greenHpClip.h = h;
//}
//
//void Player::setRedHpClip(int x, int y, int w, int h)
//{
//	redHpClip.x = x;
//	redHpClip.y = y;
//	redHpClip.w = w;
//	redHpClip.h = h;
//}
//
//bool Player::loadHpTexture(std::string path1, std::string path2, SDL_Renderer* screen)
//{
//	bool success = true;
//	if (greenHp.loadFromFile(path1, screen) == false) success = false;
//	setGreenHpClip(0, 0, 16, 2);
//	if (redHp.loadFromFile(path2, screen) == false) success = false;
//	setRedHpClip(0, 0, 16, 2);
//	return success;
//}

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
	statusPane.render(0, 632, screen, &statusPaneClip, 1, 3);
	statusPane.render(90, 689, screen, &hpClip, 1, 3);
	statusPane.render(90, 722, screen, &expClip, 1, 3);
	avata.render(25, 656, screen, &avataClip, 1, 3);
	lvText.render(screen);
	hpText.render(screen);
	expText.render(screen);
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

bool Player::loadStatusTexture(std::string path, SDL_Renderer* screen, TTF_Font * font)
{
	bool success = true;
	if (status.loadImg(path, screen) == false)
	{
		success = false;
	}
	if (status.loadText(level, currentHp, maxHP, exp, maxExp, screen, font) == false)
	{
		
		success = false;
	}
	return success;
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

bool hero_status::loadText(int lv, int hp, int maxHp, int exp, int maxExp, SDL_Renderer* screen, TTF_Font* font)
{

	bool success = true;
	SDL_Color textColor = { 255, 255, 0 };
	std::string lvt = "lv. " + std::to_string(lv);
	lvText.initText(28, 720, 20, textColor);
	if (!lvText.loadText(lvt, font, screen, 300))
	{
		std::cerr << "Failed to render text texture!\n";
		success = false;
	}
	textColor = { 255,255,255 };
	std::string hpt = std::to_string(hp) + "/" + std::to_string(maxHp);
	hpText.initText(250, 690, 20, textColor);
	if (!hpText.loadText(hpt, font, screen, 300))
	{
		std::cerr << "Failed to render text texture!\n";
		success = false;
	}
	std::string expt = std::to_string(exp) + "/" + std::to_string(maxExp);
	expText.initText(250, 720, 20, textColor);
	if (!expText.loadText(expt, font, screen, 300))
	{
		std::cerr << "Failed to render text texture!\n";
		success = false;
	}
	return success;
}

void Player::earnExp(int e)
{
	this->exp += e;
}

void Player::update(SDL_Renderer* screen, TTF_Font* font)
{
	if (exp >= maxExp)
	{
		level += 1;
		exp -= maxExp;
		maxExp += 5;
		maxHP += 5;
		minDame += 1;
		maxDame += 1;
	}
	status.update(level, currentHp, maxHP, exp, maxExp, screen, font);
}

void hero_status::update(int lv, int hp, int maxHp, int exp, int maxExp, SDL_Renderer* screen, TTF_Font* font)
{
	std::string lvt = "lv. " + std::to_string(lv);
	if (lvText.loadText(lvt, font, screen, 300) == false)
	{
		std::cout << "loi update level text" << std::endl;
	}
	std::string hpt = std::to_string(hp) + "/" + std::to_string(maxHp);
	if (!hpText.loadText(hpt, font, screen, 300))
	{
		std::cerr << "Failed to load hp text\n";
	}
	std::string expt = std::to_string(exp) + "/" + std::to_string(maxExp);
	if (!expText.loadText(expt, font, screen, 300))
	{
		std::cerr << "Failed to update exp text!\n";
	}
}