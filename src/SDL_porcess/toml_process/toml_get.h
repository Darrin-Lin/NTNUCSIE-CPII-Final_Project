#include "../../third_party/tomlc99/toml.h"
#include "../../debug.h"
#include "../../basic_include.h"

#pragma once

int8_t get_scence(toml_table_t *scences, const char *scence_name, toml_datum_t *scence_title, toml_datum_t *bg_path);
int8_t get_character(toml_table_t *characters, const char *character_id, toml_datum_t *character_name, toml_datum_t *character_avatar_path,toml_datum_t *character_tachie_path);