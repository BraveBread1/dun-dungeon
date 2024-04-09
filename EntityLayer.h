#ifndef EntityLayer_h_

#define EntityLayer_h_

#include"CommonFunc.h"
#include"Entity.h"

class EntityLayer
{
public:
	EntityLayer(int rows, int cols);
	~EntityLayer();
	void render(SDL_FRect& camera, SDL_Renderer* screen, float scale);
	bool setPosition(std::string path);
	void free();
	bool loadEntTextureSet(std::string* path, SDL_Renderer* screen);
	int** getEntSet();
	friend void addLast(Entity*& a, Entity* tmp);
	Entity* checkEntCollision(int i, int j);

	bool loadHpTexture(std::string path1, std::string path2, SDL_Renderer* screen);


private:
	int rows, cols;
	LTexture* entTexture;
	LTexture greenHp;
	LTexture redHp;
	int** entSet;
	Entity* head;

};

#endif