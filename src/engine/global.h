#pragma once

#define WINDOW_WIDTH 2400 //2160
#define WINDOW_HEIGHT 2000 //1440

#define PI 3.14159265

// #define GRAVITY WINDOW_WIDTH/200;
#define GRAVITY 0;
#define FRICTION 1; 

#define GROUND_Y WINDOW_HEIGHT * 0.9

typedef enum gamemode {
    TELEOP,
    EVOLUTION
} Gamemode;

typedef struct global {
    Gamemode gamemode;
} Global;

extern Global global;
