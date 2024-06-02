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
    // int8_t img_flag = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP | IMG_INIT_JXL | IMG_INIT_AVIF;
    int8_t img_flag = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP;
    if (!(IMG_Init(img_flag) & img_flag))
    {
        fprintf(stderr, "SDL_img init error! SDL_Error: %s\n", TTF_GetError());
        return -1;
    }
    int8_t mix_flag = MIX_INIT_MP3 | MIX_INIT_OGG;
    if (!(Mix_Init(mix_flag) & mix_flag))
    {
        fprintf(stderr, "SDL_mixer init error! SDL_Error: %s\n", Mix_GetError());
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "SDL_mixer open audio error! SDL_Error: %s\n", Mix_GetError());
        return -1;
    }
    return 0;
}