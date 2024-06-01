
#include "window_process.h"

int8_t draw_start_menu(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *font, char *bg_path, char *game_name)
{
    if (renderer == NULL || title_font == NULL || font == NULL || game_name == NULL)
    {
        return -1;
    }
    if (bg_path == NULL)
    {
        SDL_Color start_bg = DEFAULT_BG_COLOR;
        SDL_SetRenderDrawColor(renderer, start_bg.r, start_bg.g, start_bg.b, start_bg.a);
        SDL_Rect full_bg = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &full_bg);
    }
    else
    {
        draw_background(renderer, bg_path);
    }
    draw_title(renderer, title_font, game_name, TITLE_TOP);
    SDL_Surface *start_surface = NULL;
    SDL_Color color = TITLE_COLOR;
    SDL_Surface *start_text_surface = TTF_RenderUTF8_Blended_Wrapped(font, "Press [Space] to Start.", color, (WINDOW_WIDTH - 20));
    if (start_text_surface == NULL)
    {
        debug_print("can't create start_text_surface.\n");
        return -1;
    }
    SDL_Texture *start_texture = SDL_CreateTextureFromSurface(renderer, start_text_surface);
    if (start_texture == NULL)
    {
        debug_print("can't create start_texture.\n");
        SDL_FreeSurface(start_text_surface);
        return -1;
    }
    SDL_Rect start_rect = {10, WINDOW_HEIGHT - TITLE_HEIGHT * 1.5, WINDOW_WIDTH - 20, TITLE_HEIGHT * 0.5};
    SDL_Color start_bg_color = TITLE_BG_COLOR;
    SDL_SetRenderDrawColor(renderer, start_bg_color.r, start_bg_color.g, start_bg_color.b, start_bg_color.a);
    SDL_RenderFillRect(renderer, &start_rect);
    start_rect.w = start_text_surface->w;
    start_rect.h = start_text_surface->h;
    start_rect.x = WINDOW_WIDTH / 2 - start_text_surface->w / 2 + 20;
    start_rect.y += TITLE_HEIGHT * 0.5 / 2 - start_text_surface->h / 2;
    SDL_RenderCopy(renderer, start_texture, NULL, &start_rect);
    SDL_DestroyTexture(start_texture);
    start_texture = NULL;
    SDL_FreeSurface(start_text_surface);
    start_text_surface = NULL;
    return 0;
}

int8_t draw_conversation(SDL_Renderer *renderer, TTF_Font *font, char *bg_path, char *avatar_path, char *tachie_path, char *character_name, char *text)
{
    if (renderer == NULL || bg_path == NULL || avatar_path == NULL || tachie_path == NULL || character_name == NULL || text == NULL)
    {
        return -1;
    }
    draw_background(renderer, bg_path);
    draw_tachie(renderer, tachie_path);
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

int8_t draw_ending(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *font, char *bg_path, char *end_title, char *end_text)
{
    if (renderer == NULL || title_font == NULL || font == NULL || bg_path == NULL || end_title == NULL || end_text == NULL)
    {
        return -1;
    }
    draw_background(renderer, bg_path);
    SDL_Color wnd_bg = ENDING_BG_COLOR;
    SDL_SetRenderDrawColor(renderer, wnd_bg.r, wnd_bg.g, wnd_bg.b, wnd_bg.a);
    SDL_Rect full_bg = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &full_bg);
    draw_title(renderer, title_font, end_title, TITLE_TOP);
    SDL_Surface *end_surface = NULL;
    SDL_Color color = TEXT_COLOR;
    SDL_Surface *dialogue_surface = TTF_RenderUTF8_Blended_Wrapped(font, end_text, color, (WINDOW_WIDTH - 20));
    if (dialogue_surface == NULL)
    {
        debug_print("can't create dialogue_surface.\n");
        return -1;
    }
    SDL_Texture *dialogue_texture = SDL_CreateTextureFromSurface(renderer, dialogue_surface);
    if (dialogue_texture == NULL)
    {
        debug_print("can't create dialogue_texture.\n");
        SDL_FreeSurface(dialogue_surface);
        return -1;
    }
    SDL_Rect dialogue_rect = {10, TITLE_HEIGHT * 1.5, dialogue_surface->w, dialogue_surface->h};
    SDL_RenderCopy(renderer, dialogue_texture, NULL, &dialogue_rect);
    SDL_DestroyTexture(dialogue_texture);
    dialogue_texture = NULL;
    SDL_FreeSurface(dialogue_surface);
    dialogue_surface = NULL;
    return 0;
}

int8_t draw_title(SDL_Renderer *renderer, TTF_Font *title_font, char *title_text, enum title_position position)
{
    if (renderer == NULL || title_font == NULL || title_text == NULL)
    {
        return -1;
    }
    SDL_Surface *title_surface = NULL;
    SDL_Texture *title_texture = NULL;
    int32_t title_y = 0;
    if (position == TITLE_TOP)
    {
        title_y = TITLE_HEIGHT / 2;
    }
    else if (position == TITLE_CENTER)
    {
        title_y = WINDOW_HEIGHT / 2 - TITLE_HEIGHT / 2;
    }
    else if (position == TITLE_BOTTOM)
    {
        title_y = WINDOW_HEIGHT - TITLE_HEIGHT;
    }
    else
    {
        debug_print("title_position is invalid.\n");
        return -1;
    }
    SDL_Rect title_rect = {0, 0, WINDOW_WIDTH, TITLE_HEIGHT}; // just init
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
    SDL_Rect title_bg_rect = {0, title_y, WINDOW_WIDTH, TITLE_HEIGHT};
    SDL_Color title_bg_color = TITLE_BG_COLOR;
    SDL_SetRenderDrawColor(renderer, title_bg_color.r, title_bg_color.g, title_bg_color.b, title_bg_color.a);
    SDL_RenderFillRect(renderer, &title_bg_rect);
    title_rect.w = title_surface->w;
    title_rect.h = title_surface->h;
    title_rect.x = WINDOW_WIDTH / 2 - title_surface->w / 2;
    title_rect.y = title_y + TITLE_HEIGHT / 2 - title_surface->h / 2;
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
    if ((double)avatar->w / AVATAR_WIDTH > (double)avatar->h / AVATAR_HEIGHT)
    {
        avatar_rect.w = AVATAR_WIDTH;
        avatar_rect.h = (double)avatar->h / avatar->w * AVATAR_WIDTH;
        avatar_rect.y = WINDOW_HEIGHT - CHARACTER_NAME_BG_HEIGHT - avatar_rect.h;
    }
    else
    {
        avatar_rect.h = AVATAR_HEIGHT;
        avatar_rect.w = (double)avatar->w / avatar->h * AVATAR_HEIGHT;
        avatar_rect.x = AVATAR_WIDTH / 2 - avatar_rect.w / 2;
    }
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

int8_t draw_tachie(SDL_Renderer *renderer, char *tachie_path)
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
    SDL_Rect tachie_rect = {WINDOW_WIDTH / 2 - TACHIE_WIDTH / 2, WINDOW_HEIGHT - tachie->h * scale, TACHIE_WIDTH, tachie->h * scale};
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
int8_t draw_setting_bar(SDL_Renderer *renderer, TTF_Font *font, enum setting_bar_option option_choose)
{
    if (renderer == NULL || font == NULL)
    {
        return -1;
    }
    SDL_Rect setting_bar_rect = {0, 0, WINDOW_WIDTH, SETTING_BAR_HEIGHT};
    SDL_Color setting_bar_color = SETTING_BAR_BG_COLOR;
    SDL_SetRenderDrawColor(renderer, setting_bar_color.r, setting_bar_color.g, setting_bar_color.b, setting_bar_color.a);
    SDL_RenderFillRect(renderer, &setting_bar_rect);
    SDL_Surface *setting_bar_option_surface = NULL;
    SDL_Texture *setting_bar_option_texture = NULL;
    SDL_Rect setting_bar_option_rect = {0, 0, SETTING_BAR_WIDTH, SETTING_BAR_HEIGHT};
    char setting_bar_option_text[SETTING_BAR_OPTION_NUM][1024] = SETTING_BAR_OPTIONS;
    for (int32_t i = 0; i < SETTING_BAR_OPTION_NUM; i++)
    {
        SDL_Color text_color = TEXT_COLOR;
        SDL_Color bg_color = SETTING_BAR_COLOR;
        SDL_Rect setting_bar_option_rect = {0, 0, SETTING_BAR_WIDTH, SETTING_BAR_HEIGHT};
        SDL_Color bg_select_color = SETTING_BAR_SELECT_COLOR;
        setting_bar_option_rect.x = (WINDOW_WIDTH / 2 - SETTING_BAR_WIDTH * SETTING_BAR_OPTION_NUM / 2) + i * (SETTING_BAR_WIDTH);
        setting_bar_option_surface = TTF_RenderUTF8_Solid(font, setting_bar_option_text[i], text_color);
        if (setting_bar_option_surface == NULL)
        {
            debug_print("can't create setting_bar_option_surface.\n");
            return -1;
        }
        setting_bar_option_texture = SDL_CreateTextureFromSurface(renderer, setting_bar_option_surface);
        if (setting_bar_option_texture == NULL)
        {
            debug_print("can't create setting_bar_option_texture.\n");
            SDL_FreeSurface(setting_bar_option_surface);
            return -1;
        }
        if (i == option_choose)
        {
            SDL_SetRenderDrawColor(renderer, bg_select_color.r, bg_select_color.g, bg_select_color.b, bg_select_color.a);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        }
        SDL_RenderFillRect(renderer, &setting_bar_option_rect);
        setting_bar_option_rect.x += SETTING_BAR_WIDTH / 2 - setting_bar_option_surface->w / 2;
        setting_bar_option_rect.y = SETTING_BAR_HEIGHT / 2 - setting_bar_option_surface->h / 2;
        setting_bar_option_rect.w = setting_bar_option_surface->w;
        setting_bar_option_rect.h = setting_bar_option_surface->h;
        SDL_RenderCopy(renderer, setting_bar_option_texture, NULL, &setting_bar_option_rect);
    }
    return 0;
}

int8_t draw_bag(SDL_Renderer *renderer, TTF_Font *font, char items[MAX_ITEM_NUM][1024], char items_img_path[MAX_ITEM_NUM][1024], int32_t item_num, int32_t item_select)
{
    if (renderer == NULL || font == NULL || items == NULL || items_img_path == NULL || item_num == 0)
    {
        return -1;
    }
    if (item_num > MAX_ITEM_NUM)
    {
        debug_print("item_num is too large.\n");
        return -1;
    }
    int32_t item_height = WINDOW_HEIGHT / (MAX_ITEM_NUM/ITEM_COL_NUM);
    for (int32_t i = 0; i < MAX_ITEM_NUM ; i++)
    {
        SDL_Surface *item_surface = NULL;
        SDL_Texture *item_texture = NULL;
        SDL_Surface *item_img_surface = NULL;
        SDL_Texture *item_img_texture = NULL;
        int32_t row = i % (MAX_ITEM_NUM / ITEM_COL_NUM);
        int32_t col = i / (MAX_ITEM_NUM / ITEM_COL_NUM);
        SDL_Rect item_img_rect = {col * (WINDOW_WIDTH / ITEM_COL_NUM), row * item_height, item_height, item_height};
        SDL_Rect item_rect = {col * (WINDOW_WIDTH / ITEM_COL_NUM) + item_height, row * item_height, WINDOW_WIDTH / ITEM_COL_NUM - item_height, item_height};
        SDL_Color color = TEXT_COLOR;
        if (i < item_num)
        {
            item_surface = TTF_RenderUTF8_Blended_Wrapped(font, items[i], color, (WINDOW_WIDTH / ITEM_COL_NUM - item_height));
            if (item_surface == NULL)
            {
                debug_print("can't create item_surface.\n");
                return -1;
            }
            item_texture = SDL_CreateTextureFromSurface(renderer, item_surface);
            if (item_texture == NULL)
            {
                debug_print("can't create item_texture.\n");
                SDL_FreeSurface(item_surface);
                return -1;
            }
            item_img_surface = IMG_Load(items_img_path[i]);
            if (item_img_surface == NULL)
            {
                debug_print("can't create item_img_surface.\n");
                SDL_FreeSurface(item_surface);
                SDL_DestroyTexture(item_texture);
                return -1;
            }
            item_img_texture = SDL_CreateTextureFromSurface(renderer, item_img_surface);
            if (item_img_texture == NULL)
            {
                debug_print("can't create item_img_texture.\n");
                SDL_FreeSurface(item_surface);
                SDL_DestroyTexture(item_texture);
                SDL_FreeSurface(item_img_surface);
                return -1;
            }
        }
        SDL_Color item_bg_color = ITEM_COLOR;
        SDL_Color item_select_color = ITEM_SELECT_COLOR;
        SDL_Color item_img_bg_color = ITEM_IMG_BG_COLOR;
        SDL_SetRenderDrawColor(renderer, item_img_bg_color.r, item_img_bg_color.g, item_img_bg_color.b, item_img_bg_color.a);
        SDL_RenderFillRect(renderer, &item_img_rect);
        SDL_SetRenderDrawColor(renderer,item_select_color.r, item_select_color.g, item_select_color.b, item_select_color.a);
        SDL_RenderDrawRect(renderer, &item_img_rect);
        if (i == item_select)
        {
            item_bg_color = item_select_color;
        }
        SDL_SetRenderDrawColor(renderer, item_bg_color.r, item_bg_color.g, item_bg_color.b, item_bg_color.a);
        SDL_RenderFillRect(renderer, &item_rect);
        SDL_SetRenderDrawColor(renderer, item_select_color.r, item_select_color.g, item_select_color.b, item_select_color.a);
        SDL_RenderDrawRect(renderer, &item_rect);
        if (i < item_num)
        {
            item_rect.w = item_surface->w;
            item_rect.h = item_surface->h>item_height?item_height:item_surface->h;
            item_rect.x += (WINDOW_WIDTH / ITEM_COL_NUM - item_height) / 2 - item_surface->w / 2;
            item_rect.y = row* item_height + item_height / 2 - item_rect.h / 2;
            SDL_RenderCopy(renderer, item_texture, NULL, &item_rect);
            SDL_DestroyTexture(item_texture);
            item_texture = NULL;
            double scale = (double)item_height / item_img_surface->h;
            if (item_img_surface->w > item_img_surface->h)
            {
                scale = (double)item_height / item_img_surface->w;
            }
            item_img_rect.w = item_img_surface->w * scale;
            item_img_rect.h = item_img_surface->h * scale;
            item_img_rect.x = col * (WINDOW_WIDTH / ITEM_COL_NUM) + item_height / 2 - item_img_rect.w / 2;
            item_img_rect.y = row * item_height + item_height / 2 - item_img_rect.h / 2;
            SDL_RenderCopy(renderer, item_img_texture, NULL, &item_img_rect);
            SDL_DestroyTexture(item_img_texture);
            item_img_texture = NULL;
            SDL_FreeSurface(item_img_surface);
            item_img_surface = NULL;
            SDL_FreeSurface(item_surface);
            item_surface = NULL;
        }
        if (item_texture != NULL || item_surface != NULL)
        {
            debug_print("error:%d,%d,%d\n", item_texture, item_surface);
        }
    }
    return 0;
}