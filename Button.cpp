#include"Button.h"

Button::Button(int x, int y, int w, int h)
{
	mouseSate = MOUSE_DOWN;
	release = true;
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;
	pressed = 0;
}

Button::~Button()
{
	free();
}

bool Button::loadButtonTexture(std::string path, SDL_Renderer* screen)
{
	return buttonTexture.loadFromFile(path, screen);
}

void Button::render(SDL_Renderer* screen)
{
	buttonTexture.render(box.x, box.y, screen);
}

void Button::free()
{
	buttonTexture.free();
}

bool Button::checkCollision(int mx, int my)
{
	if (mx > box.x && mx < box.x + box.w && my > box.y && my < box.y + box.h) return true;
	else return false;
}

void Button::hanldEvent(SDL_Event e)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    if (checkCollision(mx, my))
    {
        mouseSate = MOUSE_OVER;
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                release = false;
                mouseSate = MOUSE_DOWN;
                if (pressed == 0)
                {
                    pressed = 1;
                }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                release = true;
                mouseSate = MOUSE_OVER;
                if (pressed == 1) pressed = 2;
            }
        }
    }
    else
    {
        mouseSate = MOUSE_OUT;
        release = true;
        pressed = 0;
    }
}


int Button::getPressed()
{
	return pressed;
}