#ifndef Tile_h_
#define Tile_h_

#include"CommonFunc.h"
#include"LTexture.h"

class Tile
{
public:
    Tile(int x, int y, int tileType);

    void render(SDL_Rect& camera, LTexture &tileTexture, SDL_Rect clip, SDL_Renderer* screen);

    int getType();
    SDL_Rect getBox();

private:
    SDL_Rect mBox;

    int mType;
};

bool checkCollision(SDL_Rect a, SDL_Rect b);

#endif