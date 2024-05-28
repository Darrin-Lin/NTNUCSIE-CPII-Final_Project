#include "SDL_include.h"
#include "../basic_include.h"
#include "../debug.h"

#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define AVATAR_WIDTH 100
#define AVATAR_HEIGHT 100
#define CHARACTER_NAME_BG_HEIGHT 50
#define TEXT_COLOR {0, 0, 0, 0xFF}

int8_t draw_conversation(SDL_Window *window, TTF_Font *font, char *bg_path, char *avatar_path, char *character_name, char *text);
int8_t draw_background(SDL_Renderer *renderer, char *bg_path);
int8_t draw_dialogue(SDL_Renderer *renderer ,TTF_Font *font, char *text);
int8_t draw_avatar(SDL_Renderer *renderer, TTF_Font *font, char *avatar_path, char *character_name);