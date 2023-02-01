#include "entity.h"

void construct_entity(Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, float acc_x, float acc_y, float max_speed, 
                      char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window)
{
    surface = IMG_Load(texture_path);
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
    rect->w = w;
    rect->h = h;
    rect->x = x;
    rect->y = y;

    entity->rect = rect;
    entity->dir[0] = 0;
    entity->dir[1] = 0;
    entity->vel[0] = 0;
    entity->vel[1] = 0;
    entity->acc[0] = acc_x;
    entity->acc[1] = acc_y;
    entity->max_speed = max_speed;
    entity->renderer = renderer;
    entity->texture = texture;
}

void update_entity(Entity* entity)
{
        if (entity->vel[0] < -entity->max_speed) 
            entity->vel[0] = -entity->max_speed;
        if (entity->vel[0] > entity->max_speed)
            entity->vel[0] = entity->max_speed;
        
        entity->vel[0] *= FRICTION;
        entity->vel[1] += GRAVITY;
        
        entity->rect->x = (entity->rect->x + entity->vel[0]);
        entity->rect->y = (entity->rect->y + entity->vel[1]);
}

void render_entity(Entity* entity)
{
    SDL_RenderCopy(entity->renderer, entity->texture, NULL, entity->rect);
}