#ifndef FogOfWar_h_
#define FogOfWar_h_

#include"CommonFunc.h"
#include"LTexture.h"
#include"Tile.h"

class FogOfWar
{
public:
	FogOfWar();
	~FogOfWar();

	void free();
	bool loadFogTexture(std::string path, SDL_Renderer* screen);

	void render(SDL_FRect& camera, SDL_Renderer* screen, float scale = 1.0);

	bool hasLOS(int i1, int j1, int i2, int j2);

	void update(int mi, int mj);

	void updateSolid(Tile**** tileSet);
	void setPartClip(int x, int y, int w, int h);

private:
	LTexture BlackTexture;

	SDL_Rect partClip;

	bool** hasSolid;

	int** fogMap;


};

#endif