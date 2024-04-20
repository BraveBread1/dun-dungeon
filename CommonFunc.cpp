#include"CommonFunc.h"

bool checkCollision(SDL_FRect a, SDL_FRect b)
{
    //The sides of the rectangles
    float leftA, leftB;
    float rightA, rightB;
    float topA, topB;
    float bottomA, bottomB;

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

int getDistance(int i, int j, int mi, int mj)
{
    int di = abs(mi - i);
    int dj = abs(mj - j);

    return di + dj;
}

bool hasLOSglobal(int i1, int j1, int i2, int j2, bool** hasSolid)
{
    int di, dj, si, sj, err, e2;

    di = abs(i2 - i1);
    dj = abs(j2 - j1);

    si = (i1 < i2) ? 1 : -1;
    sj = (j1 < j2) ? 1 : -1;
    err = dj - di;

    while (1)
    {
        e2 = 2 * err;

        if (e2 > -di)
        {
            err -= di;
            j1 += sj;
        }

        if (e2 < dj)
        {
            err += dj;
            i1 += si;
        }

        if (j1 == j2 && i1 == i2)
        {
            return 1;
        }

        if (hasSolid[i1][j1])
        {
            return 0;
        }

    }
    return 0;
}