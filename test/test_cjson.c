#include"../src/third_party/cJSON/cJSON.h"
#include<stdlib.h>
#include<stdio.h>

int main()
{
    FILE *fp = fopen("test.json", "r");
    char buffer[16384]={0};
    if (fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    fread(buffer, 1, 16384, fp);
    printf("%s\n", buffer);
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        printf("Error parsing JSON\n");
        return 1;
    }
    // cJSON_DeleteItemFromObject(json, "event");
    cJSON *event = cJSON_GetObjectItem(json, "item");
    cJSON *item = cJSON_GetArrayItem(event, 0);
    cJSON_SetValuestring(item, "new item");
    // cJSON_SetValuestring(event, "new event");
    // cJSON_AddStringToObject(json, "event", "new event");
    char *tmp = cJSON_PrintUnformatted(json);
    printf("%s\n", tmp);
    free(tmp);
    cJSON_Delete(json);
    fclose(fp);
}