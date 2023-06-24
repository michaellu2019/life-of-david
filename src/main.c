#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#include "engine/utils.h"
#include "engine/entities/entity.h"
#include "engine/entities/david.h"
#include "engine/entities/post.h"
#include "engine/entities/population.h"
#include "engine/entities/text.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    global.gamemode = EVOLUTION;
    bool quit = false;
    bool render_hitboxes = false;

    int jellyfish_width = global.gamemode == TELEOP ? WINDOW_WIDTH/12 : WINDOW_WIDTH/48;
    // int goal_x = WINDOW_WIDTH/2 - 0.8f * (rand() % WINDOW_WIDTH - WINDOW_WIDTH/2);
    int goal_x = WINDOW_WIDTH * 0.8f;
    int goal_y = WINDOW_HEIGHT - jellyfish_width * (640.0f/520);
    int goal_post_margin = WINDOW_WIDTH/70;

    char* post_img_path = "src/assets/post.png";
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
    char* jellyfish_img_path = "src/assets/jellyfish.png";
    Entity jellyfish;
    SDL_Rect jellyfish_rect;
    SDL_Surface* jellyfish_surface;
    SDL_Texture* jellyfish_texture;

    David davids[POPULATION_SIZE];
    Entity david_entities[POPULATION_SIZE];
    SDL_Rect david_rects[POPULATION_SIZE];
    SDL_Surface* david_surfaces[POPULATION_SIZE];
    SDL_Texture* david_textures[POPULATION_SIZE];

    Population population;

    char* font_ttf_path = "src/assets/Archivo/Archivo-VariableFont_wdth,wght.ttf";
    int font_size = WINDOW_HEIGHT/40;
    SDL_Color text_color = {255, 255, 255};
    Text generation_text;
    SDL_Rect generation_text_rect;
    SDL_Surface* generation_text_surface;
    SDL_Texture* generation_text_texture;
    Text population_text;
    SDL_Rect population_text_rect;
    SDL_Surface* population_text_surface;
    SDL_Texture* population_text_texture;
    Text best_score_text;
    SDL_Rect best_score_text_rect;
    SDL_Surface* best_score_text_surface;
    SDL_Texture* best_score_text_texture;
    Text best_david_text;
    SDL_Rect best_david_text_rect;
    SDL_Surface* best_david_text_surface;
    SDL_Texture* best_david_text_texture;

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
        ERROR_EXIT("Error initializing SDL: %s\n", SDL_GetError());
    
    SDL_Window* window = SDL_CreateWindow("The Life and Evolution of David", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
    TTF_Init();
      
    construct_post(&obstacle_top_post, &obstacle_top_post_entity, &obstacle_top_post_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH/4, WINDOW_HEIGHT/2 - WINDOW_HEIGHT/16, WINDOW_WIDTH/2, WINDOW_HEIGHT/50,
                   0, 0, 0, 0, 0,
                   post_img_path, obstacle_top_post_surface, obstacle_top_post_texture, renderer, window);  
    construct_post(&obstacle_bottom_post, &obstacle_bottom_post_entity, &obstacle_bottom_post_rect, WINDOW_WIDTH/2 - WINDOW_WIDTH * (3/4), WINDOW_HEIGHT/2 + WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/50,
                   0, 0, 0, 0, 0,
                   post_img_path, obstacle_bottom_post_surface, obstacle_bottom_post_texture, renderer, window);  
    construct_entity(&jellyfish, &jellyfish_rect, goal_x, goal_y, jellyfish_width, jellyfish_width * (640.0f/520),
                     0, 0, 0, 0, 0,
                     jellyfish_img_path, jellyfish_surface, jellyfish_texture, renderer, window);
    construct_population(&population, WINDOW_WIDTH/2 - WINDOW_WIDTH/48, WINDOW_WIDTH/10, goal_x, goal_y,
                         davids, david_entities, david_rects, david_surfaces, david_textures,
                         renderer, window);
    construct_text(&generation_text, &generation_text_rect, 0, 0,
                   font_ttf_path, font_size, "Generation: 0", 
                   &text_color, generation_text_surface, generation_text_texture, renderer, window);
    construct_text(&population_text, &population_text_rect, 0, font_size,
                   font_ttf_path, font_size, "Population Size: 0", 
                   &text_color, population_text_surface, population_text_texture, renderer, window);
    construct_text(&best_score_text, &best_score_text_rect, 0, font_size * 2,
                   font_ttf_path, font_size, "Best Score: 420", 
                   &text_color, best_score_text_surface, best_score_text_texture, renderer, window);
    construct_text(&best_david_text, &best_david_text_rect, 0, font_size * 3,
                   font_ttf_path, font_size, "Best David: #420.69", 
                   &text_color, best_david_text_surface, best_david_text_texture, renderer, window);
    sleep(1);
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
        update_entity(&jellyfish);

        char new_generation_text[128];
        sprintf(new_generation_text, "Generation: %d", population.generation_number);
        update_text(&generation_text, new_generation_text);
        char new_population_text[128];
        sprintf(new_population_text, "Population Size: %d", population.num_alive);
        update_text(&population_text, new_population_text);
        char new_best_score_text[128];
        sprintf(new_best_score_text, "Best Score: %.3f", population.best_score);
        update_text(&best_score_text, new_best_score_text);
        char new_best_david_text[128];
        sprintf(new_best_david_text, "Best David: #%d.%d", population.best_generation_number, population.best_id);
        update_text(&best_david_text, new_best_david_text);

        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (colliding_with_entity(population.davids[i]->entity, obstacle_top_post.entity) || colliding_with_entity(population.davids[i]->entity, obstacle_bottom_post.entity))
                terminate_david(population.davids[i]);
        }

        update_population(&population);
        
        SDL_SetRenderDrawColor(renderer, 144, 202, 249, 255);
        SDL_RenderClear(renderer);
        
        render_population(&population, render_hitboxes);
        render_post(&obstacle_top_post, render_hitboxes);
        render_post(&obstacle_bottom_post, render_hitboxes);
        render_entity(&jellyfish, render_hitboxes);
        render_text(&generation_text);
        render_text(&population_text);
        render_text(&best_score_text);
        render_text(&best_david_text);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/(population.generation_number > 1 ? 240 : 60));
    }

    eradicate_population(&population);
    SDL_DestroyTexture(obstacle_top_post.entity->texture);
    SDL_DestroyTexture(obstacle_bottom_post.entity->texture);
    SDL_DestroyTexture(jellyfish.texture);
    SDL_DestroyTexture(generation_text.texture);
    SDL_DestroyTexture(population_text.texture);
    SDL_DestroyTexture(best_score_text.texture);
    SDL_DestroyTexture(best_david_text.texture);
    
    TTF_Init();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}