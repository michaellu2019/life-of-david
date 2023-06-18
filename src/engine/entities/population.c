#include <math.h>

#include "population.h"

void construct_population(Population* population, int start_x, int start_y, int goal_x, int goal_y,
                          David davids[], Entity david_entities[], SDL_Rect david_rects[], SDL_Surface **david_surfaces, SDL_Texture **david_textures,
                          SDL_Renderer* renderer, SDL_Window* window)
{
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        construct_david(&davids[i], &david_entities[i], &david_rects[i], i, WINDOW_WIDTH/2 - WINDOW_WIDTH/96, WINDOW_WIDTH/20, WINDOW_WIDTH/48, WINDOW_WIDTH/48 * (500.0f/230), 
                    WINDOW_WIDTH * 0.0001f, WINDOW_WIDTH * 0.0001f, 0.03f, WINDOW_WIDTH/144, 5.0f,
                    "./src/assets/ss_david.png", "./src/assets/ss_david_crashed.png", david_surfaces[i], david_textures[i], renderer, window);     
        population->davids[i] = &davids[i];
    }

    population->start_x = start_x;
    population->start_y = start_y;
    population->goal_x = goal_x;
    population->goal_y = goal_y;
    population->num_alive = POPULATION_SIZE;
    population->score_sum = 0;
    population->best_score = -1;
    population->best_id = 0;
    population->best_generation_number = 0;
    population->generation_best_score = -1;
    population->generation_best_id = 0;

    printf("Population Generation #%d Size: %d Start: (%d, %d) Goal: (%d, %d)\n\n", population->generation_number, population->num_alive, population->start_x, population->start_y, population->goal_x, population->goal_y);
}

void update_population(Population* population)
{
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        update_david(population->davids[i], false);
        int dx = population->davids[i]->entity->rect->x - population->goal_x;
        int dy = population->davids[i]->entity->rect->y - population->goal_y;
        if (sqrt(dx*dx + dy*dy) < REACHED_GOAL_TOLERANCE)
        {
            population->davids[i]->entity->status = NIRVANA;
        }

        if (population->davids[i]->entity->status == DEAD && population->davids[i]->score == -1) 
        {
            population->num_alive--;
            int cost = POS_SCORE_WEIGHT * abs(dx) + POS_SCORE_WEIGHT * dy*dy + SPEED_SCORE_WEIGHT * population->davids[i]->move_index;
            float score =  (SCORE_SCALE/cost);
            population->davids[i]->score = score < 0 ? 0 : score;
            population->score_sum += population->davids[i]->score;

            if (population->davids[i]->score > population->generation_best_score)
            {
                population->generation_best_score = population->davids[i]->score;
                population->generation_best_id = i;
            }

            printf("David #%d.%d died... Score: %f Moves: %d. Population Size: %d\n", population->generation_number, i, population->davids[i]->score, population->davids[i]->move_index + 1, population->num_alive);
        }

        if (population->davids[i]->entity->status == NIRVANA) 
        {
            population->davids[i]->score *= NIRVANA_SCORE_BONUS;
            printf("David #%d.%d has achieved Nirvana... Score: %f Moves: %d.\n", population->generation_number, i, population->davids[i]->score, population->davids[i]->move_index + 1);
        }
    }

    if (population->num_alive <= 0) 
    {
        evolve_population(population);
        respawn_population(population);
    }
}

void render_population(Population* population, bool render_hitbox)
{
    for (int i = 0; i < POPULATION_SIZE; i++)
        render_david(population->davids[i], i == population->generation_best_id);
}

void respawn_population(Population* population)
{
    population->num_alive = POPULATION_SIZE;
    population->score_sum = 0;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        reset_david(population->davids[i]);
    }
}

void evolve_population(Population* population)
{
    printf("The population is evolving to generation #%d\n", population->generation_number);
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        //printf("EVOLVE %d: ", i);
        if (i == population->generation_best_id)
            continue;

        int parent_id = sample_poulation(population);
        // printf("David #%d will inherit David #%d, with score %.3f\n", i, parent_id, population->davids[parent_id]->score);
        for (int j = 0; j < NUM_MOVES; j++)
        {
            population->david_move_sets[i][j] = population->davids[parent_id]->moves[j];
            //printf("%d ", population->david_move_sets[i][j]);
        }
        
    }
    //printf("%s\n", "");
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        //printf("OVELVO %d: ", i);
        if (i == population->generation_best_id)
            continue;

        for (int j = 0; j < NUM_MOVES; j++) 
        {
            population->davids[i]->moves[j] = population->david_move_sets[i][j];
            //printf("%d ", population->davids[i]->moves[j]);
        }
        mutate_moves(population->davids[i]);
    }
    //printf("%s\n", "");
    population->generation_number++;
}

int sample_poulation(Population* population)
{
    int score_sum_sample = rand() % ((int) population->score_sum);
    float running_score_sum = 0;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        running_score_sum += population->davids[i]->score;
        if (running_score_sum > score_sum_sample) 
        {
            // printf("Sampling: Total Score: %.3f Sample: %d Selection: %d\n", population->score_sum, score_sum_sample, i);
            return i;
        }
    }

    return 0;
}

void eradicate_population(Population* population)
{
    for (int i = 0; i < POPULATION_SIZE; i++)
        SDL_DestroyTexture(population->davids[i]->entity->texture);
}