#include "SDL_include.h"
#include "../basic_include.h"
#include "../debug.h"

#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define AVATAR_WIDTH 100
#define AVATAR_HEIGHT 100
#define CHARACTER_NAME_BG_HEIGHT 50
#define TEXT_COLOR {0, 0, 0, 0xFF}

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR {0xAA, 0xAA, 0xAA, 0xAA}
#define BUTTON_SELECT_COLOR_RED 0xAA
#define BUTTON_SELECT_COLOR_GREEN 0xFF
#define BUTTON_SELECT_COLOR_BLUE 0xFF
#define BUTTON_SELECT_COLOR_ALPHA 0xAA

int8_t draw_conversation(SDL_Renderer *renderer, TTF_Font *font, char *bg_path, char *avatar_path, char *character_name, char *text);
int8_t draw_options(SDL_Renderer *renderer, TTF_Font *font, char option_text[5][1024], int32_t option_num, int32_t option_select);
int8_t draw_background(SDL_Renderer *renderer, char *bg_path);
int8_t draw_dialogue(SDL_Renderer *renderer ,TTF_Font *font, char *text);
int8_t draw_avatar(SDL_Renderer *renderer, TTF_Font *font, char *avatar_path, char *character_name);