#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "engine/utils.h"
#include "engine/entities/entity.h"
#include "engine/entities/david.h"

int main(int argc, char **argv)
{
    bool quit = false;

    David david;
    Entity david_entity;
    SDL_Rect david_rect;
    SDL_Surface* david_surface;
    SDL_Texture* david_texture;


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

    // / load the image into memory using SDL_image library function
    SDL_Surface* surface = IMG_Load("./src/assets/david.png");
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // load the image data into the graphics hardware's memory
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    construct_david(&david, &david_entity, &david_rect, 0, 0, WINDOW_WIDTH/6, WINDOW_WIDTH/6, WINDOW_WIDTH/180, -WINDOW_WIDTH/20, WINDOW_WIDTH/144, 
                     "./src/assets/david.png", david_surface, david_texture, renderer, window);
                     
    bool david_jumping = false;
    
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

        SDL_RenderClear(renderer);
        
        render_david(&david);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/60);
    }

    SDL_DestroyTexture(david.entity->texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}