#ifndef Menu_h_
#define Menu_h_

#include"CommonFunc.h"
#include"LTexture.h"
#include"Button.h"

class Menu
{
public:
	Menu();
	~Menu();

	bool loadMenuTexture(std::string path, SDL_Renderer* screen);
	void render(SDL_Renderer* screen);
	void handleEvent(SDL_Event e);

	void free();
	Button* enter;
	Button* instruction;
	Button* quit;

private:
	LTexture menuTexture;
	LTexture gameName;
	SDL_Rect gameNameClip;
	

	int x;

};


#endif
