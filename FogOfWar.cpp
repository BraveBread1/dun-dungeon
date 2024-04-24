#include"FogOfWar.h"

FogOfWar::FogOfWar()
{
	fogMap = new int* [LEVEL1_ROWS];
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		fogMap[i] = new int[LEVEL1_COLS];
		for (int j = 0; j < LEVEL1_COLS; ++j)
		{
			fogMap[i][j] = 0;
		}
	}

	hasSolid = new bool* [LEVEL1_ROWS];
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		hasSolid[i] = new bool[LEVEL1_COLS];
		for (int j = 0; j < LEVEL1_COLS; ++j)
		{
			hasSolid[i][j] = 0;
		}
	}
}

FogOfWar::~FogOfWar()
{
	free();
}

void FogOfWar::free()
{
	BlackTexture.free();
	if (hasSolid != NULL)
	{
		for (int i = 0; i < LEVEL1_ROWS; ++i)
		{
			delete[] hasSolid[i];
			hasSolid[i] = NULL;
		}
		delete[] hasSolid;
		hasSolid = NULL;
	}
	if (fogMap != NULL)
	{
		for (int i = 0; i < LEVEL1_ROWS; ++i)
		{
			delete[] fogMap[i];
			fogMap[i] = NULL;
		}
		delete[] fogMap;
		fogMap = NULL;
	}

}

bool FogOfWar::hasLOS(int i1, int j1, int i2, int j2)
{
	int di, dj, si, sj, err, e2;

	di = abs(i2 - i1);
	dj = abs(j2 - j1);

	si = (i1 < i2) ? 1 : -1;
	sj = (j1 < j2) ? 1 : -1;
	err = dj - di;

	while (1)
	{
		e2 = 2 * err;

		if (e2 > -di)
		{
			err -= di;
			j1 += sj;
		}

		if (e2 < dj)
		{
			err += dj;
			i1 += si;
		}

		if (j1 == j2 && i1 == i2)
		{
			return 1;
		}

		if (hasSolid[i1][j1])
		{
			return 0;
		}

	}
	return 0;
}

void FogOfWar::updateSolid(Tile**** tileSet, Object*** objSet)
{
	int j, i;
	for (i = 0; i < LEVEL1_ROWS; i++)
	{
		for (j = 0; j < LEVEL1_COLS; j++)
		{
			hasSolid[i][j] = 0;
			if (fogMap[i][j] == 2)
			{
				fogMap[i][j] = 1;
			}
		}
	}

	for (int l = 0; l < LEVEL1_LAYERS; ++l)
	{
		for (i = 0; i < LEVEL1_ROWS; i++)
		{
			for (j = 0; j < LEVEL1_COLS; j++)
			{
				if (tileSet[l][i][j]->getSolid())
				{
					hasSolid[i][j] = 1;
				}
			}
		}
	}
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		for (int j = 0; j < LEVEL1_COLS; ++j)
		{
			if (objSet[i][j]->getType() == 216 || objSet[i][j]->getType() == 113)
			{
				hasSolid[i][j] = 1;
			}
		}
	}
}

bool FogOfWar::loadFogTexture(std::string path, SDL_Renderer* screen)
{
	bool success = true;
	if (!BlackTexture.loadFromFile(path, screen))
	{
		success = false;
	}

	BlackTexture.setBlendMode(SDL_BLENDMODE_BLEND);

	return success;
}

void FogOfWar::update(int pi, int pj)
{
	int mi, mj;
	int i, j;

	for (i = -VIS_DISTANCE; i <= VIS_DISTANCE; i++)
	{
		for (j = -VIS_DISTANCE; j <= VIS_DISTANCE; j++)
		{
			mi = pi + i;
			mj = pj + j;
			//if (getDistance(pj, pi, mj, mi) <= VIS_DISTANCE)
			//{
				if (mi >= 0 && mj >= 0 && mj < LEVEL1_COLS && mi < LEVEL1_ROWS)
				{
					if (fogMap[mi][mj] != 2 && hasLOS(pi, pj, mi, mj))
					{
						fogMap[mi][mj] = 2;
					}
				}
			//}
		}
	}
}


void FogOfWar::render(SDL_FRect& camera, SDL_Renderer* screen, float scale)
{
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		for (int j = 0; j < LEVEL1_COLS; ++j)
		{
			SDL_FRect mBox(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			
			if (checkCollision(camera, mBox))
			{
				Uint8 a = FULL_FOG;
				if (fogMap[i][j] == 0)
				{
					//if (i != 0)//phia tren
					//{
					//	if (fogMap[i - 1][j] == 2)
					//	{
					//		a = NONE_FOG;
					//		setPartClip(0, 0, 16, 1);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
					//		a = HALF_FOG;
					//		setPartClip(0, 1, 16, 1);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y + 1, screen, &partClip, scale);
					//	}
					//	if (fogMap[i - 1][j] == 1)
					//	{
					//		a = HALF_FOG;
					//		setPartClip(0, 0, 16, 2);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//	else
					//	{
					//		a = FULL_FOG;
					//		setPartClip(0, 0, 16, 2);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//}
					//if (i < LEVEL1_ROWS)//phia duoi
					//{
					//	if (fogMap[i + 1][j] == 2)
					//	{
					//		a = NONE_FOG;
					//		setPartClip(0, 15, 16, 1);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y + 14, screen, &partClip, scale);
					//		a = HALF_FOG;
					//		setPartClip(0, 14, 16, 1);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y + 15, screen, &partClip, scale);
					//	}
					//	if (fogMap[i + 1][j] == 1)
					//	{
					//		a = HALF_FOG;
					//		setPartClip(0, 14, 16, 2);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y + 14, screen, &partClip, scale);
					//	}
					//	else
					//	{
					//		a = FULL_FOG;
					//		setPartClip(0, 14, 16, 2);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y + 14, screen, &partClip, scale);
					//	}
					//}
					//if (j > 0)//trai
					//{
					//	if (fogMap[i][j - 1] == 2)
					//	{
					//		a = NONE_FOG;
					//		setPartClip(0, 0, 1, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
					//		a = HALF_FOG;
					//		setPartClip(1, 0, 1, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x + 1, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//	if (fogMap[i][j - 1] == 1)
					//	{
					//		a = HALF_FOG;
					//		setPartClip(0, 0, 2, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//	else
					//	{
					//		a = FULL_FOG;
					//		setPartClip(0, 0, 2, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//}
					//if (j < LEVEL1_COLS)//phai
					//{
					//	if (fogMap[i][j + 1] == 2)
					//	{
					//		a = NONE_FOG;
					//		setPartClip(15, 0, 1, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x + 15, mBox.y - camera.y, screen, &partClip, scale);
					//		a = HALF_FOG;
					//		setPartClip(14, 0, 1, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x + 14, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//	if (fogMap[i][j + 1] == 1)
					//	{
					//		a = HALF_FOG;
					//		setPartClip(14, 0, 2, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x + 14, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//	else
					//	{
					//		a = FULL_FOG;
					//		setPartClip(14, 0, 2, 16);
					//		BlackTexture.setAlpha(a);
					//		BlackTexture.render(mBox.x - camera.x + 14, mBox.y - camera.y, screen, &partClip, scale);
					//	}
					//}

					//a = FULL_FOG;
					//setPartClip(2, 2, 14, 14);
					//BlackTexture.setAlpha(a);
					//BlackTexture.render(mBox.x - camera.x + 2, mBox.y - camera.y + 2, screen, &partClip, scale);
					a = FULL_FOG;
					setPartClip(0, 0, 16, 16);
					BlackTexture.setAlpha(a);
					BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);

				}
				else if (fogMap[i][j] == 1)
				{
					a = HALF_FOG;
					setPartClip(0, 0, 16, 16);
					BlackTexture.setAlpha(a);
					BlackTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &partClip, scale);
				}
			}
		}
	}
}

void FogOfWar::setPartClip(int x, int y, int w, int h)
{
	partClip.x = x;
	partClip.y = y;
	partClip.w = w;
	partClip.h = h;
}