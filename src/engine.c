#include "basic_include.h"
#include "./TOML_process/TOML_get.h"
#include "./SDL_process/window_process.h"
#include "./SDL_process/init_all_SDL.h"
#include "./cJSON_process/save_process.h"
#include "debug.h"

SDL_Window *window;
TTF_Font *font;
TTF_Font *title_font;
toml_table_t *novel;
cJSON *save;

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
void close_CJSON()
{
    cJSON_Delete(save);
}

enum window_mode
{
    MODE_START = 0,
    MODE_NOVEL,
    MODE_HELP,
    MODE_FAVORABILITY,
    MODE_BAG,
    MODE_SETTING
};

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
    char items_text[MAX_ITEM_NUM][1024] = {0};
    char items_img_path[MAX_ITEM_NUM][1024] = {0};

    // init all id
    char event_id[1024] = {0};
    char scene_id[1024] = {0};
    char character_id[1024] = {0};
    char dialogue_id[1024] = {0};
    char end_id[1024] = {0};
    char items_id[MAX_ITEM_NUM][1024] = {0};
    char tmp_item_id[1024] = {0};
    // using path
    char use_novel_path[1024] = "./res/novel.toml";                                    // {0};
    char use_background_path[1024] = "./res/img/bg.jpg";                               // {0};
    char use_avatar_path[1024] = "./res/img/avatar.png";                               // {0};
    char use_tachie_path[1024] = "./res/img/avatar.png";                               // {0};
    char use_ttf_path[1024] = "./res/fonts/font.ttf";                                  // {0};// NotoSansTC-Medium.ttf
    char use_save_path[1024] = "./res/save.json";                                      // {0};
    char use_item_path[MAX_ITEM_NUM][1024] = {"./res/img/avatar.png", "./res/img/avatar.png"}; // {0};

    // selection
    toml_array_t *options = NULL;
    char option_text[5][1024] = {0};
    int32_t option_num = 0;
    int32_t option_choose = 0;
    enum setting_bar_option setting_bar_select = 0;
    int32_t item_num = 0;
    int32_t item_select = 0;
    // temp data
    toml_datum_t tmp_datum;
    // set status
    enum status stat = 0;
    enum status next_stat = 0;
    enum window_mode mode = MODE_START;

    FILE *novel_file = fopen(use_novel_path, "r");
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
    FILE *save_file = fopen(use_save_path, "r");
    char save_buffer[16384] = {0};
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
        save = cJSON_CreateObject();
        toml_array_t *item = toml_array_in(start, "inventory");
        if (item != NULL)
        {
            for (int32_t i = 0; i < toml_array_nelem(item); i++)
            {
                toml_datum_t item_txt = toml_string_at(item, i);
                if (item_txt.ok)
                {
                    update_add_item(save, item_txt.u.s);
                    free(item_txt.u.s);
                    item_txt.ok = 0;
                    item_txt.u.s = NULL;
                }
            }
        }
        update_event(save, event_id);
    }
    else
    {
        fread(save_buffer, 1, 16384, save_file);
        save = cJSON_Parse(save_buffer);
        if (save == NULL)
        {
            debug_print("Error parsing save file.\n");
            return -1;
        }
        fclose(save_file);
        if (cJSON_GetObjectItem(save, "event") != NULL)
        {
            strcpy(event_id, cJSON_GetObjectItem(save, "event")->valuestring);
            next_stat = STATUS_EVENT;
        }
    }
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
    tmp_datum = toml_string_in(novel, "start_background");
    if (tmp_datum.ok)
    {
        strncpy(background_path, tmp_datum.u.s, sizeof(background_path));
        free(tmp_datum.u.s);
        tmp_datum.ok = 0;
        tmp_datum.u.s = NULL;
    }
    else
    {
        strncpy(background_path, "NO_BG", sizeof(background_path));
        debug_print("No background.\n");
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        debug_print("Can't create window.\n");
        return -1;
    }
    atexit(close_SDL);
    font = TTF_OpenFont(use_ttf_path, 24);
    if (font == NULL)
    {
        debug_print("can't open font.\n");
        return -1;
    }
    title_font = TTF_OpenFont(use_ttf_path, 48);
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
                    if (mode == MODE_START)
                    {
                        mode = MODE_NOVEL;
                        int8_t change = change_status(novel, &stat, &next_stat, background_path, avatar_path, tachie_path, scene_name, character_name, dialogue_text, end_text, event_id, scene_id, character_id, dialogue_id,tmp_item_id ,end_id, &options, option_choose);
                        if (change == -1)
                        {
                            debug_print("Error change status %d\n", stat);
                            debug_print("Quit.\n");
                            return -1;
                        }
                        else if (change == 1)
                        {
                            debug_print("End.\n");
                            // return 0;
                        }
                    }
                    if (mode == MODE_NOVEL)
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
                        int8_t change = change_status(novel, &stat, &next_stat, background_path, avatar_path, tachie_path, scene_name, character_name, dialogue_text, end_text, event_id, scene_id, character_id, dialogue_id,tmp_item_id, end_id, &options, option_choose);
                        if (change == -1)
                        {
                            debug_print("Error change status %d\n", stat);
                            debug_print("Quit.\n");
                            return -1;
                        }
                        else if (change == 1)
                        {
                            debug_print("End.\n");
                            // return 0;
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
                        if(stat == STATUS_EVENT && strlen(tmp_item_id)>0)
                        {
                            update_add_item(save, tmp_item_id);
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
                    }
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (mode == MODE_NOVEL)
                    {

                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            option_choose = ((option_choose - 1)+option_num)%option_num;
                            debug_print("Option choose: %d\n", option_choose);
                        }
                    }
                    if (mode == MODE_BAG)
                    {
                        item_select = ((item_select - 1) +MAX_ITEM_NUM)%MAX_ITEM_NUM;
                        debug_print("Item select: %d\n", item_select);
                    }
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (mode == MODE_NOVEL)
                    {
                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            option_choose = (option_choose + 1) % option_num;
                            debug_print("Option choose: %d\n", option_choose);
                        }
                    }
                    if (mode == MODE_BAG)
                    {
                        item_select = (item_select + 1) % MAX_ITEM_NUM;
                        debug_print("Item select: %d\n", item_select);
                    }
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    if (mode == MODE_NOVEL)
                    {
                        setting_bar_select = (setting_bar_select + 1) % SETTING_BAR_OPTION_NUM;
                    }
                    if (mode == MODE_BAG)
                    {
                        item_select = (item_select + MAX_ITEM_NUM / ITEM_COL_NUM) % MAX_ITEM_NUM;
                    }
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    if (mode == MODE_NOVEL)
                    {
                        setting_bar_select = ((setting_bar_select - 1)+ SETTING_BAR_OPTION_NUM) % SETTING_BAR_OPTION_NUM;
                    }
                    if (mode == MODE_BAG)
                    {
                        item_select = ((item_select - MAX_ITEM_NUM / ITEM_COL_NUM) + MAX_ITEM_NUM)%MAX_ITEM_NUM ;
                    }
                }
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    if (mode == MODE_NOVEL)
                    {
                        if (setting_bar_select == SETTING_BAR_OPTION_SAVE)
                        {
                            save_file = fopen(use_save_path, "w");
                            if (save_file == NULL)
                            {
                                debug_print("No save file.\n");
                                return -1;
                            }
                            char *save_str = cJSON_Print(save);
                            fwrite(save_str, 1, strlen(save_str), save_file);
                            fclose(save_file);
                            free(save_str);
                        }

                        if (setting_bar_select == SETTING_BAR_OPTION_EXIT)
                        {
                            debug_print("Quit.\n");
                            return 0;
                        }
                        if (setting_bar_select == SETTING_BAR_OPTION_HELP)
                        {
                            mode = MODE_HELP;
                        }
                        if (setting_bar_select == SETTING_BAR_OPTION_CHARACTERS)
                        {
                            mode = MODE_FAVORABILITY;
                        }
                        if (setting_bar_select == SETTING_BAR_OPTION_BAG)
                        {
                            mode = MODE_BAG;
                            item_select = 0;
                            if(update_get_item_num(save, &item_num) == -1)
                            {
                                debug_print("save error.\n");
                                return -1;
                            }
                            toml_table_t *items = toml_table_in(novel, "item");
                            for(int32_t i = 0; i < item_num; i++)
                            {
                                update_get_item_id(save, i, items_id[i]);
                                get_items(items, items_id[i], items_text[i], items_img_path[i]);
                            }

                        }
                    }
                }
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    if(mode == MODE_HELP || mode == MODE_FAVORABILITY || mode == MODE_BAG || mode == MODE_SETTING)
                    {
                        mode = MODE_NOVEL;
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
        // draw_conversation(renderer, font, "./res/img/bg.jpg", "./res/img/avatar.png", "./res/img/avatar.png", "王一二三四五六", "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");

        // char test[5][1024] = {"1", "2", "3", "4", "5"};
        if (mode == MODE_START)
        {
            if (strncmp(background_path, "NO_BG", sizeof(background_path)) == 0)
            {
                draw_start_menu(renderer, title_font, font, NULL, title);
            }
            else
            {
                draw_start_menu(renderer, title_font, font, use_background_path, title);
            }
        }
        if (mode == MODE_NOVEL)
        {
            if (stat == STATUS_EVENT)
            {
                update_event(save, event_id);
                draw_title(renderer, title_font, event_id, TITLE_BOTTOM);
                //draw 
            }
            if (stat == STATUS_SCENE)
            {
                draw_background(renderer, use_background_path);
                draw_title(renderer, title_font, scene_name, TITLE_CENTER);
            }
            if (stat == STATUS_DIALOGUE || stat == STATUS_DIALOGUE_OPTION)
            {
                draw_conversation(renderer, font, use_background_path, use_avatar_path, use_tachie_path, character_name, dialogue_text);
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
            if (stat == STATUS_END)
            {
                draw_ending(renderer, title_font, font, use_background_path, scene_name, end_text);
            }
            draw_setting_bar(renderer, font, setting_bar_select);
        }
        if (mode == MODE_BAG)
        {
            draw_bag(renderer, font, items_text, use_item_path, item_num, item_select);
        }
        // present
        SDL_RenderPresent(renderer);
        SDL_DestroyRenderer(renderer);
    }
    return 0;
}