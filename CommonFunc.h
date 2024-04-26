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
#include<cmath>
#include<cstdlib>

//const int LEVEL_WIDTH = 512;
//const int LEVEL_HEIGHT = 736;

//const int LEVEL_WIDTH = 480;
//const int LEVEL_HEIGHT = 496;

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 750;

const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int CORLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

const int PLAYER_VEL = 16;
const int PLAYER_HEIGHT = 15;
const int PLAYER_WIDTH = 12;

const int TILE_SIZE = 16;
const int TOTAL_TILE_SPRITES = 273;

//const int LEVEL_ROWS = 46;
//const int LEVEL_COLS = 32;

//const int LEVEL1_ROWS = 31;
//const int LEVEL1_COLS = 30;
const int LEVEL_LAYERS = 5;

const int MAX_MONSTER_TYPE = 4;

const int VIS_DISTANCE = 10;

const Uint8 HALF_FOG = 175;
const Uint8 FULL_FOG = 255;
const Uint8 NONE_FOG = 0;

const Uint32 ATTACK_TIME = 480;
const Uint32 MOVE_TIME = 200;

bool checkCollision(SDL_FRect a, SDL_FRect b);
int getDistance(int i, int j, int mi, int mj);
bool hasLOSglobal(int i1, int j1, int i2, int j2, bool** hasSolid);

#endif