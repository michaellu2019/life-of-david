#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "engine/utils.h"
#include "engine/entities/entity.h"
#include "engine/entities/david.h"
#include "engine/entities/post.h"

int main(int argc, char **argv)
{
    bool quit = false;

    David david;
    Entity david_entity;
    SDL_Rect david_rect;
    SDL_Surface* david_surface;
    SDL_Texture* david_texture;

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

    construct_david(&david, &david_entity, &david_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH/48, WINDOW_WIDTH/10, WINDOW_WIDTH/24, WINDOW_WIDTH/24 * (500.0f/230), 
                    WINDOW_WIDTH * 0.00001f, WINDOW_WIDTH * 0.00001f, 0.01f, WINDOW_WIDTH/144, 5.0f,
                    "./src/assets/ss_david.png", david_surface, david_texture, renderer, window);
                     
    construct_post(&left_post, &left_post_entity, &left_post_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH/48 - WINDOW_WIDTH/48 - 2, WINDOW_HEIGHT - WINDOW_WIDTH/48 * (300.0f/100), WINDOW_WIDTH/48, WINDOW_WIDTH/48 * (300.0f/100),
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", left_post_surface, left_post_texture, renderer, window);  
    construct_post(&right_post, &right_post_entity, &right_post_rect, WINDOW_WIDTH/2 + WINDOW_WIDTH/48 + 2, WINDOW_HEIGHT - WINDOW_WIDTH/48 * (300.0f/100), WINDOW_WIDTH/48, WINDOW_WIDTH/48 * (300.0f/100),
                   0, 0, 0, 0, 0,
                   "src/assets/post.png", right_post_surface, right_post_texture, renderer, window);  
    
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

        update_david(&david);
        update_post(&left_post);
        update_post(&right_post);

        if (colliding_with_entity(david.entity, left_post.entity) || colliding_with_entity(david.entity, right_post.entity))
        {
            reset_david(&david);
        }

        SDL_RenderClear(renderer);
        
        render_david(&david, true);
        render_post(&left_post, true);
        render_post(&right_post, true);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/60);
    }

    SDL_DestroyTexture(david.entity->texture);
    SDL_DestroyTexture(left_post.entity->texture);
    SDL_DestroyTexture(right_post.entity->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}