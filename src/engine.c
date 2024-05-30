#include "basic_include.h"
#include "./toml_process/toml_get.h"
#include "./SDL_process/window_process.h"
#include "./SDL_process/init_all_SDL.h"
#include "debug.h"

SDL_Window *window;
TTF_Font *font;
TTF_Font *title_font;
toml_table_t *novel;

void close_SDL()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void close_TTF()
{
    TTF_CloseFont(font);
    TTF_CloseFont(title_font);
    TTF_Quit();
}
void close_TOML()
{
    toml_free(novel);
}

int main(int argc, char *argv[])
{
    debug_print("DEBUG MODE ON\n");
    char background_path[1024] = {0};
    char avatar_path[1024] = {0};
    char tachie_path[1024] = {0};
    char scene_name[1024] = {0};
    char character_name[1024] = {0};
    char dialogue_text[1024] = {0};
    char end_text[4096] = {0};
    // init all id
    char event_id[1024] = {0};
    char scene_id[1024] = {0};
    char character_id[1024] = {0};
    char dialogue_id[1024] = {0};
    char end_id[1024] = {0};

    toml_array_t *options = NULL;
    char option_text[5][1024] = {0};
    int32_t option_num = 0;
    int32_t option_choose = 0;
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
    novel = toml_parse_file(novel_file, errbuf, sizeof(errbuf));
    if (novel == NULL)
    {
        debug_print("Error parsing file: %s\n", errbuf);
        return -1;
    }
    atexit(close_TOML);
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
            tmp_datum.ok = 0;
            tmp_datum.u.s = NULL;
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
            tmp_datum.ok = 0;
            tmp_datum.u.s = NULL;
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
        tmp_datum.ok = 0;
        tmp_datum.u.s = NULL;
    }
    else
    {
        debug_print("No title.\n");
        return -1;
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        debug_print("Can't create window.\n");
        return -1;
    }
    atexit(close_SDL);
    font = TTF_OpenFont("./res/fonts/NotoSansTC-Medium.ttf", 24);
    if (font == NULL)
    {
        debug_print("can't open font.\n");
        return -1;
    }
    title_font = TTF_OpenFont("./res/fonts/NotoSansTC-Medium.ttf", 48);
    atexit(close_TTF);
    // show
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                debug_print("Quit.\n");
                return 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    if (wait_key)
                    {
                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            option_choose = 0;
                            option_num = 0;
                            toml_table_t *options_txt = toml_table_at(options, option_choose);
                            if (options_txt == NULL)
                            {
                                debug_print("No option.\n");
                                return -1;
                            }
                            if (next_stat == STATUS_EVENT)
                            {
                                tmp_datum = toml_string_in(options_txt, "event");
                                if (tmp_datum.ok)
                                {
                                    strncpy(event_id, tmp_datum.u.s, sizeof(event_id));
                                    free(tmp_datum.u.s);
                                    tmp_datum.ok = 0;
                                    tmp_datum.u.s = NULL;
                                }
                                else
                                {
                                    debug_print("No next.\n");
                                    return -1;
                                }
                            }
                            else if (next_stat == STATUS_DIALOGUE)
                            {
                                tmp_datum = toml_string_in(options_txt, "next");
                                if (tmp_datum.ok)
                                {
                                    strncpy(dialogue_id, tmp_datum.u.s, sizeof(dialogue_id));
                                    free(tmp_datum.u.s);
                                    tmp_datum.ok = 0;
                                    tmp_datum.u.s = NULL;
                                }
                                else
                                {
                                    debug_print("No next.\n");
                                    return -1;
                                }
                            }
                            else
                            {
                                debug_print("No next.\n");
                                return -1;
                            }
                        }
                        int8_t change = change_status(novel, &stat, &next_stat, background_path, avatar_path, tachie_path, scene_name, character_name, dialogue_text, end_text, event_id, scene_id, character_id, dialogue_id, end_id, &options, option_choose);
                        if (change == -1)
                        {
                            debug_print("Error change status %d\n", stat);
                            debug_print("Quit.\n");
                            return -1;
                        }
                        else if (change == 1)
                        {
                            debug_print("End.\n");
                            return 0;
                        }
                        
                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            option_num = toml_array_nelem(options);
                            if (option_num > 5)
                            {
                                debug_print("Quit.\n");
                                return -1;
                            }
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
                        debug_print("Option choose: %d\n\n", option_choose);

                        wait_key = 1;
                    }
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    debug_print("UP key press.\n");
                    if (stat == STATUS_DIALOGUE_OPTION)
                    {
                        option_choose = option_choose == 0 ? option_num - 1 : option_choose - 1;
                        debug_print("Option choose: %d\n", option_choose);
                    }
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    debug_print("DOWN key press.\n");
                    if (stat == STATUS_DIALOGUE_OPTION)
                    {
                        option_choose = option_choose == option_num - 1 ? 0 : option_choose + 1;
                        debug_print("Option choose: %d\n", option_choose);
                    }
                }
            }
        }

        // create renderer
        SDL_Renderer *renderer = NULL;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
        {
            debug_print("can't create renderer.\n");
            return -1;
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // draw
        if (stat == STATUS_DIALOGUE && wait_key == 0)
        {
            draw_conversation(renderer, font, "./res/img/bg.jpg", "./res/img/avatar.png", "./res/img/avatar.png", "王", "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
            wait_key = 1;
        }
        // draw_conversation(renderer, font, "./res/img/bg.jpg", "./res/img/avatar.png", "./res/img/avatar.png", "王一二三四五六", "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");

        // char test[5][1024] = {"1", "2", "3", "4", "5"};
        if (stat == STATUS_SCENE)
        {
            draw_background(renderer, "./res/img/bg.jpg");
            draw_title(renderer, title_font, scene_name);
        }
        if (stat == STATUS_DIALOGUE || stat == STATUS_DIALOGUE_OPTION)
        {
            draw_conversation(renderer, font, "./res/img/bg.jpg", "./res/img/avatar.png", "./res/img/avatar.png", character_name, dialogue_text);
        }
        if (stat == STATUS_DIALOGUE_OPTION)
        {
            for (int32_t i = 0; i < option_num; i++)
            {
                toml_table_t *options_txt = toml_table_at(options, i);
                if (options_txt == NULL)
                {
                    debug_print("No option.\n");
                    return -1;
                }
                tmp_datum = toml_string_in(options_txt, "text");
                if (tmp_datum.ok)
                {
                    strncpy(option_text[i], tmp_datum.u.s, sizeof(option_text[i]));
                    free(tmp_datum.u.s);
                    tmp_datum.ok = 0;
                    tmp_datum.u.s = NULL;
                }
                else
                {
                    debug_print("No option.\n");
                    return -1;
                }
            }
            draw_options(renderer, font, option_text, option_num, option_choose);
        }

        // draw_options(renderer, font, test, 5, 0);
        // draw_background(renderer, "./res/img/bg.jpg");
        // draw_title(renderer, title_font, "title");
        // present
        SDL_RenderPresent(renderer);
        SDL_DestroyRenderer(renderer);
    }
    return 0;
}