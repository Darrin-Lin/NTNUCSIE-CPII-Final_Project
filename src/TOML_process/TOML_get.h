#include "../third_party/tomlc99/toml.h"
#include "../debug.h"
#include "../basic_include.h"

#pragma once

enum status
{
    STATUS_ERROR = -1,
    STATUS_START = 0,
    STATUS_EVENT,
    STATUS_SCENE,
    STATUS_DIALOGUE,
    STATUS_DIALOGUE_OPTION,
    STATU_SETTING,
    STATUS_END
};

int8_t change_status(toml_table_t *novel, enum status *stat, enum status *next_stat, char background_path[1024], char avatar_path[1024], char tachie_path[1024], char ending_music_path[1024], char scene_name[1024], char character_name[1024], char dialogue_text[1024], char end_text[4096], char event_id[1024], char scene_id[1024], char character_id[1024], char dialogue_id[1024], char item_id[1024], char end_id[1024], toml_array_t **options, int32_t option_choose);
int8_t get_scene(toml_table_t *scenes, const char *scene_name, toml_datum_t *scene_title, toml_datum_t *bg_path);
int8_t get_character(toml_table_t *characters, const char *character_id, toml_datum_t *character_name, toml_datum_t *character_avatar_path, toml_datum_t *character_tachie_path);
int8_t get_character_mood(toml_table_t *characters, const char *character_id, char *avatar_path, char *tachie_path, int32_t favorability_add);
int8_t get_dialogue(toml_table_t *dialogues, const char *dialogue_id, toml_datum_t *dialogue_character, toml_datum_t *dialogue_text, toml_array_t **options);
int8_t get_ending(toml_table_t *ends, const char *end_id, char end_title[1024], char end_text[4096], char end_bg_path[1024], char ending_music_path[1024]);
int8_t get_items(toml_table_t *items, const char *item_id, char *item_name_and_text, char *item_img_path);