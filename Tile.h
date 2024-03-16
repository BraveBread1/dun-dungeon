#ifndef Tile_h_
#define Tile_h_

#include"CommonFunc.h"
#include"LTexture.h"

class Tile
{
public:
    Tile(int x, int y, int tileType);

    void render(SDL_FRect& camera, LTexture &tileTexture, SDL_Rect clip, SDL_Renderer* screen, float scale = 1.0);

    int getType();
    SDL_FRect getBox();

private:
    SDL_FRect mBox;

    int mType;
};

bool checkCollision(SDL_FRect a, SDL_FRect b);

#endif