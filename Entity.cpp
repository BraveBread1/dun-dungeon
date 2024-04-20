#include"Entity.h"

Entity::Entity(int i, int j, int type)
{
	this->i = i;
	this->j = j;
	mType = type;

	//if (mType == 1)
	//{
		this->maxHP = 100;
		this->currentHp = this->maxHP;
		this->dame = 10;
		this->alert = false;
		hunt = 0;
	//}

		sawI = -1;
		sawJ = -1;

	entClip.x = 0;
	entClip.y = 0;
	entClip.w = TILE_SIZE;
	entClip.h = TILE_SIZE;

	mBox.x = j * TILE_SIZE;
	mBox.y = i * TILE_SIZE;
	mBox.w = TILE_SIZE;
	mBox.h = TILE_SIZE;

	greenHpClip.x = 0;
	greenHpClip.y = 0;
	greenHpClip.w = 16;
	greenHpClip.h = 2;

	redHpClip.x = 0;
	redHpClip.y = 0;
	redHpClip.w = 16;
	redHpClip.h = 2;

	next = NULL;
}

void Entity::freeEntity()
{
	if (next != NULL)
	{
		next = NULL;
	}
}

void Entity::render(SDL_FRect& camera, SDL_Renderer* screen, LTexture& entTexture, LTexture& greenHpTexture, LTexture& redHpTexture,Uint32 time , float scale)
{
	if (checkCollision(camera, mBox))
	{
		setBox();
		setEntClip((time / 100) % 11 * 16, 0);
		entTexture.render(mBox.x - camera.x, mBox.y - camera.y - 6, screen, &entClip, scale);
		renderHp(screen, greenHpTexture, redHpTexture, camera, scale);
	}
}

Entity::~Entity()
{
	freeEntity();
}

int Entity::getType()
{
	return mType;
}

SDL_FRect Entity::getBox()
{
	return mBox;
}

void Entity::setEntClip(int x, int y)
{
	entClip.x = x;
	entClip.y = y;
	entClip.w = 16;
	entClip.h = 16;
}

int Entity::getPosI()
{
	return i;
}

int Entity::getPosJ()
{
	return j;
}

void Entity::attacked(int dame)
{
	currentHp -= dame;
	if (currentHp <= 0)
	{
		currentHp = 0;
		dead = true;
	}
}

void Entity::setGreenHpClip(int x, int y, int w, int h)
{
	greenHpClip.x = x;
	greenHpClip.y = y;
	greenHpClip.w = w;
	greenHpClip.h = h;
}

void Entity::setRedHpClip(int x, int y, int w, int h)
{
	redHpClip.x = x;
	redHpClip.y = y;
	redHpClip.w = w;
	redHpClip.h = h;
}

void Entity::renderHp(SDL_Renderer* screen, LTexture& greenHpTexture, LTexture& redHpTexture, SDL_FRect& camera, float scale)
{
	int hpPercent = (1.0 * currentHp / maxHP) * 16;

	setGreenHpClip(0, 0, hpPercent, 2);
	redHpTexture.render(j * TILE_SIZE - camera.x, i * TILE_SIZE - camera.y - 8, screen, &redHpClip, scale);
	greenHpTexture.render(j * TILE_SIZE - camera.x, i * TILE_SIZE - camera.y - 8, screen, &greenHpClip, scale);
}

bool Entity::nextToPlayer(int pi, int pj)
{
	if (i == pi && j == pj) {
		std::cout << "the dao nao lai bang duoc" << std::endl;
		return false;
	}
	if (i == pi - 1 || i == pi || i == pi + 1)
	{
		if (j == pj - 1 || j == pj || j == pj + 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Entity::move(int di, int dj, bool ** hasSolid)
{
	std::cout << di << " " << dj << std::endl;
	std::cout << "ent moved" << std::endl;
	int ci, cj;
	ci = this->i + di;
	cj = this->j + dj;
	if (dj < 0)
	{
		goDest = LEFT;
	}
	else if (dj > 0)
	{
		goDest = RIGHT;
	}
	this->i = ci;
	this->j = cj;
}

//void Entity::attack(Player* player)
//{
//	player->attacked(dame);
//}

void Entity::setPath(Dest go)
{
	goDest = go;
}

int Entity::getDame()
{
	return this->dame;
}

void Entity::setBox()
{
	mBox.x = j * TILE_SIZE;
	mBox.y = i * TILE_SIZE;
	mBox.w = TILE_SIZE;
	mBox.h = TILE_SIZE;
}

bool Entity::getDead()
{
	return this->dead;
}

bool Entity::getAlert()
{
	return alert;
}

//void Entity::think(int pi, int pj, int** hasSolid)
//{
//	if (alert != true)
//	{
//		lookForPlayer(pi, pj, hasSolid);
//	}
//	else if (hasLOSglobal(pi, pj, i, j, hasSolid))
//	{
//		moveToPlayer();
//	}
//}

void Entity::lookForPlayer(int pi, int pj, bool** hasSolid)
{
	if (getDistance(pi, pj, i, j) <= VIS_DISTANCE && hasLOSglobal(pi, pj, i, j, hasSolid))
	{
		alert = true;
		hunt = 3;
	}
}

//void Entity::moveToPlayer(Entity* e)
//{
//	int dx, dy;
//
//	createAStarRoute(e, dungeon.player->x, dungeon.player->y, &dx, &dy);
//
//	moveEntity(e, dx, dy);
//
//	m->patrolDest.x = dungeon.player->x;
//
//	m->patrolDest.y = dungeon.player->y;
//}

void Entity::setPatrolDest(int j, int i)
{
	patrolDest.x = j;
	patrolDest.y = i;
}

SDL_Point Entity::getPatrolDest()
{
	return patrolDest;
}

bool Entity::isBlocked(int j, int i, bool** hasSolid, int pj, int pi, Entity* head)
{
	Entity* e;

	if (i == this->i && j == this->j) return 0;

	if (hasSolid[i][j] == 1 || (j == pj && i == pi))
	{
		return 1;
	}

	for (Entity* e = head; e != NULL; e = e->next)
	{
		if (e->getPosI() == i && e->getPosJ() == j)
		{
			return 1;
		}
	}

	return 0;
}

int Entity::getHunt()
{
	return hunt;
}
void Entity::setHunt(int h)
{
	hunt = h;
}

int Entity::getSawI()
{
	return sawI;
}
int Entity::getSawJ()
{
	return sawJ;
}
void Entity::setSaw(int i, int j)
{
	sawI = i;
	sawJ = j;
}

bool Entity::gotLastSaw()
{
	if (this->i == sawI && this->j == sawJ)
	{
		return true;
	}
	else
	{
		return false;
	}
}