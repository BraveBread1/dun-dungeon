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
		case SDLK_LEFT: j -= 1; break;
		case SDLK_RIGHT: j += 1; break;
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

void Player::render(SDL_Renderer* screen, SDL_FRect& camera, float scale)
{
	player.render(mPosJ * TILE_SIZE - camera.x, mPosI * TILE_SIZE - camera.y - 6, screen, &clip, scale);
	renderHp(screen, camera, scale);
}

void Player::renderHp(SDL_Renderer* screen, SDL_FRect& camera, float scale)
{
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
	// Tính toán vị trí trung tâm của nhân vật dựa trên tỷ lệ zoom
	float playerCenterX = (mPosJ * TILE_SIZE + PLAYER_WIDTH / 2) * scale;
	float playerCenterY = (mPosI * TILE_SIZE + PLAYER_HEIGHT / 2) * scale;

	// Điều chỉnh camera để nhân vật ở giữa màn hình, tính theo tỷ lệ zoom
	camera.x = playerCenterX - SCREEN_WIDTH / 2.0f;
	camera.y = playerCenterY - SCREEN_HEIGHT / 2.0f;

	// Đảm bảo camera không đi ra ngoài giới hạn của bản đồ
	camera.x = fmax(0.0f, camera.x);
	camera.y = fmax(0.0f, camera.y);

	// Tính toán giới hạn tối đa của camera dựa trên kích thước của bản đồ
	float maxCameraX = LEVEL_WIDTH * scale - SCREEN_WIDTH;
	float maxCameraY = LEVEL_HEIGHT * scale - SCREEN_HEIGHT;

	// Đảm bảo camera không vượt quá biên của bản đồ
	camera.x = fmin(camera.x, maxCameraX);
	camera.y = fmin(camera.y, maxCameraY);

	// Xử lý trường hợp đặc biệt khi kích thước camera lớn hơn kích thước bản đồ
	if (SCREEN_WIDTH >= LEVEL_WIDTH * scale)
	{
		camera.x = (LEVEL_WIDTH * scale - SCREEN_WIDTH) / 2.0f; // Giữ camera ở giữa theo chiều ngang
	}

	if (SCREEN_HEIGHT >= LEVEL_HEIGHT * scale)
	{
		camera.y = (LEVEL_HEIGHT * scale - SCREEN_HEIGHT) / 2.0f; // Giữ camera ở giữa theo chiều dọc
	}
}

bool Player::loadPlayerTexture(std::string path, SDL_Renderer* screen)
{
	if (player.loadFromFile(path, screen))
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
	target->attacked(dame);
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