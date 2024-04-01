#include"Tile.h"

Tile::Tile(int x, int y, int tileType)
{
    mBox.x = x;
    mBox.y = y;

    mBox.w = TILE_SIZE;
    mBox.h = TILE_SIZE;

    mType = tileType;
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