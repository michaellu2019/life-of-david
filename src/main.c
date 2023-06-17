#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "engine/utils.h"
#include "engine/entities/entity.h"
#include "engine/entities/david.h"
#include "engine/entities/post.h"
#include "engine/entities/population.h"

int main(int argc, char **argv)
{
    bool quit = false;


    Post left_post;
    Entity left_post_entity;
    SDL_Rect left_post_rect;
    SDL_Surface* left_post_surface;
    SDL_Texture* left_post_texture;
    Post right_post;
    Entity right_post_entity;
    SDL_Rect right_post_rect;
    SDL_Surface* right_post_surface;
    SDL_Texture* right_post_texture;

    David davids[POPULATION_SIZE];
    Entity david_entities[POPULATION_SIZE];
    SDL_Rect david_rects[POPULATION_SIZE];
    SDL_Surface* david_surfaces[POPULATION_SIZE];
    SDL_Texture* david_textures[POPULATION_SIZE];

    Population population;

    srand(time(NULL));


    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
        ERROR_EXIT("Error initializing SDL: %s\n", SDL_GetError());
    
    SDL_Window* window = SDL_CreateWindow("David", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        SDL_Quit();
        ERROR_EXIT("Error creating window: %s\n", SDL_GetError());
    }

    uint32_t render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        ERROR_EXIT("Error creating renderer: %s\n", SDL_GetError());
    }
      
    construct_post(&left_post, &left_post_entity, &left_post_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH/48 - WINDOW_WIDTH/48 - 2, WINDOW_HEIGHT - WINDOW_WIDTH/48 * (300.0f/100), WINDOW_WIDTH/48, WINDOW_WIDTH/48 * (300.0f/100),
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", left_post_surface, left_post_texture, renderer, window);  
    construct_post(&right_post, &right_post_entity, &right_post_rect, WINDOW_WIDTH/2 + WINDOW_WIDTH/48 + 2, WINDOW_HEIGHT - WINDOW_WIDTH/48 * (300.0f/100), WINDOW_WIDTH/48, WINDOW_WIDTH/48 * (300.0f/100),
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", right_post_surface, right_post_texture, renderer, window);  
    construct_population(&population, WINDOW_WIDTH/2 - WINDOW_WIDTH/48, WINDOW_WIDTH/10, 
                         davids, david_entities, david_rects, david_surfaces, david_textures,
                         renderer, window);

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    SDL_Scancode keycode = event.key.keysym.scancode;
                    break;
                default:
                    break;
            }
        }

        update_post(&left_post);
        update_post(&right_post);
        update_population(&population);

        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (colliding_with_entity(population.davids[i]->entity, left_post.entity) || colliding_with_entity(population.davids[i]->entity, right_post.entity))
            {
                terminate_david(population.davids[i]);
            }
        }
        
        SDL_RenderClear(renderer);
        
        render_population(&population, true);
        render_post(&left_post, true);
        render_post(&right_post, true);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/60);
    }

    eradicate_population(&population);
    SDL_DestroyTexture(left_post.entity->texture);
    SDL_DestroyTexture(right_post.entity->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}