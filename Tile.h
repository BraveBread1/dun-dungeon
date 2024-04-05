#ifndef Tile_h_
#define Tile_h_

#include"CommonFunc.h"
#include"LTexture.h"

class Tile
{
public:
    Tile(int x, int y, int tileType, bool solid);

    void render(SDL_FRect& camera, LTexture &tileTexture, SDL_Rect clip, SDL_Renderer* screen, float scale = 1.0);

    int getType();
    SDL_FRect getBox();
    bool getVisible();
    bool getRevealed();
    bool getSolid();
    void setVisible(bool i);
    void setRevealed(bool i);
    void setSolid(bool i);

private:
    SDL_FRect mBox;
    int mType;
    bool visible;
    bool revealed;
    bool isSolid;
};

#endif