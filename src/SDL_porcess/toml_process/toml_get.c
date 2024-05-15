#include "../../third_party/tomlc99/toml.h"
#include "../../debug.h"
#include "../../basic_include.h"

int8_t get_scence(toml_table_t *scences, const char *scence_id, toml_datum_t *scence_title, toml_datum_t *bg_path)
{
    toml_table_t *scence = toml_table_in(scences, scence_id);
    if (!scence)
    {
        debug_print("scence is NULL");
        return -1;
    }
    *scence_title = toml_string_in(scence,"name");
    *bg_path = toml_string_in(scence,"background");
    return 0;
}
int8_t get_character(toml_table_t *characters, const char *character_id, toml_datum_t *character_name, toml_datum_t *character_avatar_path,toml_datum_t *character_tachie_path)
{
    toml_table_t *character = toml_table_in(characters, character_id);
    if (!character)
    {
        debug_print("character is NULL");
        return -1;
    }
    *character_name = toml_string_in(character,"name");
    *character_avatar_path = toml_string_in(character,"avatar");
    *character_tachie_path = toml_string_in(character,"tachie");
    return 0;
}
int8_t get_event(toml_table_t *events, const char *event_id, toml_datum_t *event_scene, toml_datum_t *event_dialogue)
{
    toml_table_t *event = toml_table_in(events, event_id);
    if (!event)
    {
        debug_print("event is NULL");
        return -1;
    }
    *event_scene = toml_string_in(event,"scene");
    *event_dialogue = toml_string_in(event,"dialogue");
    return 0;
}
int8_t get_dialogue(toml_table_t *dialogues, const char *dialogue_id, toml_datum_t *dialogue_character, toml_datum_t *dialogue_text, toml_array_t *options)
{
    toml_table_t *dialogue = toml_table_in(dialogues, dialogue_id);
    if (!dialogue)
    {
        debug_print("dialogue is NULL");
        return -1;
    }
    *dialogue_character = toml_string_in(dialogue,"character");
    *dialogue_text = toml_string_in(dialogue,"text");
    *options = toml_array_in(dialogue,"options");
    return 0;
}



