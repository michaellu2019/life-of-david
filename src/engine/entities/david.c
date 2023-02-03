#include <math.h>

#include "david.h"

void construct_david(David* david, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                     float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed, 
                     char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window)
{
    construct_entity(entity, rect, x, y, w, h, acc_x, acc_y, acc_w, max_speed, max_angular_speed, texture_path, surface, texture, renderer, window);
    david->entity = entity;
    david->acc = sqrt(acc_x * acc_x + acc_y * acc_y);
}

void update_david(David* david)
{
    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);

    if (keyboard_state[SDL_SCANCODE_UP] || keyboard_state[SDL_SCANCODE_W])
    {
        david->entity->vel[0] -= david->acc * cos((david->entity->angle + 90.0) * PI/180);
        david->entity->vel[1] -= david->acc * sin((david->entity->angle + 90.0) * PI/180);
    }
    if (keyboard_state[SDL_SCANCODE_DOWN] || keyboard_state[SDL_SCANCODE_S])
    {
        david->entity->vel[0] += david->acc * cos((david->entity->angle + 90.0) * PI/180);
        david->entity->vel[1] += david->acc * sin((david->entity->angle + 90.0) * PI/180);
    }
    if (keyboard_state[SDL_SCANCODE_LEFT] || keyboard_state[SDL_SCANCODE_A])
    {
        david->entity->vel[2] -= david->entity->acc[2];
    }
    if (keyboard_state[SDL_SCANCODE_RIGHT] || keyboard_state[SDL_SCANCODE_D])
    {
        david->entity->vel[2] += david->entity->acc[2];
    }

    update_entity(david->entity);
}

void render_david(David* david, bool render_hitbox)
{
    render_entity(david->entity, render_hitbox);
}

void reset_david(David* david)
{
    reset_entity(david->entity);
}