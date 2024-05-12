#include "../src/SDL_porcess/init_all_SDL.h"
#include "../src/SDL_porcess/SDL_include.h"
#define DEBUG
#include "../src/debug.h"
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, NULL, destination, &offset);
    return;
}

int main(int argc, char *argv[])
{
    if (mySDL_init())
    {
        return -1;
    }
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    SDL_Surface *message = NULL;
    SDL_Texture *texture = IMG_LoadTexture(renderer, "test.png");
    if (texture == NULL)
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return -1;
    }
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_Rect rect1 = {150, 150, 200, 200};

    TTF_Font *font = TTF_OpenFont("../ttf/NotoSansTC-Medium.ttf", 32);
    if(font==NULL)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        debug_print("font is NULL");
        return -1;
    }
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};

    message = TTF_RenderText_Solid(font, "The quick brown fox jumps over the lazy dog", color);
    if (message == NULL)
    {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        debug_print("message is NULL");
        return -1;
    }
    SDL_Texture *mTexture;
    mTexture = SDL_CreateTextureFromSurface(renderer, message);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_DestroyTexture(mTexture);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_DestroyTexture(message);
                TTF_CloseFont(font);
                SDL_Quit();
                return 0;
            }
        }
        // SDL_RenderClear(renderer);
        apply_surface(0, 150, message, window);
        SDL_RenderCopy(renderer,mTexture,NULL,&rect1);  
        SDL_RenderCopy(renderer, texture, NULL, &rect1);  
        // SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    return 0;
}