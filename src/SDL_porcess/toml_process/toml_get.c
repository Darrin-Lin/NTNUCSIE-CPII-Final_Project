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
}
