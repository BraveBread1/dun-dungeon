#include"Tile.h"

Tile::Tile(int x, int y, int tileType)
{
    mBox.x = x;
    mBox.y = y;

    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    mType = tileType;
}

void Tile::render(SDL_Rect& camera, LTexture &tileTexture, SDL_Rect clip, SDL_Renderer* screen)
{
    if (checkCollision(camera, mBox))
    {
        tileTexture.render(mBox.x - camera.x, mBox.y - camera.y, screen, &clip);
    }
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}