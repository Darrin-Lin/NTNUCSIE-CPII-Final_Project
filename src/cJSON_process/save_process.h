#include "../third_party/cJSON/cJSON.h"
#include "../basic_include.h"
#include "../debug.h"

#pragma once

#define MAX_ITEM_NUM 10
#define MAX_FACORABILITY 100
#define MIN_FACORABILITY 0
#define MAX_CHARACTER_NUM 30

int8_t update_save_file(char *save_path, cJSON *save);
int8_t update_event(cJSON *save, char *event_id);
int8_t update_add_item(cJSON *save, char *item_id);
int8_t update_remove_item(cJSON *save, char *item_id);
int8_t update_check_item(cJSON *save, char *item_id);
int8_t update_get_item_num(cJSON *save, int32_t *item_num);
int8_t update_get_item_id(cJSON *save, int32_t item_index, char *item_id);
int8_t update_favorability_add(cJSON *save, char *character_id, int32_t favorability_add);
int8_t update_favorability_get(cJSON *save, char *character_id, int32_t *favorability);
int8_t update_favorability_set(cJSON *save, char *character_id, int32_t favorability);

