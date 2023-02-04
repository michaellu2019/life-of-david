#include <math.h>

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
            SDL_RenderDrawLine(entity->renderer, entity->hitbox_corners[i][0], entity->hitbox_corners[i][1], entity->hitbox_corners[(i + 1) % NUM_HITBOX_CORNERS][0], entity->hitbox_corners[(i + 1) % NUM_HITBOX_CORNERS][1]);     
        SDL_SetRenderDrawColor(entity->renderer, 0, 0, 0, 255);
    }
}

void update_entity_hitbox_corners(Entity* entity)
{
    int center_x = entity->rect->x + entity->rect->w/2;
    int center_y = entity->rect->y + entity->rect->h/2;

    entity->hitbox_corners[0][0] = (float) center_x - entity->rect->w/2;
    entity->hitbox_corners[0][1] = (float) center_y - entity->rect->h/2;
    entity->hitbox_corners[1][0] = (float) center_x + entity->rect->w/2;
    entity->hitbox_corners[1][1] = (float) center_y - entity->rect->h/2;
    entity->hitbox_corners[2][0] = (float) center_x + entity->rect->w/2;
    entity->hitbox_corners[2][1] = (float) center_y + entity->rect->h/2;
    entity->hitbox_corners[3][0] = (float) center_x - entity->rect->w/2; 
    entity->hitbox_corners[3][1] = (float) center_y + entity->rect->h/2;

    float p_cos = cos(entity->angle * PI/180);
    float p_sin = sin(entity->angle * PI/180);
    for (int i = 0; i < NUM_HITBOX_CORNERS; i++)
    {
        float cx = entity->hitbox_corners[i][0] - center_x;
        float cy = entity->hitbox_corners[i][1] - center_y;
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

bool is_point_collinear(float p[2], float q[2], float r[2])
{
    return q[0] <= fmax(p[0], r[0]) && q[0] >= fmin(p[0], r[0]) &&
           q[1] <= fmax(p[1], r[1]) && q[1] >= fmin(p[1], r[1]);

}

PointOrientation get_point_orientation(float p[2], float q[2], float r[2])
{
    float orientation_value = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1]);
    return orientation_value > 0 ? CLOCKWISE :
           orientation_value < 0 ? COUNTERCLOCKWISE :
                                   COLLINEAR;
}

// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool colliding_with_entity(Entity* entity_one, Entity* entity_two)
{
    for (int i = 0; i < NUM_HITBOX_CORNERS; i++)
    {
        for (int j = 0; j < NUM_HITBOX_CORNERS; j++)
        {
            float p1[2] = {entity_one->hitbox_corners[i][0], entity_one->hitbox_corners[i][1]};
            float q1[2] = {entity_one->hitbox_corners[(i + 1) % NUM_HITBOX_CORNERS][0], entity_one->hitbox_corners[(i + 1) % NUM_HITBOX_CORNERS][1]};
            float p2[2] = {entity_two->hitbox_corners[j][0], entity_two->hitbox_corners[j][1]};
            float q2[2] = {entity_two->hitbox_corners[(j + 1) % NUM_HITBOX_CORNERS][0], entity_two->hitbox_corners[(j + 1) % NUM_HITBOX_CORNERS][1]};
            
            PointOrientation o1 = get_point_orientation(p1, q1, p2);
            PointOrientation o2 = get_point_orientation(p1, q1, q2);
            PointOrientation o3 = get_point_orientation(p2, q2, p1);
            PointOrientation o4 = get_point_orientation(p2, q2, q1);

            if (o1 != o2 && o3 != o4)
                return true;
            if (o1 == COLLINEAR && is_point_collinear(p1, p2, q1))
                return true;
            if (o2 == COLLINEAR && is_point_collinear(p1, q2, q1))
                return true;
            if (o3 == COLLINEAR && is_point_collinear(p2, p1, q2))
                return true;
            if (o4 == COLLINEAR && is_point_collinear(p2, q1, q2))
                return true;
        }
    }

    return false;
}
