
#include "window_process.h"

int8_t draw_conversation(SDL_Window *window,TTF_Font *font , char *bg_path, char *avatar_path, char *character_name, char *text)
{
    SDL_Renderer *renderer = NULL;
    if (window == NULL || bg_path == NULL || avatar_path == NULL || character_name == NULL || text == NULL || font == NULL)
    {
        return -1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        debug_print("can't create renderer.\n");
        return -1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    draw_background(renderer, bg_path);
    draw_avatar(renderer, font, avatar_path, character_name);
    draw_dialogue(renderer,font, text);
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    
    return 0;
}

int8_t draw_background(SDL_Renderer *renderer, char *bg_path)
{
    SDL_Surface *bg = NULL;
    SDL_Texture *bg_texture = NULL;
    if (renderer == NULL || bg_path == NULL)
    {
        return -1;
    }
    bg = IMG_Load(bg_path);
    if (bg == NULL)
    {
        debug_print("can't find bg.\n");
        return -1;
    }
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg);
    if (bg_texture == NULL)
    {
        debug_print("can't create bg_texture.\n");
        SDL_FreeSurface(bg);
        bg = NULL;
        return -1;
    }
    SDL_Rect bg_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect);
    SDL_DestroyTexture(bg_texture);
    bg_texture = NULL;
    SDL_FreeSurface(bg);
    bg = NULL;
    if (bg_texture != NULL || bg != NULL)
    {
        debug_print("error:%d,%d\n", bg_texture, bg);
    }
    return 0;
}

int8_t draw_dialogue(SDL_Renderer *renderer,TTF_Font *font, char *text)
{
    SDL_Surface *dialogue_surface = NULL;
    SDL_Texture *dialogue_texture = NULL;
    SDL_Rect dialogue_bg_rect = {AVATAR_WIDTH, WINDOW_HEIGHT - (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT), (WINDOW_WIDTH - AVATAR_WIDTH), (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT)};
    if (renderer == NULL || text == NULL || font == NULL)
    {
        return -1;
    }
    SDL_Color color = TEXT_COLOR;
    dialogue_surface = TTF_RenderUTF8_Solid(font, text, color);
    if (dialogue_surface == NULL)
    {
        debug_print("can't render text.\n");
        return -1;
    }
    dialogue_texture = SDL_CreateTextureFromSurface(renderer, dialogue_surface);
    if (dialogue_texture == NULL)
    {
        debug_print("can't create dialogue_texture.\n");
        SDL_FreeSurface(dialogue_surface);
        return -1;
    }
    SDL_Rect dialogue_rect = {AVATAR_WIDTH, WINDOW_HEIGHT - (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT), (WINDOW_WIDTH - AVATAR_WIDTH), (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT)};
    dialogue_rect.w = dialogue_surface->w;
    dialogue_rect.h = dialogue_surface->h;
    SDL_SetRenderDrawColor(renderer, 0, 105, 148, 0xAA);
    SDL_RenderFillRect(renderer, &dialogue_bg_rect);
    SDL_RenderCopy(renderer, dialogue_texture, NULL, &dialogue_rect);
    SDL_DestroyTexture(dialogue_texture);
    dialogue_texture = NULL;
    SDL_FreeSurface(dialogue_surface);
    dialogue_surface = NULL;
    if (dialogue_texture != NULL || dialogue_surface != NULL )
    {
        debug_print("error:%d,%d,%d\n", dialogue_texture, dialogue_surface);
    }
    return 0;
}

int8_t draw_avatar(SDL_Renderer *renderer,TTF_Font *font ,char *avatar_path, char *character_name)
{
    SDL_Surface *avatar = NULL;
    SDL_Surface *character_name_surface = NULL;
    SDL_Texture *avatar_texture = NULL;
    SDL_Texture *character_name_texture = NULL;
    SDL_Rect avatar_rect = {0, WINDOW_HEIGHT - AVATAR_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, AVATAR_HEIGHT};
    SDL_Rect character_name_bg_rect = {0, WINDOW_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, CHARACTER_NAME_BG_HEIGHT};
    if (renderer == NULL || avatar_path == NULL || character_name == NULL|| font == NULL)
    {
        return -1;
    }
    avatar = IMG_Load(avatar_path);
    if (avatar == NULL)
    {
        debug_print("can't find avater.\n");
        return -1;
    }
    SDL_Color color = TEXT_COLOR;
    character_name_surface = TTF_RenderUTF8_Solid(font, character_name, color);
    if (character_name_surface == NULL)
    {
        debug_print("can't create character_name_surface.\n");
        SDL_FreeSurface(avatar);
        avatar = NULL;
        return -1;
    }
    avatar_texture = SDL_CreateTextureFromSurface(renderer, avatar);
    if (avatar_texture == NULL)
    {
        debug_print("can't create avatar_texture.\n");
        SDL_FreeSurface(avatar);
        avatar = NULL;
        SDL_FreeSurface(character_name_surface);
        character_name_surface = NULL;
        return -1;
    }
    character_name_texture = SDL_CreateTextureFromSurface(renderer, character_name_surface);
    if (character_name_texture == NULL)
    {
        debug_print("can't create character_name_texture.\n");
        SDL_FreeSurface(avatar);
        avatar = NULL;
        SDL_FreeSurface(character_name_surface);
        character_name_surface = NULL;
        SDL_DestroyTexture(avatar_texture);
        avatar_texture = NULL;
        return -1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 244, 164, 96, 0xFF);
    SDL_RenderFillRect(renderer, &character_name_bg_rect);
    SDL_SetRenderDrawColor(renderer, 230, 127, 80, 0xFF);
    SDL_RenderFillRect(renderer, &avatar_rect);
    SDL_RenderCopy(renderer, avatar_texture, NULL, &avatar_rect);
    SDL_RenderCopy(renderer, character_name_texture, NULL, &character_name_bg_rect);
    SDL_FreeSurface(avatar);
    SDL_FreeSurface(character_name_surface);
    SDL_DestroyTexture(avatar_texture);
    SDL_DestroyTexture(character_name_texture);
    return 0;
}