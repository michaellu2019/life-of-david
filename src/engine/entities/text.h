#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../utils.h"
#include "../global.h"

typedef struct text {
    SDL_Rect* rect;
    TTF_Font* ttf_font;
    char* font_path;
    int font_size;
    char* message;
    SDL_Color* color;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
} Text;

void construct_text(Text* text, SDL_Rect* rect, int x, int y, 
                    char* font_path, int font_size, char* message, SDL_Color* color,
                    SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window);

void update_text(Text* text, char* message);

void render_text(Text* text);