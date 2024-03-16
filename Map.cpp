#include"Map.h"
#include"Player.h"

Map::Map(int row, int col, int layer)
{
	this->rows = row;
	this->cols = col;
	this->layers = layer;
	tileSet = new Tile***[layer];
}

Map::~Map()
{
	free();
}

void Map::free()
{
	if (tileClips != NULL)
	{
		delete[] tileClips;
		tileClips = NULL;
	}
	if (tileSet != NULL)
	{
		for (int l = 0; l < layers; ++l)
		{
			for (int i = 0; i < rows; ++i)
			{
				for (int j = 0; j < cols; ++j)
				{
					delete tileSet[l][i][j];
					tileSet[l][i][j] = NULL;
				}
				delete[] tileSet[l][i];
				tileSet[l][i] = NULL;
			}
			delete[] tileSet[l];
			tileSet[l] = NULL;
		}
		delete[] tileSet;
		tileSet = NULL;
	}
	tileTexture.free();
	rows = 0;
	cols = 0;
	layers = 0;
}

bool Map::setTiles(std::string* path)
{
	bool success = true;

	std::ifstream* in = new std::ifstream[layers];

	for (int l = 0; l < layers; ++l)
	{
		in[l].open(path[l].c_str());
		if (in[l].fail())
		{
			std::cerr << "unable to load layer " << l << std::endl;
			success = false;
		}
		else
		{
			tileSet[l] = new Tile ** [rows];
			for (int i = 0; i < rows; ++i)
			{
				tileSet[l][i] = new Tile*[cols];
				for (int j = 0; j < cols; ++j)
				{
					int tileType = -1;
					in[l] >> tileType;

					if (in[l].fail())
					{
						std::cerr << "Error loading map: Unexpected end of file!\n";;
						success = false;
					}
					if (tileType >= 0)
					{
						tileSet[l][i][j] = new Tile(j * TILE_SIZE, i * TILE_SIZE, tileType);
					}
					else {
						std::cerr << "Error loading map: Invalid tile type at %d!" << i * rows + j;
						success = false;
					}
					
				}
			}
		}
		in[l].close();
	}
	delete[]  in;
	in = NULL;
	return success;
}

bool Map::setTileClips(std::string path, SDL_Renderer* screen)
{
	bool success = true;
	if (!tileTexture.loadFromFile(path, screen))
	{
		std::cerr << "unable to load tile texture" << std::endl;
		success = false;
	}
	tileClips = new SDL_Rect[TOTAL_TILE_SPRITES];
	for (int i = 1; i < TOTAL_TILE_SPRITES; ++i)
	{
		tileClips[i].x = ((i - 1) % 16 ) * 16;
		tileClips[i].y = ((i - 1) / 16) * 16;
		tileClips[i].w = TILE_SIZE;
		tileClips[i].h = TILE_SIZE;
	}
	return success;
}

Tile**** Map::getTileSet()
{
	return tileSet;
}

LTexture Map::getTileTexture()
{
	return tileTexture;
}

void Map::renderLayer(SDL_FRect& camera, SDL_Renderer* screen, int layer, float scale)
{
		for (int i = 0; i < LEVEL1_ROWS; ++i)
		{
			for (int j = 0; j < LEVEL1_COLS; ++j)
			{
				if (tileSet[layer][i][j]->getType() != 0)
				{
					tileSet[layer][i][j]->render(camera, tileTexture, tileClips[tileSet[layer][i][j]->getType()], screen, scale);
				}
			}
		}
}