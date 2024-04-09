#include"EntityLayer.h"

EntityLayer::EntityLayer(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;
	head = NULL;
}

bool EntityLayer::setPosition(std::string path)
{
	bool success = true;
	std::ifstream in;
	in.open(path.c_str());
	if (in.fail())
	{
		std::cerr << "unable to load entity layer" << std::endl;
		success = false;
	}
	else
	{
		entSet = new int *[rows];
		for (int i = 0; i < rows; ++i)
		{
			entSet[i] = new int [cols];
			for (int j = 0; j < cols; ++j)
			{
				int entType = -1;
				in >> entType;
				if (in.fail())
				{
					std::cerr << "Error loading entect layer: Unexpected end of file" << std::endl;
					success = false;
				}
				if (entType >= 0)
				{
					entSet[i][j] = entType;
					if (entType > 0)
					{
						Entity* newEnt = new Entity(i, j, entType);
						addLast(head, newEnt);
					}

				}
				else
				{
					std::cerr << "Error loading entect layer: Invalid tile type at %d!" << i * rows + j;
					success = false;
				}
			}
		}
		in.close();
	}
	return success;
}

void EntityLayer::free()
{
	if (entTexture != NULL)
	{
		for (int i = 0; i < MAX_MONSTER_TYPE; ++i)
		{
			entTexture->free();
		}
		delete[] entTexture;
		entTexture = NULL;
	}

	if (entSet != NULL)
	{
		for (int i = 0; i < rows; ++i)
		{
			delete[] entSet[i];
			entSet[i] = NULL;
		}
		delete[] entSet;
		entSet = NULL;
	}

}

EntityLayer::~EntityLayer()
{
	free();
}

bool EntityLayer::loadEntTextureSet(std::string* path, SDL_Renderer* screen)
{
	bool success = true;
	entTexture = new LTexture[MAX_MONSTER_TYPE];
	for (int i = 0; i < MAX_MONSTER_TYPE; ++i)
	{
		if (entTexture[i].loadFromFile(path[i], screen) == false)
		{
			std::cerr << "unable to load entity texture" << std::endl;
			success = false;
		}
	}
	return success;
}

void EntityLayer::render(SDL_FRect& camera, SDL_Renderer* screen, float scale)
{
	Entity* p = head;
	while (p != NULL)
	{
		if (p->getType() != 0)
		{
			p->render(camera, screen, entTexture[p->getType() - 1], greenHp, redHp, scale);
		}
		p = p->next;
	}
}

int** EntityLayer::getEntSet()
{
	return entSet;
}

void addLast(Entity* &a, Entity* tmp)
{
	if (a == NULL) {
		a = tmp;
	}
	else {
		Entity* trc = NULL;
		Entity* p = a;
		while (p != NULL) {
			trc = p;
			p = p->next;
		}
		trc->next = tmp;
	}
}

Entity* EntityLayer::checkEntCollision(int i, int j)
{
	Entity* currentMonster = head;
	while (currentMonster != NULL)
	{
		if (currentMonster->getPosI() == i && currentMonster->getPosJ() == j)
		{
			return currentMonster;
		}
		else
		{
			currentMonster = currentMonster->next;
		}
	}

	return NULL;
}

bool EntityLayer::loadHpTexture(std::string path1, std::string path2, SDL_Renderer* screen)
{
	bool success = true;
	if (greenHp.loadFromFile(path1, screen) == false) success = false;
	if (redHp.loadFromFile(path2, screen) == false) success = false;
	return success;
}