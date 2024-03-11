#ifndef CommonFunc_h_
#define CommonFunc_h_

#include<string>
#include<SDL_image.h>
#include<SDL.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>

const int LEVEL_WIDTH = 560;
const int LEVEL_HEIGHT = 480;

const int SCREEN_WIDTH = 560;
const int SCREEN_HEIGHT = 480;

const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int CORLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

const int PLAYER_VEL = 16;
const int PLAYER_HEIGHT = 15;
const int PLAYER_WIDTH = 12;

const int TILE_LENG = 16;
const int TOTAL_TILES = 1050;
const int TOTAL_TILE_SPRITES = 3;

const int TILE_VOID = 0;
const int TILE_WALL = 1;
const int TILE_ROAD = 2;

const int LEVEL_WIDTH_CELL = 35;
const int LEVEL_HEIGHT_CELL = 30;

#endif