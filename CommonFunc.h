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

const int LEVEL_WIDTH = 512;
const int LEVEL_HEIGHT = 736;

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 600;

const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int CORLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

const int PLAYER_VEL = 16;
const int PLAYER_HEIGHT = 15;
const int PLAYER_WIDTH = 12;

const int TILE_SIZE = 16;
const int TOTAL_TILES = 1050;
const int TOTAL_TILE_SPRITES = 257;

const int TILE_VOID = 0;
const int TILE_WALL = 1;
const int TILE_ROAD = 2;

const int LEVEL1_ROWS = 46;
const int LEVEL1_COLS = 32;
const int LEVEL1_LAYERS = 3;

#endif