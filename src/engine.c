#include "basic_include.h"
#include "./TOML_process/TOML_get.h"
#include "./SDL_process/window_process.h"
#include "./SDL_process/init_all_SDL.h"
#include "./cJSON_process/save_process.h"
#include "debug.h"

SDL_Window *window;
TTF_Font *font;
TTF_Font *title_font;
Mix_Music *title_music;
Mix_Music *bgm_music;
Mix_Music *ending_music;
Mix_Chunk *sound_switch;
Mix_Chunk *sound_select;
Mix_Chunk *sound_change;
Mix_Chunk *sound_get_item;
Mix_Chunk *sound_dialogue;
Mix_Chunk *sound_scene;
Mix_Chunk *sound_esc;
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
void close_Mix()
{
    Mix_FreeChunk(sound_switch);
    Mix_FreeChunk(sound_select);
    Mix_FreeChunk(sound_change);
    Mix_FreeChunk(sound_get_item);
    Mix_FreeChunk(sound_dialogue);
    Mix_FreeChunk(sound_scene);
    Mix_FreeChunk(sound_esc);
    Mix_CloseAudio();
    Mix_Quit();
}
void close_title_music()
{
    Mix_FreeMusic(title_music);
}
void close_bgm_music()
{
    Mix_FreeMusic(bgm_music);
}
void colse_ending_music()
{
    Mix_FreeMusic(ending_music);
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
    char path[512] = {0};
    if (argc > 1)
    {
        strncpy(path, argv[1], sizeof(path));
        debug_print("Path: %s\n", path);
    }
    if (path[strlen(path) - 1] != '/')
    {
        strncat(path, "/", 512);
    }
    // check if path exist
    if (access(path, F_OK) == -1)
    {
        printf("Path not exist.\n");
        return -1;
    }
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
    char ending_music_path[1024] = {0};
    // init all id
    char event_id[1024] = {0};
    char scene_id[1024] = {0};
    char character_id[1024] = {0};
    char dialogue_id[1024] = {0};
    char end_id[1024] = {0};
    char items_id[MAX_ITEM_NUM][1024] = {0};
    char tmp_item_id[1024] = {0};
    char character_ids[MAX_CHARACTER_NUM][1024] = {0};
    char character_names[MAX_CHARACTER_NUM][1024] = {0};
    char character_img_path[MAX_CHARACTER_NUM][1024] = {0};

    // using path
    char use_novel_path[2048] = {0};
    char use_background_path[2048] = {0};
    char use_avatar_path[2048] = {0};
    char use_tachie_path[2048] = {0};
    char use_ttf_path[2048] = {0}; // NotoSansTC-Medium.ttf
    char use_save_path[2048] = {0};
    char use_item_path[MAX_ITEM_NUM][2048] = {0};
    char use_character_path[MAX_CHARACTER_NUM][2048] = {0};
    char use_music_path[2048] = {0}; // {0};

    // selection
    toml_array_t *options = NULL;
    char option_text[5][1024] = {0};
    int32_t option_num = 0;
    int32_t option_choose = 0;
    enum setting_bar_option setting_bar_select = 0;
    int32_t item_num = 0;
    int32_t item_select = 0;
    int32_t favorability_add = 0;
    int32_t character_num = 0;
    int32_t character_select = 0;
    int32_t favorability[MAX_FACORABILITY] = {0};
    int8_t play_ending_music = 0;
    int8_t animation_play = 0;
    int8_t reload = 1;
    // temp data
    toml_datum_t tmp_datum;
    // set status
    enum status stat = 0;
    enum status next_stat = 0;
    enum window_mode mode = MODE_START;
    snprintf(use_novel_path, 2048, "%s%s", path, "novel.toml");
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
    snprintf(use_save_path, 2048, "%s%s", path, "save.json");
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
        if (cJSON_GetObjectItem(save, "item") != NULL)
        {
            cJSON *item = cJSON_GetObjectItem(save, "item");
            item_num = cJSON_GetArraySize(item);
            for (int32_t i = 0; i < item_num; i++)
            {
                cJSON *item_id = cJSON_GetArrayItem(item, i);
                strncpy(items_id[i], item_id->valuestring, sizeof(items_id[i]));
                get_items(toml_table_in(novel, "item"), items_id[i], items_text[i], items_img_path[i]);
            }
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
    snprintf(use_ttf_path, 2048, "%s%s", path, "fonts/font.ttf");
    font = TTF_OpenFont(use_ttf_path, 24);
    if (font == NULL)
    {
        debug_print("can't open font.\n");
        return -1;
    }
    title_font = TTF_OpenFont(use_ttf_path, 48);
    atexit(close_TTF);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/switch.mp3");
    sound_switch = Mix_LoadWAV(use_music_path);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/select.mp3");
    sound_select = Mix_LoadWAV(use_music_path);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/change.mp3");
    sound_change = Mix_LoadWAV(use_music_path);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/get_item.mp3");
    sound_get_item = Mix_LoadWAV(use_music_path);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/dialogue.mp3");
    sound_dialogue = Mix_LoadWAV(use_music_path);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/scene.mp3");
    sound_scene = Mix_LoadWAV(use_music_path);
    snprintf(use_music_path, 2048, "%s%s", path, "sound/esc.mp3");
    sound_esc = Mix_LoadWAV(use_music_path);
    if (sound_switch == NULL || sound_select == NULL || sound_change == NULL || sound_get_item == NULL || sound_dialogue == NULL || sound_scene == NULL || sound_esc == NULL)
    {
        debug_print("can't load sound.\n");
        return -1;
    }
    atexit(close_Mix);
    snprintf(use_music_path, 2048, "%s%s", path, "music/title.mp3");
    title_music = Mix_LoadMUS(use_music_path);
    if (title_music == NULL)
    {
        debug_print("can't load title music.\n");
    }
    else
    {
        Mix_PlayMusic(title_music, -1);
        atexit(close_title_music);
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
                return 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    if (mode == MODE_START)
                    {
                        Mix_PauseMusic();
                        snprintf(use_music_path, 2048, "%s%s", path, "music/bgm.mp3");
                        bgm_music = Mix_LoadMUS(use_music_path);
                        if (bgm_music == NULL)
                        {
                            debug_print("can't load bgm music.\n");
                        }
                        else
                        {
                            Mix_PlayMusic(bgm_music, -1);
                            atexit(close_bgm_music);
                        }
                        mode = MODE_NOVEL;
                        int8_t change = change_status(novel, &stat, &next_stat, background_path, avatar_path, tachie_path, ending_music_path, scene_name, character_name, dialogue_text, end_text, event_id, scene_id, character_id, dialogue_id, tmp_item_id, end_id, &options, option_choose);
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

                            toml_table_t *options_txt = toml_table_at(options, option_choose);
                            update_favorability_add(save, character_id, favorability_add);
                            favorability_add = 0;
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
                            option_choose = 0;
                            option_num = 0;
                        }
                        int8_t change = change_status(novel, &stat, &next_stat, background_path, avatar_path, tachie_path, ending_music_path, scene_name, character_name, dialogue_text, end_text, event_id, scene_id, character_id, dialogue_id, tmp_item_id, end_id, &options, option_choose);
                        if (change == -1)
                        {
                            debug_print("Error change status %d\n", stat);
                            debug_print("Quit.\n");
                            return -1;
                        }
                        else if (change == 1)
                        {

                            if (!play_ending_music)
                            {

                                if (strncmp(ending_music_path, "NO_MUSIC", 1024) != 0)
                                {
                                    Mix_PauseMusic();
                                    snprintf(use_music_path, 2048, "%s%s", path, ending_music_path);
                                    ending_music = Mix_LoadMUS(use_music_path);
                                    if (ending_music == NULL)
                                    {
                                        debug_print("can't load ending music.\n");
                                    }
                                    else
                                    {
                                        play_ending_music = 1;
                                        Mix_PlayMusic(ending_music, -1);
                                        atexit(colse_ending_music);
                                    }
                                }
                            }
                            debug_print("End.\n");
                            // return 0;
                        }

                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            Mix_PlayChannel(-1, sound_dialogue, 0);
                            option_num = toml_array_nelem(options);
                            if (option_num > 5)
                            {
                                debug_print("Quit.\n");
                                return -1;
                            }
                        }
                        if (stat == STATUS_DIALOGUE)
                        {
                            Mix_PlayChannel(-1, sound_dialogue, 0);
                        }
                        if (stat == STATUS_EVENT)
                        {
                            animation_play = 1;
                            reload = 1;
                            update_event(save, event_id);
                            if (strlen(tmp_item_id) > 0)
                            {
                                Mix_PlayChannel(-1, sound_get_item, 0);
                                update_add_item(save, tmp_item_id);
                            }
                            else
                            {
                                Mix_PlayChannel(-1, sound_change, 0);
                            }
                        }
                        if (stat == STATUS_SCENE)
                        {
                            Mix_PlayChannel(-1, sound_scene, 0);
                        }
                        if (stat == STATUS_DIALOGUE || stat == STATUS_DIALOGUE_OPTION)
                        {
                            int32_t favorability_tmp = 0;
                            if (update_favorability_get(save, character_id, &favorability_tmp) == 1)
                            {
                                strncpy(character_ids[character_num], character_id, 1024);
                                strncpy(character_img_path[character_num], avatar_path, 1024);
                                character_num++;
                            }
                            else
                            {
                                int8_t new_name = 1;
                                for (int32_t i = 0; i < character_num; i++)
                                {
                                    if (strncmp(character_ids[i], character_id, 1024) == 0)
                                    {
                                        new_name = 0;
                                        break;
                                    }
                                }
                                if (new_name)
                                {
                                    strncpy(character_ids[character_num], character_id, 1024);
                                    strncpy(character_img_path[character_num], avatar_path, 1024);
                                    character_num++;
                                }
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
                    }
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (mode == MODE_NOVEL)
                    {
                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            Mix_PlayChannel(-1, sound_switch, 0);
                            option_choose = ((option_choose - 1) + option_num) % option_num;
                            debug_print("Option choose: %d\n", option_choose);
                        }
                    }
                    if (mode == MODE_BAG)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        item_select = ((item_select - 1) + MAX_ITEM_NUM) % MAX_ITEM_NUM;
                        debug_print("Item select: %d\n", item_select);
                    }
                    if (mode == MODE_FAVORABILITY)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        character_select = ((character_select - 1) + MAX_CHARACTER_NUM) % MAX_CHARACTER_NUM;
                    }
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (mode == MODE_NOVEL)
                    {
                        if (stat == STATUS_DIALOGUE_OPTION)
                        {
                            Mix_PlayChannel(-1, sound_switch, 0);
                            option_choose = (option_choose + 1) % option_num;
                            debug_print("Option choose: %d\n", option_choose);
                        }
                    }
                    if (mode == MODE_BAG)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        item_select = (item_select + 1) % MAX_ITEM_NUM;
                        debug_print("Item select: %d\n", item_select);
                    }
                    if (mode == MODE_FAVORABILITY)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        character_select = (character_select + 1) % MAX_CHARACTER_NUM;
                    }
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    if (mode == MODE_NOVEL)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        setting_bar_select = (setting_bar_select + 1) % SETTING_BAR_OPTION_NUM;
                    }
                    if (mode == MODE_BAG)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        item_select = (item_select + (MAX_ITEM_NUM / ITEM_COL_NUM)) % MAX_ITEM_NUM;
                    }
                    if (mode == MODE_FAVORABILITY)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        character_select = (character_select + (MAX_CHARACTER_NUM / CHARACTERS_COL_NUM)) % MAX_CHARACTER_NUM;
                    }
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    if (mode == MODE_NOVEL)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        setting_bar_select = ((setting_bar_select - 1) + SETTING_BAR_OPTION_NUM) % SETTING_BAR_OPTION_NUM;
                    }
                    if (mode == MODE_BAG)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        item_select = ((item_select - (MAX_ITEM_NUM / ITEM_COL_NUM)) + MAX_ITEM_NUM) % MAX_ITEM_NUM;
                    }
                    if (mode == MODE_FAVORABILITY)
                    {
                        Mix_PlayChannel(-1, sound_switch, 0);
                        character_select = (character_select - (MAX_CHARACTER_NUM / CHARACTERS_COL_NUM) + MAX_CHARACTER_NUM) % MAX_CHARACTER_NUM;
                    }
                }
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    if (mode == MODE_NOVEL)
                    {
                        Mix_PlayChannel(-1, sound_select, 0);
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
                            character_select = 0;
                            for (int32_t i = 0; i < character_num; i++)
                            {
                                update_favorability_get(save, character_ids[i], &favorability[i]);
                                toml_table_t *characters = toml_table_in(novel, "character");
                                toml_datum_t tmp_avatar_path;
                                toml_datum_t tmp_tachie_path;
                                get_character(characters, character_ids[i], &tmp_datum, &tmp_avatar_path, &tmp_tachie_path);
                                if (tmp_datum.ok)
                                {
                                    strncpy(character_names[i], tmp_datum.u.s, 1024);
                                    free(tmp_datum.u.s);
                                    tmp_datum.ok = 0;
                                    tmp_datum.u.s = NULL;
                                }
                                if (tmp_avatar_path.ok)
                                {
                                    strncpy(character_img_path[i], tmp_avatar_path.u.s, 1024);
                                    free(tmp_avatar_path.u.s);
                                    tmp_avatar_path.ok = 0;
                                    tmp_avatar_path.u.s = NULL;
                                }
                                if (tmp_tachie_path.ok)
                                {
                                    free(tmp_tachie_path.u.s);
                                    tmp_tachie_path.ok = 0;
                                    tmp_tachie_path.u.s = NULL;
                                }
                            }
                        }
                        if (setting_bar_select == SETTING_BAR_OPTION_BAG)
                        {
                            mode = MODE_BAG;
                            item_select = 0;
                            if (update_get_item_num(save, &item_num) == -1)
                            {
                                debug_print("save error.\n");
                                return -1;
                            }
                            toml_table_t *items = toml_table_in(novel, "item");
                            for (int32_t i = 0; i < item_num; i++)
                            {
                                update_get_item_id(save, i, items_id[i]);
                                debug_print("Item id: %s\n", items_id[i]);
                                get_items(items, items_id[i], items_text[i], items_img_path[i]);
                            }
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    Mix_PlayChannel(-1, sound_esc, 0);
                    if (mode == MODE_HELP || mode == MODE_FAVORABILITY || mode == MODE_BAG || mode == MODE_SETTING)
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
                snprintf(use_background_path, 2048, "%s%s", path, background_path);
                draw_start_menu(renderer, title_font, font, use_background_path, title);
            }
        }
        if (mode == MODE_NOVEL)
        {
            snprintf(use_background_path,2048, "%s%s", path, background_path);
            snprintf(use_avatar_path, 2048, "%s%s", path, avatar_path);
            snprintf(use_tachie_path, 2048, "%s%s", path, tachie_path);
            if (stat == STATUS_EVENT)
            {
                if (animation_play && draw_animation(renderer, use_background_path,reload) == 1) // show animation
                {
                    animation_play = 0;
                    debug_print("Animation end.\n");
                }
                reload = 0;
                if (strlen(tmp_item_id))
                {
                    char tmp_item_name[1024] = {0};
                    char tmp_item_img_path[1024] = {0};
                    char use_tmp_item_img_path[2048] = {0};
                    toml_table_t *items = toml_table_in(novel, "item");
                    get_items(items, tmp_item_id, tmp_item_name, tmp_item_img_path);
                    strtok(tmp_item_name, ":");
                    snprintf(use_tmp_item_img_path, 2048, "%s%s", path, tmp_item_img_path);
                    draw_item_get(renderer, title_font, tmp_item_name, use_tmp_item_img_path);
                }
                draw_title(renderer, title_font, "press [space] to continue", TITLE_BOTTOM);
                // draw
            }
            if (stat == STATUS_SCENE)
            {
                draw_background(renderer, use_background_path);
                draw_title(renderer, title_font, scene_name, TITLE_CENTER);
            }
            if (stat == STATUS_DIALOGUE)
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
                    if (i == option_choose)
                    {
                        tmp_datum = toml_int_in(options_txt, "favorability_add");
                        if (tmp_datum.ok)
                        {
                            favorability_add = tmp_datum.u.i;
                            tmp_datum.ok = 0;
                            tmp_datum.u.i = 0;
                        }
                        else
                        {
                            favorability_add = 0;
                        }
                    }
                }
                if (favorability_add)
                {
                    toml_table_t *characters = toml_table_in(novel, "character");
                    get_character_mood(characters, character_id, avatar_path, tachie_path, favorability_add);
                    snprintf(use_avatar_path, 2048, "%s%s", path, avatar_path);
                    snprintf(use_tachie_path, 2048, "%s%s", path, tachie_path);
                }

                draw_conversation(renderer, font, use_background_path, use_avatar_path, use_tachie_path, character_name, dialogue_text);
                draw_options(renderer, font, option_text, option_num, option_choose);
            }
            if (stat == STATUS_END)
            {
                draw_ending(renderer, title_font, font, use_background_path, scene_name, end_text);
            }
            draw_setting_bar(renderer, font, setting_bar_select);
        }
        if (mode == MODE_HELP)
        {
            draw_help(renderer, title_font);
        }
        if (mode == MODE_BAG)
        {
            for (int32_t i = 0; i < item_num; i++)
            {
                snprintf(use_item_path[i], 2048, "%s%s", path, items_img_path[i]);
            }
            draw_bag(renderer, font, items_text, use_item_path, item_num, item_select);
        }
        if (mode == MODE_FAVORABILITY)
        {
            for (int32_t i = 0; i < character_num; i++)
            {
                snprintf(use_character_path[i], 2048, "%s%s", path, character_img_path[i]);
            }
            draw_favorability(renderer, font, character_names, use_character_path, favorability, character_num, character_select);
        }
        // present
        SDL_RenderPresent(renderer);
        SDL_DestroyRenderer(renderer);
    }
    return 0;
}