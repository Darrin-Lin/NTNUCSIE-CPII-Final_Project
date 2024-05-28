#include "basic_include.h"
#include "./SDL_process/window_process.h"
#include "./SDL_process/init_all_SDL.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    debug_print("DEBUG MODE ON\n") if (mySDL_init())
    {
        return -1;
    }
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                debug_print("Quit.\n");
                SDL_DestroyWindow(window);
                SDL_Quit();
                debug_print("Quit.\n");
                return 0;
            }
        }
        draw_conversation(window, "./res/img/bg.jpg", "./res/img/avatar.png", "王小明", "test");
    }
    return 0;
}