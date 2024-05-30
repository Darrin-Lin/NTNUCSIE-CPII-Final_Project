
#include "window_process.h"

int8_t draw_conversation(SDL_Renderer *renderer, TTF_Font *font, char *bg_path, char *avatar_path,char *tachie_path, char *character_name, char *text)
{
    if (renderer == NULL || bg_path == NULL || avatar_path == NULL || tachie_path == NULL || character_name == NULL || text == NULL)
    {
        return -1;
    }
    draw_background(renderer, bg_path);
    draw_tachie(renderer,tachie_path);
    draw_avatar(renderer, font, avatar_path, character_name);
    draw_dialogue(renderer, font, text);
    return 0;
}
int8_t draw_options(SDL_Renderer *renderer, TTF_Font *font, char option_text[5][1024], int32_t option_num, int32_t option_select)
{
    if (renderer == NULL || font == NULL || option_text == NULL || option_num == 0)
    {
        return -1;
    }
    if (option_num > 5)
    {
        debug_print("option_num is too large.\n");
        return -1;
    }
    if (option_select >= option_num)
    {
        debug_print("option_selec is too large.\n");
        return -1;
    }
    if (option_select < 0)
    {
        debug_print("option_selec is too small.\n");
        return -1;
    }

    for (int32_t i = 0; i < option_num; i++)
    {
        SDL_Surface *option_surface = NULL;
        SDL_Texture *option_texture = NULL;
        SDL_Rect option_rect = {WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2, (2 * i + 1) * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
        SDL_Color color = TEXT_COLOR;
        option_surface = TTF_RenderUTF8_Solid(font, option_text[i], color);
        if (option_surface == NULL)
        {
            debug_print("can't create option_surface.\n");
            return -1;
        }
        option_texture = SDL_CreateTextureFromSurface(renderer, option_surface);
        if (option_texture == NULL)
        {
            debug_print("can't create option_texture.\n");
            SDL_FreeSurface(option_surface);
            return -1;
        }
        SDL_Color button_color = BUTTON_COLOR;
        if (i == option_select)
        {
            button_color.r = BUTTON_SELECT_COLOR_RED;
            button_color.g = BUTTON_SELECT_COLOR_GREEN;
            button_color.b = BUTTON_SELECT_COLOR_BLUE;
            button_color.a = BUTTON_SELECT_COLOR_ALPHA;
        }
        SDL_SetRenderDrawColor(renderer, button_color.r, button_color.g, button_color.b, button_color.a);
        SDL_RenderFillRect(renderer, &option_rect);
        option_rect.w = option_surface->w;
        option_rect.h = option_surface->h;
        option_rect.x = WINDOW_WIDTH / 2 - option_surface->w / 2;
        option_rect.y = (2 * i + 1) * BUTTON_HEIGHT + BUTTON_HEIGHT / 2 - option_surface->h / 2;
        SDL_RenderCopy(renderer, option_texture, NULL, &option_rect);
        SDL_DestroyTexture(option_texture);
        option_texture = NULL;
        SDL_FreeSurface(option_surface);
        option_surface = NULL;

        if (option_texture != NULL || option_surface != NULL)
        {
            debug_print("error:%d,%d,%d\n", option_texture, option_surface);
        }
    }
    return 0;
}

int8_t draw_title(SDL_Renderer *renderer, TTF_Font *title_font, char *title_text)
{
    if (renderer == NULL || title_font == NULL || title_text == NULL)
    {
        return -1;
    }
    SDL_Surface *title_surface = NULL;
    SDL_Texture *title_texture = NULL;
    SDL_Rect title_rect = {0, WINDOW_HEIGHT / 2 - TITLE_HEIGHT / 2, WINDOW_WIDTH, TITLE_HEIGHT};
    SDL_Color color = TITLE_COLOR;
    title_surface = TTF_RenderUTF8_Solid(title_font, title_text, color);
    if (title_surface == NULL)
    {
        debug_print("can't create title_surface.\n");
        return -1;
    }
    title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    if (title_texture == NULL)
    {
        debug_print("can't create title_texture.\n");
        SDL_FreeSurface(title_surface);
        return -1;
    }
    SDL_Rect title_bg_rect = {0, WINDOW_HEIGHT / 2 - TITLE_HEIGHT / 2, WINDOW_WIDTH, TITLE_HEIGHT};
    SDL_Color title_bg_color = TITLE_BG_COLOR;
    SDL_SetRenderDrawColor(renderer, title_bg_color.r, title_bg_color.g, title_bg_color.b, title_bg_color.a);
    SDL_RenderFillRect(renderer, &title_bg_rect);
    title_rect.w = title_surface->w;
    title_rect.h = title_surface->h;
    title_rect.x = WINDOW_WIDTH / 2 - title_surface->w / 2;
    title_rect.y = WINDOW_HEIGHT / 2 - title_surface->h / 2;
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_DestroyTexture(title_texture);
    title_texture = NULL;
    SDL_FreeSurface(title_surface);
    title_surface = NULL;
    if (title_texture != NULL || title_surface != NULL)
    {
        debug_print("error:%d,%d,%d\n", title_texture, title_surface);
    }
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

int8_t draw_dialogue(SDL_Renderer *renderer, TTF_Font *font, char *text)
{
    SDL_Surface *dialogue_surface = NULL;
    SDL_Texture *dialogue_texture = NULL;
    SDL_Rect dialogue_bg_rect = {AVATAR_WIDTH, WINDOW_HEIGHT - (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT), (WINDOW_WIDTH - AVATAR_WIDTH), (CHARACTER_NAME_BG_HEIGHT + AVATAR_HEIGHT)};
    if (renderer == NULL || text == NULL || font == NULL)
    {
        return -1;
    }
    SDL_Color color = TEXT_COLOR;
    dialogue_surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, (WINDOW_WIDTH - AVATAR_WIDTH - 10));
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
    if (dialogue_texture != NULL || dialogue_surface != NULL)
    {
        debug_print("error:%d,%d,%d\n", dialogue_texture, dialogue_surface);
    }
    return 0;
}

int8_t draw_avatar(SDL_Renderer *renderer, TTF_Font *font, char *avatar_path, char *character_name)
{
    SDL_Surface *avatar = NULL;
    SDL_Surface *character_name_surface = NULL;
    SDL_Texture *avatar_texture = NULL;
    SDL_Texture *character_name_texture = NULL;
    SDL_Rect avatar_rect = {0, WINDOW_HEIGHT - AVATAR_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, AVATAR_HEIGHT};
    SDL_Rect character_name_bg_rect = {0, WINDOW_HEIGHT - CHARACTER_NAME_BG_HEIGHT, AVATAR_WIDTH, CHARACTER_NAME_BG_HEIGHT};
    if (renderer == NULL || avatar_path == NULL || character_name == NULL || font == NULL)
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
    character_name_bg_rect.w = character_name_surface->w < AVATAR_WIDTH ? character_name_surface->w : AVATAR_WIDTH;
    character_name_bg_rect.h = character_name_surface->h;
    character_name_bg_rect.x = AVATAR_WIDTH / 2 - character_name_bg_rect.w / 2;
    character_name_bg_rect.y = WINDOW_HEIGHT - CHARACTER_NAME_BG_HEIGHT;
    SDL_RenderCopy(renderer, character_name_texture, NULL, &character_name_bg_rect);
    SDL_FreeSurface(avatar);
    SDL_FreeSurface(character_name_surface);
    SDL_DestroyTexture(avatar_texture);
    SDL_DestroyTexture(character_name_texture);
    return 0;
}

int8_t draw_tachie(SDL_Renderer *renderer,char *tachie_path)
{
    SDL_Surface *tachie = NULL;
    SDL_Texture *tachie_texture = NULL;
    if (renderer == NULL || tachie_path == NULL)
    {
        return -1;
    }
    tachie = IMG_Load(tachie_path);
    if (tachie == NULL)
    {
        debug_print("can't find tachie.\n");
        return -1;
    }
    tachie_texture = SDL_CreateTextureFromSurface(renderer, tachie);
    if (tachie_texture == NULL)
    {
        debug_print("can't create tachie_texture.\n");
        SDL_FreeSurface(tachie);
        tachie = NULL;
        return -1;
    }
    double scale = (double)TACHIE_WIDTH / tachie->w;
    SDL_Rect tachie_rect = {WINDOW_WIDTH/2 - TACHIE_WIDTH/2, WINDOW_HEIGHT - tachie->h * scale, TACHIE_WIDTH, tachie->h * scale};
    SDL_RenderCopy(renderer, tachie_texture, NULL, &tachie_rect);
    SDL_DestroyTexture(tachie_texture);
    tachie_texture = NULL;
    SDL_FreeSurface(tachie);
    tachie = NULL;
    if (tachie_texture != NULL || tachie != NULL)
    {
        debug_print("error:%d,%d\n", tachie_texture, tachie);
    }
    return 0;
}