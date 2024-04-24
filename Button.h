#ifndef BUTTON_H
#define BUTTON_H

#include"CommonFunc.h"
#include"LTexture.h"

class Button
{
public:
    Button(int x, int y, int w, int h);
    ~Button();

    void hanldEvent(SDL_Event e);
    bool loadButtonTexture(std::string path, SDL_Renderer* screen);
    void render(SDL_Renderer* screen);
    void free();
    bool checkCollision(int mx, int my);
    int getPressed();

private:
    LTexture buttonTexture;
    SDL_Rect box;
    enum mouse_state
    {
        MOUSE_OUT = 0,
        MOUSE_OVER = 1,
        MOUSE_DOWN = 2
    };
    int mouseSate;
    bool release;
    int pressed;
    
};


#endif // BUTTON_H