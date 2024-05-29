#include "basic_include.h"
#include "./toml_process/toml_get.h"
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
    TTF_Font *font = TTF_OpenFont("./res/fonts/NotoSansTC-Medium.ttf", 24);
    char background_path[1024] = {0};
    char avatar_path[1024] = {0};
    char scene_name[1024] = {0};
    char character_name[1024] = {0};
    char dialogue_text[1024] = {0};
    // init all id
    char event_id[1024] = {0};
    char scene_id[1024] = {0};
    char character_id[1024] = {0};
    char dialogue_id[1024] = {0};
    int8_t option_num=0;
    toml_datum_t tmp_datum;
    // set status
    enum status stat = 0;
    enum status next_stat = 0;
    int8_t wait_key = 0;
    FILE *novel_file = fopen("./res/novel.toml", "r");
    if (novel_file == NULL)
    {
        debug_print("No novel file.\n");
        return -1;
    }
    char errbuf[200] = {0};
    toml_table_t *novel = toml_parse_file(novel_file, errbuf , sizeof(errbuf));
    if (novel == NULL)
    {
        debug_print("Error parsing file: %s\n", errbuf);
        return -1;
    }
    if(errbuf[0] != '\0')
    {
        debug_print("TOML Error: %s\n", errbuf);
        return -1;
    }

    // read save file
    // TODO: save file
    // TODO: add object toml or json
    FILE *save_file = fopen("./res/save.toml", "r");
    if (save_file == NULL)
    {
        debug_print("No save file.\n");
        toml_table_t *start = toml_table_in(novel, "player");
        tmp_datum = toml_string_in(start, "starter");
        if(tmp_datum.ok)
        {
            strncpy(event_id, tmp_datum.u.s, sizeof(event_id));
            stat = STATUS_EVENT;
            free(tmp_datum.u.s);
        }
        else
        {
            debug_print("No starter.\n");
            return -1;
        }
    }
    else
    {
        toml_table_t *save = toml_parse_file(save_file, errbuf , sizeof(errbuf));
        if (save == NULL)
        {
            debug_print("Error parsing file: %s\n", errbuf);
            return -1;
        }
        if(errbuf[0] != '\0')
        {
            debug_print("TOML Error: %s\n", errbuf);
            return -1;
        }
        tmp_datum = toml_string_in(save, "event");
        if(tmp_datum.ok)
        {
            strncpy(event_id, tmp_datum.u.s, sizeof(event_id));
            stat = STATUS_EVENT;
            free(tmp_datum.u.s);
        }
        else
        {
            debug_print("No event.\n");
            return -1;
        }
        fclose(save_file);
    }

    // show
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                debug_print("Quit.\n");
                SDL_DestroyWindow(window);
                TTF_CloseFont(font);
                SDL_Quit();
                debug_print("Quit.\n");
                return 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    debug_print("Quit.\n");
                    SDL_DestroyWindow(window);
                    TTF_CloseFont(font);
                    SDL_Quit();
                    debug_print("Quit.\n");
                    return 0;
                }
            }
        }
        draw_conversation(window, font, "./res/img/bg.jpg", "./res/img/avatar.png", "王", "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
    }
    return 0;
}