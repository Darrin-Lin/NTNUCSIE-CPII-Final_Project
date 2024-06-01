#include "./save_process.h"

int8_t update_save_file(char *save_path, cJSON *save)
{
    if (save == NULL)
    {
        debug_print("save is empty\n");
        return -1;
    }
    FILE *fp = fopen(save_path, "w");
    if (fp == NULL)
    {
        debug_print("%s not found\n", save_path);
        return -1;
    }
    fprintf(fp, "%s", cJSON_PrintUnformatted(save));
    fclose(fp);
    return 0;
}
int8_t update_event(cJSON *save, char *event_id)
{
    if (save == NULL || event_id == NULL)
    {
        debug_print("save or event_id is empty\n");
        return -1;
    }
    cJSON *event = cJSON_GetObjectItem(save, "event");
    if (event == NULL)
    {
        debug_print("event not found\n");
        cJSON_AddStringToObject(save, "event", event_id);
    }
    else
    {
        cJSON_SetValuestring(event, event_id);
    }
    return 0;
}

int8_t update_add_item(cJSON *save, char *item_id)
{
    if (save == NULL || item_id == NULL)
    {
        debug_print("save or item_id is empty\n");
        return -1;
    }
    cJSON *item = cJSON_GetObjectItem(save, "item");
    if (item == NULL)
    {
        debug_print("item not found\n");
        cJSON *item_array = cJSON_CreateArray();
        cJSON_AddItemToObject(save, "item", item_array);
        cJSON_AddItemToArray(item_array, cJSON_CreateString(item_id));
    }
    else
    {
        if(cJSON_GetArraySize(item) >= MAX_ITEM_NUM)
        {
            debug_print("Bag is full.\n");
            return -1;
        }
        cJSON_AddItemToArray(item, cJSON_CreateString(item_id));
    }
    return 0;
}

int8_t update_remove_item(cJSON *save, char *item_id)
{
    if (save == NULL || item_id == NULL)
    {
        debug_print("save or item_id is empty\n");
        return -1;
    }
    cJSON *items = cJSON_GetObjectItem(save, "item");
    if (items == NULL)
    {
        debug_print("item is empty\n");
        return -1;
    }
    else
    {
        for (int i = 0; i < cJSON_GetArraySize(items); i++)
        {
            cJSON *item = cJSON_GetArrayItem(items, i);
            if (strcmp(item->valuestring, item_id) == 0)
            {
                cJSON_DeleteItemFromArray(items, i);
                return 0;
            }
        }
    }
    debug_print("item not found\n");
    return -1;
}

int8_t update_check_item(cJSON *save, char *item_id)
{
    if (save == NULL || item_id == NULL)
    {
        debug_print("save or item_id is empty\n");
        return 0;
    }
    cJSON *items = cJSON_GetObjectItem(save, "item");
    if (items == NULL)
    {
        debug_print("item is empty\n");
        return 0;
    }
    else
    {
        for (int i = 0; i < cJSON_GetArraySize(items); i++)
        {
            cJSON *item = cJSON_GetArrayItem(items, i);
            if (strcmp(item->valuestring, item_id) == 0)
            {
                return 1;
            }
        }
    }
    debug_print("item not found\n");
    return 0;
}
int8_t update_get_item_num(cJSON *save, int32_t *item_num)
{
    if (save == NULL)
    {
        debug_print("save is empty\n");
        return -1;
    }
    cJSON *items = cJSON_GetObjectItem(save, "item");
    if (items == NULL)
    {
        debug_print("item is empty\n");
        *item_num = 0;
    }
    else
    {
        *item_num = cJSON_GetArraySize(items);
        if(*item_num > MAX_ITEM_NUM)
        {
            debug_print("Bag is full.\n");
            return 1;
        }
    }
    return 0;
}
int8_t update_get_item_id(cJSON *save, int32_t item_index, char *item_id)
{
    if (save == NULL || item_id == NULL)
    {
        debug_print("save or item_id is empty\n");
        return -1;
    }
    cJSON *items = cJSON_GetObjectItem(save, "item");
    if (items == NULL)
    {
        debug_print("item is empty\n");
        return -1;
    }
    else
    {
        cJSON *item = cJSON_GetArrayItem(items, item_index);
        if (item == NULL)
        {
            debug_print("item not found\n");
            return -1;
        }
        else
        {
            strcpy(item_id, item->valuestring);
            return 0;
        }
    }
    return -1;
}


int8_t update_favorability_add(cJSON *save, char *character_id, int32_t favorability_add)
{
    if (save == NULL || character_id == NULL)
    {
        debug_print("save or character_id is empty\n");
        return -1;
    }
    cJSON *favorability = cJSON_GetObjectItem(save, "favorability");
    if (favorability == NULL)
    {
        debug_print("favorability is empty\n");
        return -1;
    }
    else
    {
        cJSON *character = cJSON_GetObjectItem(favorability, character_id);

        if (character == NULL)
        {
            debug_print("character not found\n");
            cJSON_AddNumberToObject(favorability, character_id, 0 + favorability_add);
        }
        else
        {
            cJSON_SetIntValue(character, cJSON_GetNumberValue(character) + favorability_add);
        }
        return 0;
    }
    debug_print("character not found\n");
    return -1;
}

int8_t update_favorability_get(cJSON *save, char *character_id, int32_t *favorability)
{
    if (save == NULL || character_id == NULL)
    {
        debug_print("save or character_id is empty\n");
        return -1;
    }
    cJSON *favorability_json = cJSON_GetObjectItem(save, "favorability");
    if (favorability_json == NULL)
    {
        debug_print("favorability not empty\n");
        cJSON_AddItemToObject(save, "favorability", cJSON_CreateObject());
        favorability_json = cJSON_GetObjectItem(save, "favorability");
        cJSON_AddNumberToObject(favorability_json, character_id, 0);
        *favorability = 0;
    }
    else
    {
        cJSON *character = cJSON_GetObjectItem(favorability_json, character_id);
        if (character == NULL)
        {
            debug_print("character not found\n");
            cJSON_AddNumberToObject(favorability_json, character_id, 0);
        }
        else if(cJSON_IsNumber(character))
        {
            if(cJSON_GetNumberValue(character) > MAX_FACORABILITY)
            {
                cJSON_SetIntValue(character, MAX_FACORABILITY);
                *favorability = MAX_FACORABILITY;
            }
            else if(cJSON_GetNumberValue(character) < MIN_FACORABILITY)
            {
                cJSON_SetIntValue(character, MIN_FACORABILITY);
                *favorability = MIN_FACORABILITY;
            }
            else
            {
                *favorability = cJSON_GetNumberValue(character);
            }
            *favorability = cJSON_GetNumberValue(character);
        }
        else
        {
            debug_print("character is not number\n");
            return -1;
        }

    }
    return 0;
}
int8_t update_favorability_set(cJSON *save, char *character_id, int32_t favorability)
{
    if (save == NULL || character_id == NULL)
    {
        debug_print("save or character_id is empty\n");
        return -1;
    }
    if(favorability > MAX_FACORABILITY)
    {
        debug_print("favorability is too high\n");
        favorability = MAX_FACORABILITY;
    }
    else if(favorability < MIN_FACORABILITY)
    {
        debug_print("favorability is too low\n");
        favorability = MIN_FACORABILITY;
    }
    cJSON *favorability_json = cJSON_GetObjectItem(save, "favorability");
    if (favorability_json == NULL)
    {
        debug_print("favorability not empty\n");
        cJSON_AddItemToObject(save, "favorability", cJSON_CreateObject());
        favorability_json = cJSON_GetObjectItem(save, "favorability");
        cJSON_AddNumberToObject(favorability_json, character_id, favorability);
    }
    else
    {
        cJSON *character = cJSON_GetObjectItem(favorability_json, character_id);
        if (character == NULL)
        {
            debug_print("character not found\n");
            cJSON_AddNumberToObject(favorability_json, character_id, favorability);
        }
        else
        {
            cJSON_SetIntValue(character, favorability);
        }
    }
    return 0;
}