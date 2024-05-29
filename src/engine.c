#include "basic_include.h"
#include "./toml_process/toml_get.h"
#include "./SDL_process/window_process.h"
#include "./SDL_process/init_all_SDL.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    debug_print("DEBUG MODE ON\n");
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
    toml_array_t *options = NULL;
    int8_t option_num = 0;
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
    toml_table_t *novel = toml_parse_file(novel_file, errbuf, sizeof(errbuf));
    if (novel == NULL)
    {
        debug_print("Error parsing file: %s\n", errbuf);
        return -1;
    }
    if (errbuf[0] != '\0')
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
        if (tmp_datum.ok)
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
        toml_table_t *save = toml_parse_file(save_file, errbuf, sizeof(errbuf));
        if (save == NULL)
        {
            debug_print("Error parsing file: %s\n", errbuf);
            return -1;
        }
        if (errbuf[0] != '\0')
        {
            debug_print("TOML Error: %s\n", errbuf);
            return -1;
        }
        tmp_datum = toml_string_in(save, "event");
        if (tmp_datum.ok)
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
        toml_free(save);
    }
    wait_key = 1;
    if (mySDL_init())
    {
        return -1;
    }
    char title[1024] = {0};
    tmp_datum = toml_string_in(novel, "name");
    if (tmp_datum.ok)
    {
        strncpy(title, tmp_datum.u.s, sizeof(title));
        free(tmp_datum.u.s);
    }
    else
    {
        debug_print("No title.\n");
        return -1;
    }
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    TTF_Font *font = TTF_OpenFont("./res/fonts/NotoSansTC-Medium.ttf", 24);
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
                TTF_Quit();
                SDL_Quit();
                toml_free(novel);
                return 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    if (wait_key)
                    {
                        if (change_status(novel, &stat, &next_stat, background_path, avatar_path, scene_name, character_name, dialogue_text, event_id, scene_id, character_id, dialogue_id, options, option_num) == -1)
                        {
                            debug_print("Error change status %d\n", stat);
                            debug_print("Quit.\n");
                            SDL_DestroyWindow(window);
                            TTF_CloseFont(font);
                            TTF_Quit();
                            SDL_Quit();
                            toml_free(novel);
                            return -1;
                        }
                        debug_print("Change status: %d\n", stat);
                        debug_print("Next status: %d\n", next_stat);
                        debug_print("Background path: %s\n", background_path);
                        debug_print("Avatar path: %s\n", avatar_path);
                        debug_print("Scene name: %s\n", scene_name);
                        debug_print("Character name: %s\n", character_name);
                        debug_print("Dialogue text: %s\n", dialogue_text);
                        debug_print("Event id: %s\n", event_id);
                        debug_print("Scene id: %s\n", scene_id);
                        debug_print("Character id: %s\n", character_id);
                        debug_print("Dialogue id: %s\n", dialogue_id);
                        debug_print("Option num: %d\n\n", option_num);

                        wait_key = 1;
                    }
                }
            }
        }
        if (stat == STATUS_DIALOGUE && wait_key == 0)
        {
            draw_conversation(window, font, "./res/img/bg.jpg", "./res/img/avatar.png", "王", "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
            wait_key = 1;
        }
    }
    return 0;
}