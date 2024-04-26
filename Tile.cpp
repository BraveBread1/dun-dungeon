#include"Tile.h"

Tile::Tile(int x, int y, int tileType, bool solid)
{
    mBox.x = x;
    mBox.y = y;

    mBox.w = TILE_SIZE;
    mBox.h = TILE_SIZE;

    mType = tileType;

    this->isSolid = solid;
    this->revealed = 0;
}

void Tile::render(SDL_FRect& camera, LTexture &tileTexture, SDL_Rect clip, SDL_Renderer* screen, float scale)
{
    if (checkCollision(camera, mBox))
    {
        tileTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &clip, scale);
    }
}


int Tile::getType()
{
    return mType;
}

SDL_FRect Tile::getBox()
{
    return mBox;
}

bool Tile::getRevealed() { return revealed; }
bool Tile::getSolid() { return isSolid; }
bool Tile::getVisible() { return visible; }

void Tile::setRevealed(bool i) { revealed = i; }
void Tile::setSolid(bool i) { isSolid = i; }
void Tile::setVisible(bool i) { visible = i; }