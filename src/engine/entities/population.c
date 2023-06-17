#include "population.h"

void construct_population(Population* population, int start_x, int start_y,
                          David davids[], Entity david_entities[], SDL_Rect david_rects[], SDL_Surface **david_surfaces, SDL_Texture **david_textures,
                          SDL_Renderer* renderer, SDL_Window* window)
{
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        construct_david(&davids[i], &david_entities[i], &david_rects[i], WINDOW_WIDTH/2 - WINDOW_WIDTH/48, WINDOW_WIDTH/10, WINDOW_WIDTH/24, WINDOW_WIDTH/24 * (500.0f/230), 
                    WINDOW_WIDTH * 0.00001f, WINDOW_WIDTH * 0.00001f, 0.01f, WINDOW_WIDTH/144, 5.0f,
                    "./src/assets/ss_david.png", "./src/assets/ss_david_crashed.png", david_surfaces[i], david_textures[i], renderer, window);     
        population->davids[i] = &davids[i];
    }
}

void update_population(Population* population)
{
    for (int i = 0; i < POPULATION_SIZE; i++) {
        update_david(population->davids[i], false);
    }
}

void render_population(Population* population, bool render_hitbox)
{
    for (int i = 0; i < POPULATION_SIZE; i++) {
        render_david(population->davids[i], render_hitbox);
    }
}

void eradicate_population(Population* population)
{
    for (int i = 0; i < POPULATION_SIZE; i++) {
        SDL_DestroyTexture(population->davids[i]->entity->texture);
    }
}