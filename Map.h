#ifndef Map_h_
#define Map_h_
#include"CommonFunc.h"
#include"LTexture.h"
#include"Tile.h"

class Map
{
private:
	int rows, cols;
	int layers;
	
	LTexture tileTexture;
	SDL_Rect* tileClips;

public:
	Map(int row, int col, int layer);
	~Map();
	bool setTiles(std::string* path);
	void free();
	bool setTileClips(std::string path, SDL_Renderer* screen);
	Tile**** getTileSet();
	LTexture getTileTexture();
	void renderLayer(SDL_FRect& camera, SDL_Renderer* screen, int layer, float scale);
	Tile**** tileSet;
};

#endif