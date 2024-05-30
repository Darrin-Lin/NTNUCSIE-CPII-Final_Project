#include "../third_party/cJSON/cJSON.h"
#include "../basic_include.h"
#include "../debug.h"

#pragma once

int8_t update_save_file(char *save_path, cJSON *save);
int8_t update_event(cJSON *save, char *event_id);
int8_t update_add_item(cJSON *save, char *item_id);
int8_t update_remove_item(cJSON *save, char *item_id);
int8_t update_check_item(cJSON *save, char *item_id);