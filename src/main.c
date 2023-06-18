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
    srand(time(NULL));

    bool quit = false;
    bool render_hitboxes = false;

    int goal_x = WINDOW_WIDTH/2 - WINDOW_WIDTH/12;//0.8f * (rand() % WINDOW_WIDTH/2 - WINDOW_WIDTH/4);
    int goal_y = WINDOW_HEIGHT - WINDOW_WIDTH/96 * (300.0f/100);
    int goal_post_margin = WINDOW_WIDTH/70;

    Post obstacle_top_post;
    Entity obstacle_top_post_entity;
    SDL_Rect obstacle_top_post_rect;
    SDL_Surface* obstacle_top_post_surface;
    SDL_Texture* obstacle_top_post_texture;
    Post obstacle_bottom_post;
    Entity obstacle_bottom_post_entity;
    SDL_Rect obstacle_bottom_post_rect;
    SDL_Surface* obstacle_bottom_post_surface;
    SDL_Texture* obstacle_bottom_post_texture;
    Post goal_left_post;
    Entity goal_left_post_entity;
    SDL_Rect goal_left_post_rect;
    SDL_Surface* goal_left_post_surface;
    SDL_Texture* goal_left_post_texture;
    Post goal_right_post;
    Entity goal_right_post_entity;
    SDL_Rect goal_right_post_rect;
    SDL_Surface* goal_right_post_surface;
    SDL_Texture* goal_right_post_texture;

    David davids[POPULATION_SIZE];
    Entity david_entities[POPULATION_SIZE];
    SDL_Rect david_rects[POPULATION_SIZE];
    SDL_Surface* david_surfaces[POPULATION_SIZE];
    SDL_Texture* david_textures[POPULATION_SIZE];

    Population population;

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
      
    construct_post(&obstacle_top_post, &obstacle_top_post_entity, &obstacle_top_post_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH/4, WINDOW_HEIGHT/2 - WINDOW_HEIGHT/16, WINDOW_WIDTH/2, WINDOW_HEIGHT/50,
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", obstacle_top_post_surface, obstacle_top_post_texture, renderer, window);  
    construct_post(&obstacle_bottom_post, &obstacle_bottom_post_entity, &obstacle_bottom_post_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH * (3/4), WINDOW_HEIGHT/2 + WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/50,
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", obstacle_bottom_post_surface, obstacle_bottom_post_texture, renderer, window);  
    construct_post(&goal_left_post, &goal_left_post_entity, &goal_left_post_rect, goal_x - WINDOW_WIDTH/96 - WINDOW_WIDTH/96 - goal_post_margin, goal_y, WINDOW_WIDTH/96, WINDOW_WIDTH/96 * (300.0f/100),
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", goal_left_post_surface, goal_left_post_texture, renderer, window);  
    construct_post(&goal_right_post, &goal_right_post_entity, &goal_right_post_rect, goal_x + WINDOW_WIDTH/96 + goal_post_margin, goal_y, WINDOW_WIDTH/96, WINDOW_WIDTH/96 * (300.0f/100),
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", goal_right_post_surface, goal_right_post_texture, renderer, window);  
    construct_population(&population, WINDOW_WIDTH/2 - WINDOW_WIDTH/48, WINDOW_WIDTH/10, goal_x, goal_y,
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

        update_post(&obstacle_top_post);
        update_post(&obstacle_bottom_post);
        update_post(&goal_left_post);
        update_post(&goal_right_post);
        update_population(&population);

        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (colliding_with_entity(population.davids[i]->entity, obstacle_top_post.entity) || colliding_with_entity(population.davids[i]->entity, obstacle_bottom_post.entity) ||
                colliding_with_entity(population.davids[i]->entity, goal_left_post.entity) || colliding_with_entity(population.davids[i]->entity, goal_right_post.entity))
            {
                terminate_david(population.davids[i]);
            }
        }
        
        SDL_RenderClear(renderer);
        
        render_population(&population, render_hitboxes);
        render_post(&obstacle_top_post, render_hitboxes);
        render_post(&obstacle_bottom_post, render_hitboxes);
        render_post(&goal_left_post, render_hitboxes);
        render_post(&goal_right_post, render_hitboxes);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/60);
    }

    eradicate_population(&population);
    SDL_DestroyTexture(obstacle_top_post.entity->texture);
    SDL_DestroyTexture(obstacle_bottom_post.entity->texture);
    SDL_DestroyTexture(goal_left_post.entity->texture);
    SDL_DestroyTexture(goal_right_post.entity->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}