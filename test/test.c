#include "../src/SDL_porcess/init_all_SDL.h"
#include "../src/SDL_porcess/SDL_include.h"
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

    SDL_Texture *texture = IMG_LoadTexture(renderer, "test.png");
    if (texture == NULL)
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return -1;
    }
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_Rect rect1 = {150, 150, 200, 200};

    TTF_Font *notosans = TTF_OpenFont("../ttf/NotoSansTC-Medium.ttf", 32);
    SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    return 0;
}