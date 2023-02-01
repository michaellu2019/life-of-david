#pragma once

#include <stdbool.h>

#include "entity.h"

typedef struct david {
    Entity* entity;
    bool jumping;
} David;

void construct_david(David* david, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, float acc_x, float acc_y, float max_speed, 
                     char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_david(David* david);

void render_david(David* david);