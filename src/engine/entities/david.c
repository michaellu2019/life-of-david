#include <math.h>

#include "david.h"

void construct_david(David* david, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                     float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed, 
                     char* alive_texture_path, char* dead_texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window)
{
    construct_entity(entity, rect, x, y, w, h, acc_x, acc_y, acc_w, max_speed, max_angular_speed, alive_texture_path, surface, texture, renderer, window);
    david->entity = entity;
    david->acc = sqrt(acc_x * acc_x + acc_y * acc_y);
    david->fitness = 0;
    david->move_index = 0;
    david->alive_texture_path = alive_texture_path;
    david->dead_texture_path = dead_texture_path;
    generate_random_moves(david);
}

void update_david(David* david, bool teleop)
{
    if (!david->entity->alive)
        return; 

    if (david->entity->rect->x <= 0 || david->entity->rect->y <= 0 || 
        david->entity->rect->x + david->entity->rect->w > WINDOW_WIDTH || david->entity->rect->y + david->entity->rect->h > WINDOW_HEIGHT) {
        terminate_david(david);
    }
    // float x = david->entity->rect->x;
    // float y = david->entity->rect->y;
    // printf("%d %d %.2f %.2f\n", david->move_index, david->moves[david->move_index], x, y);
        
    const uint8_t *keyboard_state = teleop ? 0 : SDL_GetKeyboardState(NULL);

    if (david->moves[david->move_index] == UP || david->moves[david->move_index] == UP_AND_LEFT || david->moves[david->move_index] == UP_AND_RIGHT || 
        (teleop && keyboard_state[SDL_SCANCODE_UP] || keyboard_state[SDL_SCANCODE_W]))
    {
        david->entity->vel[0] -= david->acc * cos((david->entity->angle + 90.0) * PI/180);
        david->entity->vel[1] -= david->acc * sin((david->entity->angle + 90.0) * PI/180);
    }
    if (david->moves[david->move_index] == DOWN || david->moves[david->move_index] == DOWN_AND_LEFT || david->moves[david->move_index] == DOWN_AND_RIGHT || 
        (teleop && keyboard_state[SDL_SCANCODE_DOWN] || keyboard_state[SDL_SCANCODE_S]))
    {
        david->entity->vel[0] += david->acc * cos((david->entity->angle + 90.0) * PI/180);
        david->entity->vel[1] += david->acc * sin((david->entity->angle + 90.0) * PI/180);
    }
    if (david->moves[david->move_index] == LEFT || david->moves[david->move_index] == UP_AND_LEFT || david->moves[david->move_index] == DOWN_AND_LEFT || 
        (teleop && keyboard_state[SDL_SCANCODE_LEFT] || keyboard_state[SDL_SCANCODE_A]))
    {
        david->entity->vel[2] -= david->entity->acc[2];
    }
    if (david->moves[david->move_index] == RIGHT || david->moves[david->move_index] == UP_AND_RIGHT || david->moves[david->move_index] == DOWN_AND_RIGHT || 
        (teleop && keyboard_state[SDL_SCANCODE_RIGHT] || keyboard_state[SDL_SCANCODE_D]))
    {
        david->entity->vel[2] += david->entity->acc[2];
    }

    david->move_index++;
    if (david->move_index >= NUM_MOVES) {
        terminate_david(david);
    }

    update_entity(david->entity);
}

void render_david(David* david, bool render_hitbox)
{
    render_entity(david->entity, render_hitbox);
}

void generate_random_moves(David* david)
{
    for (int i = 0; i < NUM_MOVES; i++)
    {
        int rand_num = rand() % MOVE_SET_SIZE;
        david->moves[i] = rand_num;
    }
}

void reset_david(David* david)
{
    reset_entity(david->entity);
}

void terminate_david(David* david)
{
    david->entity->alive = false;
    david->entity->surface = IMG_Load(david->dead_texture_path);

    david->entity->texture = SDL_CreateTextureFromSurface(david->entity->renderer, david->entity->surface);
    SDL_FreeSurface(david->entity->surface);
}