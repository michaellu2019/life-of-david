#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "../utils.h"
#include "../global.h"

typedef struct entity {
    SDL_Rect* rect;
    int start_x;
    int start_y;
    double angle;
    short dir[2];
    float acc[3];
    float vel[3];
    float max_speed;
    float max_angular_speed;
    int hitbox_corners[4][2];
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
} Entity;

void construct_entity(Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                      float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed,
                      char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_entity(Entity* entity);

void render_entity(Entity* entity, bool render_hitbox);

void update_entity_hitbox_corners(Entity* entity);

void reset_entity(Entity* entity);