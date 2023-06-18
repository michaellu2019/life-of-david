#pragma once

#include "entity.h"

#define NUM_MOVES 2400
#define MOVE_SET_SIZE 8
#define MOVE_MUTATION_RATE 15

typedef enum move {
    UP,
    UP_AND_LEFT,
    UP_AND_RIGHT,
    DOWN,
    DOWN_AND_LEFT,
    DOWN_AND_RIGHT,
    LEFT,
    RIGHT,
} Move;

typedef struct david {
    int id;
    Entity* entity;
    float acc;
    float score;
    char* alive_texture_path;
    char* dead_texture_path;
    Move moves[NUM_MOVES];
    uint32_t move_index;
} David;

void construct_david(David* david, Entity* entity, SDL_Rect* rect, int id, int x, int y, int w, int h, 
                     float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed, 
                     char* alive_texture_path, char* dead_texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_david(David* david, bool teleop);

void render_david(David* david, bool render_hitbox);

void generate_random_moves(David* david);

void mutate_moves(David* david);

void reset_david(David* david);

void terminate_david(David* david);