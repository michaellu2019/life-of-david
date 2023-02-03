#pragma once

#include "entity.h"

typedef struct post {
    Entity* entity;
} Post;

void construct_post(Post* post, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                    float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed,
                    char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_post(Post* post);

void render_post(Post* post, bool render_hitbox);