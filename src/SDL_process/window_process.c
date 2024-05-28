
#include "window_process.h"

int8_t draw_conversation(SDL_Window *window, char *bg_path, char *avatar_path, char *character_name, char *text)
{
    int8_t error = 0;
    SDL_Rect bg_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect avatar_rect = {0, WINDOW_HEIGHT - AVATAR_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, AVATAR_HEIGHT};
    SDL_Rect dialogue_bg_rect = {AVATAR_WIDTH, WINDOW_HEIGHT - (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT), (WINDOW_WIDTH - AVATAR_WIDTH), (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT)};
    SDL_Rect character_name_bg_rect = {0, WINDOW_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, CHARACTER_NAME_BG_HEIGHT};
    SDL_Renderer *renderer = NULL;
    SDL_Surface *bg = NULL;
    SDL_Surface *avatar = NULL;
    SDL_Surface *dialogue_surface = NULL;
    SDL_Surface *character_name_surface = NULL;
    SDL_Texture *bg_texture = NULL;
    SDL_Texture *avatar_texture = NULL;
    SDL_Texture *dialogue_texture = NULL;
    SDL_Texture *character_name_texture = NULL;
    TTF_Font *font = NULL;
    if (window == NULL || bg_path == NULL || avatar_path == NULL || character_name == NULL || text == NULL)
    {
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        debug_print("can't create renderer\n");
        return -1;
    }
    bg = IMG_Load(bg_path);
    if (bg == NULL)
    {
        debug_print("can't find bg.\n");
        goto bg_err;
        error = -1;
    }
    avatar = IMG_Load(avatar_path);
    if (avatar == NULL)
    {
        debug_print("can't find avater.\n");
        goto avatar_err;
        error = -1;
    }
    font = TTF_OpenFont("./res/ttf/NotoSansTC-Medium.ttf", 24);
    if (font == NULL)
    {
        debug_print("can't find font.\n");
        goto font_err;
        error = -1;
    }
    SDL_Color color = {0, 0, 0, 0xFF};
    dialogue_surface = TTF_RenderText_Solid(font, text, color);
    if (dialogue_surface == NULL)
    {
        debug_print("can't render text.\n");
        goto dialogue_surface_err;
        error = -1;
    }
    character_name_surface = TTF_RenderText_Solid(font, character_name, color);
    if (character_name_surface == NULL)
    {
        debug_print("can't create character_name_surface.\n");
        goto character_name_surface_err;
        error = -1;
    }
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg);
    if (bg_texture == NULL)
    {
        debug_print("can't create bg_texture.\n");
        goto bg_texture_err;
        error = -1;
    }
    avatar_texture = SDL_CreateTextureFromSurface(renderer, avatar);
    if (avatar_texture == NULL)
    {
        debug_print("can't create avatar_texture.\n");
        goto avatar_texture_err;
        error = -1;
    }
    dialogue_texture = SDL_CreateTextureFromSurface(renderer, dialogue_surface);
    if (dialogue_texture == NULL)
    {
        debug_print("can't create dialogue_texture.\n");
        goto dialogue_texture_err;
        error = -1;
    }
    character_name_texture = SDL_CreateTextureFromSurface(renderer, character_name_surface);
    if (character_name_texture == NULL)
    {
        debug_print("can't create character_name_texture.\n");
        goto character_name_texture_err;
        error = -1;
    }
    SDL_Rect dialogue_rect = {AVATAR_WIDTH, WINDOW_HEIGHT - (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT), (WINDOW_WIDTH - AVATAR_WIDTH), (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT)};
    SDL_Rect character_name_rect = {0, WINDOW_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, CHARACTER_NAME_BG_HEIGHT};
    dialogue_rect.w = dialogue_surface->w;
    dialogue_rect.h = dialogue_surface->h;
    // character_name_rect.w = dialogue->w;
    character_name_rect.h = character_name_surface->h;
    SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect);
    SDL_RenderCopy(renderer, avatar_texture, NULL, &avatar_rect);
    SDL_RenderCopy(renderer, dialogue_texture, NULL, &dialogue_rect);
    SDL_RenderCopy(renderer,character_name_texture , NULL, &character_name_rect);
    SDL_RenderPresent(renderer);
// return 0;
SDL_DestroyTexture(character_name_texture);
character_name_texture = NULL;
character_name_texture_err:
    SDL_DestroyTexture(dialogue_texture);
    dialogue_texture = NULL;
dialogue_texture_err:
    SDL_DestroyTexture(avatar_texture);
    avatar_texture = NULL;
avatar_texture_err:
    SDL_DestroyTexture(bg_texture);
    bg_texture = NULL;
bg_texture_err:
    SDL_FreeSurface(character_name_surface);
    character_name_surface = NULL;
character_name_surface_err:
    SDL_FreeSurface(dialogue_surface);
    dialogue_surface = NULL;
dialogue_surface_err:
    TTF_CloseFont(font);
    font = NULL;
font_err:
    SDL_FreeSurface(avatar);
    avatar = NULL;
avatar_err:
    SDL_FreeSurface(bg);
    bg = NULL;
bg_err:
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    if(renderer != NULL || bg != NULL || avatar != NULL || font != NULL || dialogue_surface != NULL || character_name_surface != NULL || bg_texture != NULL || avatar_texture != NULL || dialogue_texture != NULL || character_name_texture != NULL)
    {
        debug_print("error:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",renderer,bg,avatar,font,dialogue_surface,character_name_surface,bg_texture,avatar_texture,dialogue_texture,character_name_texture);
    }
    return error;
}