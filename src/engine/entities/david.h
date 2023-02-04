#pragma once

#include "entity.h"

typedef struct david {
    Entity* entity;
    float acc;
    long fitness;
} David;

void construct_david(David* david, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                     float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed, 
                     char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_david(David* david);

void render_david(David* david, bool render_hitbox);

void update_david_hitbox_corners(David* david);

void reset_david(David* david);