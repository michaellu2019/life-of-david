#include "david.h"

void construct_david(David* david, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, float acc_x, float acc_y, float max_speed, 
                     char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window)
{
    construct_entity(entity, rect, x, y, w, h, acc_x, acc_y, max_speed, texture_path, surface, texture, renderer, window);
    david->entity = entity;
    david->jumping = false;
}

void update_david(David* david)
{
    david->entity->dir[0] = 0;
    david->entity->dir[1] = 0;

    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);

    if (keyboard_state[SDL_SCANCODE_UP] || keyboard_state[SDL_SCANCODE_W])
    {
        david->entity->dir[1] = -1;
        if (!david->jumping)
        {
            david->entity->vel[1] = david->entity->acc[1];
            david->jumping = true;
        }
    }
    if (keyboard_state[SDL_SCANCODE_DOWN] || keyboard_state[SDL_SCANCODE_S])
    {
        david->entity->dir[1] = 1;
    }
    if (keyboard_state[SDL_SCANCODE_LEFT] || keyboard_state[SDL_SCANCODE_A])
    {
        david->entity->dir[0] = -1;
        david->entity->vel[0] -= david->entity->acc[0];
    }
    if (keyboard_state[SDL_SCANCODE_RIGHT] || keyboard_state[SDL_SCANCODE_D])
    {
        david->entity->dir[0] = 1;
        david->entity->vel[0] += david->entity->acc[0];
    }
    update_entity(david->entity);

    if (david->entity->rect->y > GROUND_Y - david->entity->rect->h)
    {
        david->entity->rect->y = GROUND_Y - david->entity->rect->h;
        david->jumping = false;
    }
}

void render_david(David* david)
{
    render_entity(david->entity);
}