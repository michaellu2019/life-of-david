#pragma once

#include "david.h"

#define POPULATION_SIZE 100

typedef struct population {
    David *davids[POPULATION_SIZE];
    int num_alive;
    int start_x;
    int start_y;
} Population;

void construct_population(Population* population, int start_x, int start_y,
                          David davids[], Entity david_entities[], SDL_Rect david_rects[], SDL_Surface **david_surfaces, SDL_Texture **david_textures,
                          SDL_Renderer* renderer, SDL_Window* window);

void update_population(Population* population);

void render_population(Population* population, bool render_hitbox);

void eradicate_population(Population* population);