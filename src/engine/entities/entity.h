#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "../utils.h"
#include "../global.h"

typedef struct entity {
    SDL_Rect* rect;
    short dir[2];
    float acc[2];
    float vel[2];
    float max_speed;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
} Entity;

void construct_entity(Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, float acc_x, float acc_y, float max_speed, 
                      char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_entity(Entity* entity);

void render_entity(Entity* entity);