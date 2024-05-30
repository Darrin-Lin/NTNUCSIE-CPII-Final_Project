#include "./save_process.h"

int8_t update_save_file(char *save_path, cJSON *save)
{
    if(save == NULL)
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
    if(save == NULL || event_id == NULL)
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
    if(save == NULL || item_id == NULL)
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
        cJSON_AddItemToArray(item, cJSON_CreateString(item_id));
    }
    return 0;
}

int8_t update_remove_item(cJSON *save, char *item_id)
{
    if(save == NULL || item_id == NULL)
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
        for(int i = 0; i < cJSON_GetArraySize(items); i++)
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
    if(save == NULL || item_id == NULL)
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
        for(int i = 0; i < cJSON_GetArraySize(items); i++)
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