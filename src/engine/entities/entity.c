#include "entity.h"

void construct_entity(Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                      float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed,
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
    entity->start_x = x;
    entity->start_y = y;
    entity->angle = 0.0;
    entity->dir[0] = 0;
    entity->dir[1] = 0;
    entity->vel[0] = 0;
    entity->vel[1] = 0;
    entity->vel[2] = 0;
    entity->acc[0] = acc_x;
    entity->acc[1] = acc_y;
    entity->acc[2] = acc_w;
    entity->max_speed = max_speed;
    entity->max_angular_speed = max_angular_speed;
    entity->renderer = renderer;
    entity->texture = texture;

    update_entity_hitbox_corners(entity);
}

void update_entity(Entity* entity)
{
    entity->vel[0] = entity->vel[0] < -entity->max_speed ? -entity->max_speed :
                     entity->vel[0] > entity->max_speed ? entity->max_speed :
                                                          entity->vel[0];
    entity->vel[1] = entity->vel[1] < -entity->max_speed ? -entity->max_speed :
                     entity->vel[1] > entity->max_speed ? entity->max_speed :
                                                          entity->vel[1];
    entity->vel[2] = entity->vel[2] < -entity->max_angular_speed ? -entity->max_angular_speed :
                     entity->vel[2] > entity->max_angular_speed ? entity->max_angular_speed :
                                                          entity->vel[2];
    
    entity->vel[0] *= FRICTION;
    entity->vel[1] *= FRICTION;
    entity->vel[2] *= FRICTION;

    entity->rect->x = entity->rect->x + entity->vel[0];
    entity->rect->y = entity->rect->y + entity->vel[1];
    entity->angle = entity->angle + entity->vel[2];

    update_entity_hitbox_corners(entity);
}

void render_entity(Entity* entity, bool render_hitbox)
{
    SDL_RenderCopyEx(entity->renderer, entity->texture, NULL, entity->rect, entity->angle, NULL, SDL_FLIP_NONE);
    
    if (render_hitbox)
    {
        SDL_SetRenderDrawColor(entity->renderer, 255, 0, 0, 255);
        for (int i = 0; i < 4; i++)
            SDL_RenderDrawLine(entity->renderer, entity->hitbox_corners[i][0], entity->hitbox_corners[i][1], entity->hitbox_corners[(i + 1) % 4][0], entity->hitbox_corners[(i + 1) % 4][1]);     
        SDL_SetRenderDrawColor(entity->renderer, 0, 0, 0, 255);
    }
}

void update_entity_hitbox_corners(Entity* entity)
{
    int center_x = entity->rect->x + entity->rect->w/2;
    int center_y = entity->rect->y + entity->rect->h/2;

    entity->hitbox_corners[0][0] = center_x - entity->rect->w/2;
    entity->hitbox_corners[0][1] = center_y - entity->rect->h/2;
    entity->hitbox_corners[1][0] = center_x + entity->rect->w/2;
    entity->hitbox_corners[1][1] = center_y - entity->rect->h/2;
    entity->hitbox_corners[2][0] = center_x + entity->rect->w/2;
    entity->hitbox_corners[2][1] = center_y + entity->rect->h/2;
    entity->hitbox_corners[3][0] = center_x - entity->rect->w/2; 
    entity->hitbox_corners[3][1] = center_y + entity->rect->h/2;

    float p_cos = cos(entity->angle * PI/180);
    float p_sin = sin(entity->angle * PI/180);
    for (int i = 0; i < 4; i++)
    {
        int cx = entity->hitbox_corners[i][0] - center_x;
        int cy = entity->hitbox_corners[i][1] - center_y;
        entity->hitbox_corners[i][0] = cx * p_cos - cy * p_sin + center_x;
        entity->hitbox_corners[i][1] = cx * p_sin + cy * p_cos + center_y;
    }
}

void reset_entity(Entity* entity)
{
    entity->rect->x = entity->start_x;
    entity->rect->y = entity->start_y;
    entity->angle = 0.0;
    entity->dir[0] = 0;
    entity->dir[1] = 0;
    entity->vel[0] = 0;
    entity->vel[1] = 0;
    entity->vel[2] = 0;
}