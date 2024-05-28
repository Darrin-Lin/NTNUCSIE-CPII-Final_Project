#include "SDL_include.h"
#include "../basic_include.h"
#include "../debug.h"

#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define AVATAR_WIDTH 100
#define AVATAR_HEIGHT 100
// #define CHARACTER_NAME_BG_WIDTH 200
#define CHARACTER_NAME_BG_HEIGHT 50
// #define MESSAGE_BG_WIDTH 500
// #define MESSAGE_BG_HEIGHT 200


int8_t draw_conversation(SDL_Window *window, char *bg_path, char *avatar_path, char *character_name,char* text);