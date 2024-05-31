#include "SDL_include.h"
#include "../basic_include.h"
#include "../debug.h"

#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define AVATAR_WIDTH 150
#define AVATAR_HEIGHT 150
#define CHARACTER_NAME_BG_HEIGHT 50
#define TACHIE_WIDTH 400
#define TEXT_COLOR {0, 0, 0, 0xFF}


#define BUTTON_WIDTH 800
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR {0xAA, 0xAA, 0xAA, 0xAA}
#define BUTTON_SELECT_COLOR_RED 0xAA
#define BUTTON_SELECT_COLOR_GREEN 0xFF
#define BUTTON_SELECT_COLOR_BLUE 0xFF
#define BUTTON_SELECT_COLOR_ALPHA 0xAA

#define TITLE_HEIGHT 100
#define TITLE_COLOR {0xFF, 0xFF, 0xFF, 0xFF}
#define TITLE_BG_COLOR {0, 105, 148, 0xAA}

#define ENDING_BG_COLOR {0xAA, 0xAA, 0xAA, 0xAA}

#define SETTING_BAR_OPTIONS {"Save", "Bag", "Characters", "Exit"}
#define SETTING_BAR_OPTION_NUM 4
#define SETTING_BAR_HEIGHT 30
#define SETTING_BAR_WIDTH 150
#define SETTING_BAR_COLOR {0xAA, 0xAA, 0xAA, 0xAA}
#define SETTING_BAR_SELECT_COLOR {0xFF, 0xFF, 0xFF, 0xFF}
#define SETTING_BAR_BG_COLOR {0x00,0x00,0xAA,0xFF}

enum title_position
{
    TITLE_TOP = 0,
    TITLE_CENTER,
    TITLE_BOTTOM
};

enum setting_bar_option
{
    SETTING_BAR_OPTION_SAVE = 0,
    SETTING_BAR_OPTION_BAG,
    SETTING_BAR_OPTION_CHARACTERS,
    SETTING_BAR_OPTION_EXIT
};

int8_t draw_conversation(SDL_Renderer *renderer, TTF_Font *font, char *bg_path, char *avatar_path,char *tachie_path, char *character_name, char *text);
int8_t draw_options(SDL_Renderer *renderer, TTF_Font *font, char option_text[5][1024], int32_t option_num, int32_t option_select);
int8_t draw_title(SDL_Renderer *renderer, TTF_Font *title_font, char *title_text, enum title_position position);
int8_t draw_ending(SDL_Renderer *renderer, TTF_Font *title_font, TTF_Font *font, char *bg_path, char *end_title, char *end_text);
int8_t draw_background(SDL_Renderer *renderer, char *bg_path);
int8_t draw_dialogue(SDL_Renderer *renderer ,TTF_Font *font, char *text);
int8_t draw_avatar(SDL_Renderer *renderer, TTF_Font *font, char *avatar_path, char *character_name);
int8_t draw_tachie(SDL_Renderer *renderer,char *tachie_path);
int8_t draw_setting_bar(SDL_Renderer *renderer, TTF_Font *font, enum setting_bar_option option_choose);