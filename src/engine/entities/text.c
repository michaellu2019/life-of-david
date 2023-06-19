#include "text.h"

void construct_text(Text* text, SDL_Rect* rect, int x, int y, 
                    char* font_path, int font_size, char* message, SDL_Color* color,
                    SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window)
{
    TTF_Font* ttf_font = TTF_OpenFont(font_path, font_size);
    surface = TTF_RenderText_Solid(ttf_font, message, *color);
    if (!surface)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        ERROR_EXIT("Error creating renderer: %s\n", SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        ERROR_EXIT("Error creating texture: %s\n", SDL_GetError());
    }

    SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
    rect->w = strlen(message) * font_size/2;
    rect->h = font_size;
    rect->x = x;
    rect->y = y;

    text->rect = rect;
    text->ttf_font = ttf_font;
    text->font_path = font_path;
    text->font_size = font_size;
    text->message = message;
    text->color = color;
    text->surface = surface;
    text->renderer = renderer;
    text->texture = texture;
}

void update_text(Text* text, char* message)
{
    text->message = message;
    SDL_DestroyTexture(text->texture);
    text->surface = TTF_RenderText_Solid(text->ttf_font, message, *text->color);
    text->texture = SDL_CreateTextureFromSurface(text->renderer, text->surface);
    SDL_FreeSurface(text->surface);
    text->rect->w = strlen(message) * text->font_size/2;
}

void render_text(Text* text)
{
    SDL_RenderCopy(text->renderer, text->texture, NULL, text->rect);
}