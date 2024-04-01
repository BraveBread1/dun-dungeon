#include"ObjectLayer.h"

ObjectLayer::ObjectLayer(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;
}

void ObjectLayer::free()
{
	objTexture.free();
	if (objSet != NULL)
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				delete objSet[i][j];
				objSet[i][j] = NULL;
			}
			delete[] objSet[i];
			objSet[i] = NULL;
		}
		delete[] objSet;
		objSet = NULL;
	}
	if (objClips != NULL)
	{
		delete[] objClips;
		objClips = NULL;
	}
}

ObjectLayer::~ObjectLayer()
{
	free();
}

bool ObjectLayer::setPosition(std::string path)
{
	bool success = true;
	std::ifstream in;
	in.open(path.c_str());
	if (in.fail())
	{
		std::cerr << "unable to load object layer" << std::endl;
		success = false;
	}
	else
	{
		objSet = new Object **[rows];
		for (int i = 0; i < rows; ++i)
		{
			objSet[i] = new Object * [cols];
			for (int j = 0; j < cols; ++j)
			{
				int objType = -1;
				in >> objType;
				if (in.fail())
				{
					std::cerr << "Error loading object layer: Unexpected end of file" << std::endl;
					success = false;
				}
				if (objType >= 0)
				{
					objSet[i][j] = new Object(i, j, objType);
				}
				else
				{
					std::cerr << "Error loading object layer: Invalid tile type at %d!" << i * rows + j;
					success = false;
				}
			}
		}
		in.close();
	}
	return success;
}

bool ObjectLayer::setObjClips(std::string path, SDL_Renderer* screen)
{
	bool success = true;
	if (!objTexture.loadFromFile(path, screen))
	{
		std::cerr << "unable to load object texture" << std::endl;
		success = false;
	}
	objClips = new SDL_Rect[TOTAL_TILE_SPRITES];
	for (int i = 1; i < TOTAL_TILE_SPRITES; ++i)
	{
		objClips[i].x = ((i - 1) % 16) * 16;
		objClips[i].y = ((i - 1) / 16) * 16;
		objClips[i].w = TILE_SIZE;
		objClips[i].h = TILE_SIZE;
	}
	return success;
}

void ObjectLayer::render(SDL_FRect& camera, SDL_Renderer* screen, float scale)
{
	for (int i = 0; i < LEVEL1_ROWS; ++i)
	{
		for (int j = 0; j < LEVEL1_COLS; ++j)
		{
			if (objSet[i][j]->getType() != 0)
			{
				objSet[i][j]->render(camera, objTexture, objClips[objSet[i][j]->getType()], screen, scale);
			}
		}
	}
}

Object*** ObjectLayer::getObjSet()
{
	return objSet;
}