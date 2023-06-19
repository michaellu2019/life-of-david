#pragma once

#include "david.h"

#define POPULATION_SIZE 100
#define SCORE_SCALE 100000000.0f
#define POS_SCORE_WEIGHT 2.0f
#define SPEED_SCORE_WEIGHT 0.1f
#define NIRVANA_SCORE_BONUS 8.0f
#define REACHED_GOAL_TOLERANCE 40.0f

typedef struct population {
    David *davids[POPULATION_SIZE];
    int start_x;
    int start_y;
    int goal_x;
    int goal_y;
    int generation_number;
    int num_alive;
    float score_sum;
    float best_score;
    int best_id;
    int best_generation_number;
    float generation_best_score;
    int generation_best_id;
    Move david_move_sets[POPULATION_SIZE][NUM_MOVES];
} Population;

void construct_population(Population* population, int start_x, int start_y, int goal_x, int goal_y,
                          David davids[], Entity david_entities[], SDL_Rect david_rects[], SDL_Surface **david_surfaces, SDL_Texture **david_textures,
                          SDL_Renderer* renderer, SDL_Window* window);

void update_population(Population* population);

void render_population(Population* population, bool render_hitbox);

void respawn_population(Population* population);

void evolve_population(Population* population); 

int sample_poulation(Population* population);

void eradicate_population(Population* population);