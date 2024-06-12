#include "SDL_include.h"
#include "../basic_include.h"
#include "../debug.h"
#include "../cJSON_process/save_process.h"

#pragma once

#define DEFAULT_BG_COLOR       \
    {                          \
        0xFF, 0xFF, 0xFF, 0xFF \
    }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define AVATAR_WIDTH 150
#define AVATAR_HEIGHT 150
#define CHARACTER_NAME_BG_HEIGHT 50
#define TACHIE_WIDTH 400
#define TEXT_COLOR    \
    {                 \
        0, 0, 0, 0xFF \
    }

#define BUTTON_WIDTH 800
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR           \
    {                          \
        0xAA, 0xAA, 0xAA, 0xAA \
    }
#define BUTTON_SELECT_COLOR_RED 0xAA
#define BUTTON_SELECT_COLOR_GREEN 0xFF
#define BUTTON_SELECT_COLOR_BLUE 0xFF
#define BUTTON_SELECT_COLOR_ALPHA 0xAA

#define TITLE_HEIGHT 100
#define TITLE_COLOR            \
    {                          \
        0xFF, 0xFF, 0xFF, 0xFF \
    }
#define TITLE_BG_COLOR    \
    {                     \
        0, 105, 148, 0xAA \
    }

#define ENDING_BG_COLOR        \
    {                          \
        0xAA, 0xAA, 0xAA, 0xAA \
    }

#define SETTING_BAR_OPTIONS                         \
    {                                               \
        "Help", "Save", "Bag", "Characters", "Exit" \
    }
#define SETTING_BAR_OPTION_NUM 5
#define SETTING_BAR_HEIGHT 30
#define SETTING_BAR_WIDTH 150
#define SETTING_BAR_COLOR      \
    {                          \
        0xAA, 0xAA, 0xAA, 0xAA \
    }
#define SETTING_BAR_SELECT_COLOR \
    {                            \
        0xFF, 0xFF, 0xFF, 0xFF   \
    }
#define SETTING_BAR_BG_COLOR   \
    {                          \
        0x00, 0x00, 0xAA, 0xFF \
    }
#define ITEM_COL_NUM 2
#define ITEM_COLOR             \
    {                          \
        0xAA, 0xAA, 0xAA, 0xAA \
    }
#define ITEM_SELECT_COLOR      \
    {                          \
        0xFF, 0xFF, 0xFF, 0xFF \
    }
#define ITEM_IMG_BG_COLOR      \
    {                          \
        0xAA, 0xAA, 0xAA, 0xFF \
    }

#define ITEM_GET_IMG_WIDTH 400
#define ITEM_GET_BG_COLOR      \
    {                          \
        0xAA, 0xAA, 0xAA, 0xFF \
    }
#define ITEM_GET_TITLE_COLOR   \
    {                          \
        0x66, 0x66, 0x66, 0xAA \
    }
#define ITEM_GET_TEXT_COLOR    \
    {                          \
        0x00, 0x00, 0x00, 0xFF \
    }

#define CHARACTERS_COL_NUM 3
#define CHARACTERS_COLOR       \
    {                          \
        0xAA, 0xAA, 0xAA, 0xAA \
    }
#define CHARACTERS_SELECT_COLOR \
    {                           \
        0xFF, 0xFF, 0xFF, 0xFF  \
    }
#define CHARACTERS_BG_COLOR    \
    {                          \
        0x00, 0x00, 0xAA, 0xFF \
    }
#define CHARACTERS_FAVORABILITY_COLOR \
    {                           \
        0xFF, 0xAA, 0xAA, 0xFF  \
    }
#define HELP_BG_COLOR          \
    {                          \
        0xAA, 0xAA, 0xAA, 0xFF \
    }
#define HELP_MESSAGE_NUM 7
#define HELP_MESSAGES {"[SPACE] : continue, select option", "[ESC] : back to novel page", "[↑] : option select up", "[↓] : option select down", "[ENTER] : setting bar select","[→] : setting bar option right","[←] : setting bar option left"}
#define ANIMATION_FRAME_STEP 16
#define ANIMATION_DELAY 2

enum title_position
{
    TITLE_TOP = 0,
    TITLE_CENTER,
    TITLE_BOTTOM
};

enum setting_bar_option
{
    SETTING_BAR_OPTION_HELP = 0,
    SETTING_BAR_OPTION_SAVE,
    SETTING_BAR_OPTION_BAG,
    SETTING_BAR_OPTION_CHARACTERS,
    SETTING_BAR_OPTION_EXIT
};
int8_t draw_start_menu(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *font, char *bg_path, char *game_name);
int8_t draw_conversation(SDL_Renderer *renderer, TTF_Font *font, char *bg_path, char *avatar_path, char *tachie_path, char *character_name, char *text);
int8_t draw_options(SDL_Renderer *renderer, TTF_Font *font, char option_text[5][1024], int32_t option_num, int32_t option_select);
int8_t draw_title(SDL_Renderer *renderer, TTF_Font *title_font, char *title_text, enum title_position position);
int8_t draw_ending(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *font, char *bg_path, char *end_title, char *end_text);
int8_t draw_background(SDL_Renderer *renderer, char *bg_path);
int8_t draw_dialogue(SDL_Renderer *renderer, TTF_Font *font, char *text);
int8_t draw_avatar(SDL_Renderer *renderer, TTF_Font *font, char *avatar_path, char *character_name);
int8_t draw_tachie(SDL_Renderer *renderer, char *tachie_path);
int8_t draw_setting_bar(SDL_Renderer *renderer, TTF_Font *font, enum setting_bar_option option_choose);
int8_t draw_bag(SDL_Renderer *renderer, TTF_Font *font, char items[MAX_ITEM_NUM][1024], char items_img_path[MAX_ITEM_NUM][2048], int32_t item_num, int32_t item_select);
int8_t draw_item_get(SDL_Renderer *renderer, TTF_Font *title_font, char *item_name, char *item_img_path);
int8_t draw_favorability(SDL_Renderer *renderer, TTF_Font *font, char characters_name[MAX_CHARACTER_NUM][1024], char characters_img_path[MAX_CHARACTER_NUM][2048],int32_t characters_favorability[MAX_CHARACTER_NUM] , int32_t characters_num, int32_t char_select);
int8_t draw_help(SDL_Renderer *renderer,TTF_Font *font);
int8_t draw_animation(SDL_Renderer *renderer, char *background_path, int8_t reload);