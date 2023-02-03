#include "post.h"

void construct_post(Post* post, Entity* entity, SDL_Rect* rect, int x, int y, int w, int h, 
                    float acc_x, float acc_y, float acc_w, float max_speed, float max_angular_speed,
                    char* texture_path, SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window)
{
    construct_entity(entity, rect, x, y, w, h, acc_x, acc_y, acc_w, max_speed, max_angular_speed, texture_path, surface, texture, renderer, window);
    post->entity = entity;
}

void update_post(Post* post)
{
    update_entity(post->entity);
}

void render_post(Post* post, bool render_hitbox)
{
    render_entity(post->entity, render_hitbox);
}