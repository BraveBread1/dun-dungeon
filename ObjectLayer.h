#ifndef OBJECTLAYER_H_
#define OBJECTLAYER_H_

#include"CommonFunc.h"
#include"Object.h"
#include"LTexture.h"

class ObjectLayer
{
public:
	ObjectLayer(int rows, int cols);
	~ObjectLayer();
	bool setPosition(std::string path);
	void free();
	bool setObjClips(std::string path, SDL_Renderer* screen);
	void render(SDL_FRect& camera, SDL_Renderer* screen, float scale);
	Object*** getObjSet();
	

private:
	int rows, cols;
	LTexture objTexture;
	SDL_Rect* objClips;
	Object*** objSet;
};

#endif