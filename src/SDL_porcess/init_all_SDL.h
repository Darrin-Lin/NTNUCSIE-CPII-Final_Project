#include "SDL_include.h"
#include "../basic_include.h"

#pragma once

int8_t mySDL_init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "SDL init error! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "SDL_ttf init error! SDL_Error: %s\n", TTF_GetError());
        return -1;
    }
    int8_t img_flag = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP | IMG_INIT_JXL | IMG_INIT_AVIF;
    if (!(IMG_Init(img_flag) & img_flag))
    {
        fprintf(stderr, "SDL_img init error! SDL_Error: %s\n", TTF_GetError());
        return -1;
    }
    return 0;
}