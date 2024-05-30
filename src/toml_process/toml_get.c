#include "toml_get.h"

int8_t change_status(toml_table_t *novel, enum status *stat, enum status *next_stat, char background_path[1024], char avatar_path[1024], char tachie_path[1024], char scene_name[1024], char character_name[1024], char dialogue_text[1024], char end_text[4096], char event_id[1024], char scene_id[1024], char character_id[1024], char dialogue_id[1024], char end_id[1024], toml_array_t **options, int32_t option_choose)
{
    if (stat == NULL || next_stat == NULL || background_path == NULL || avatar_path == NULL || tachie_path == NULL || scene_name == NULL || character_name == NULL || dialogue_text == NULL || end_text == NULL || event_id == NULL || scene_id == NULL || character_id == NULL || dialogue_id == NULL || end_id == NULL || options == NULL)
    {
        return -1;
    }
    toml_datum_t tmp_datum;
    toml_datum_t tmp_datum1;
    toml_datum_t tmp_datum2;
    switch (*next_stat)
    {
    case STATUS_ERROR:
        debug_print("Error status.\n");
        return -1;
        break;
    case STATUS_START:
        *stat = STATUS_START;
        *next_stat = STATUS_EVENT;
        break;
    case STATUS_EVENT:
        toml_table_t *events = toml_table_in(novel, "event");
        if (!events)
        {
            debug_print("No event.\n");
            return -1;
        }
        tmp_datum = toml_string_in(toml_table_in(events, event_id), "scene");
        if (tmp_datum.ok)
        {
            strncpy(scene_id, tmp_datum.u.s, 1024);
            *stat = STATUS_EVENT;
            *next_stat = STATUS_SCENE;
            free(tmp_datum.u.s);
            tmp_datum.u.s = NULL;
            tmp_datum.ok = 0;

            tmp_datum = toml_string_in(toml_table_in(events, event_id), "dialogue");
            if (tmp_datum.ok)
            {
                strncpy(dialogue_id, tmp_datum.u.s, 1024);
                free(tmp_datum.u.s);
                tmp_datum.u.s = NULL;
                tmp_datum.ok = 0;
            }
            else
            {
                debug_print("No dialogue.\n");
                // toml_free(events);
                return -1;
            }
        }
        else
        {
            tmp_datum = toml_string_in(toml_table_in(events, event_id), "end");
            if (tmp_datum.ok)
            {
                *stat = STATUS_EVENT;
                *next_stat = STATUS_END;
                strncpy(end_id, tmp_datum.u.s, 1024);
                free(tmp_datum.u.s);
                tmp_datum.u.s = NULL;
                tmp_datum.ok = 0;
            }
            else
            {

                debug_print("No scene or end.\n");
                // toml_free(events);
                return -1;
            }
        }

        break;
    case STATUS_SCENE:
        toml_table_t *scenes = toml_table_in(novel, "scene");
        if (!scenes)
        {
            debug_print("No scene.\n");
            return -1;
        }
        get_scene(scenes, scene_id, &tmp_datum, &tmp_datum1);
        if (tmp_datum.ok && tmp_datum1.ok)
        {
            strncpy(scene_name, tmp_datum.u.s, 1024);
            strncpy(background_path, tmp_datum1.u.s, 1024);
            *stat = STATUS_SCENE;
            *next_stat = STATUS_DIALOGUE;
            free(tmp_datum.u.s);
            free(tmp_datum1.u.s);
            tmp_datum1.u.s = NULL;
            tmp_datum.u.s = NULL;
            tmp_datum1.ok = 0;
            tmp_datum.ok = 0;
        }
        else
        {
            if (!tmp_datum.ok)
            {
                debug_print("No scene name.\n");
            }
            else
            {
                free(tmp_datum.u.s);
                tmp_datum.u.s = NULL;
                tmp_datum.ok = 0;
            }
            if (!tmp_datum1.ok)
            {
                debug_print("No background path.\n");
            }
            else
            {
                free(tmp_datum1.u.s);
                tmp_datum1.u.s = NULL;
                tmp_datum1.ok = 0;
            }
            debug_print("No scene.\n");
            // toml_free(scenes);
            return -1;
        }
        // toml_free(scenes);
        break;
    case STATUS_DIALOGUE:
        toml_table_t *dialogues = toml_table_in(novel, "dialogue");
        if (!dialogues)
        {
            debug_print("No dialogue.\n");
            return -1;
        }
        get_dialogue(dialogues, dialogue_id, &tmp_datum, &tmp_datum1, NULL);
        if (tmp_datum.ok && tmp_datum1.ok)
        {
            strncpy(character_id, tmp_datum.u.s, 1024);
            strncpy(dialogue_text, tmp_datum1.u.s, 1024);
            free(tmp_datum.u.s);
            free(tmp_datum1.u.s);
            tmp_datum1.u.s = NULL;
            tmp_datum.u.s = NULL;
            tmp_datum1.ok = 0;
            tmp_datum.ok = 0;
            toml_table_t *characters = toml_table_in(novel, "character");
            if (!characters)
            {
                debug_print("No character.\n");
                // toml_free(dialogues);
                return -1;
            }
            get_character(characters, character_id, &tmp_datum, &tmp_datum1, &tmp_datum2);
            if (tmp_datum.ok && tmp_datum1.ok && tmp_datum2.ok)
            {
                strncpy(character_name, tmp_datum.u.s, 1024);
                strncpy(avatar_path, tmp_datum1.u.s, 1024);
                strncpy(tachie_path, tmp_datum2.u.s, 1024);
                free(tmp_datum.u.s);
                free(tmp_datum1.u.s);
                free(tmp_datum2.u.s);
                tmp_datum1.u.s = NULL;
                tmp_datum.u.s = NULL;
                tmp_datum2.u.s = NULL;
                tmp_datum1.ok = 0;
                tmp_datum.ok = 0;
                tmp_datum2.ok = 0;
            }
            else
            {
                if (!tmp_datum.ok)
                {
                    debug_print("No character name.\n");
                }
                else
                {
                    free(tmp_datum.u.s);
                    tmp_datum.u.s = NULL;
                    tmp_datum.ok = 0;
                }
                if (!tmp_datum1.ok)
                {
                    debug_print("No avatar path.\n");
                }
                else
                {
                    free(tmp_datum1.u.s);
                    tmp_datum1.u.s = NULL;
                    tmp_datum1.ok = 0;
                }
                if (!tmp_datum2.ok)
                {
                    debug_print("No tachie path.\n");
                }
                else
                {
                    free(tmp_datum2.u.s);
                    tmp_datum2.u.s = NULL;
                    tmp_datum2.ok = 0;
                }
                debug_print("No character.\n");
                // toml_free(characters);
                return -1;
            }
            *stat = STATUS_DIALOGUE;
            *next_stat = STATUS_DIALOGUE_OPTION;
        }
        else
        {
            if (!tmp_datum.ok)
            {
                debug_print("No character id.\n");
            }
            else
            {
                free(tmp_datum.u.s);
                tmp_datum.u.s = NULL;
                tmp_datum.ok = 0;
            }
            if (!tmp_datum1.ok)
            {
                debug_print("No dialogue text.\n");
            }
            else
            {
                free(tmp_datum1.u.s);
            }
            debug_print("No dialogue.\n");
            // toml_free(dialogues);
            return -1;
        }
        // toml_free(dialogues);
        break;
    case STATUS_DIALOGUE_OPTION:
        toml_table_t *dialogues_opt = toml_table_in(novel, "dialogue");
        if (!dialogues_opt)
        {
            debug_print("No dialogue.\n");
            return -1;
        }
        get_dialogue(dialogues_opt, dialogue_id, NULL, NULL, options);
        if (*options == NULL)
        {
            debug_print("option error.\n");
            // toml_free(dialogues_opt);
            return -1;
        }
        *stat = STATUS_DIALOGUE_OPTION;
        toml_table_t *option_table = toml_table_at(*options, 0);
        if (option_table == NULL)
        {
            debug_print("option idx error\n");
            // toml_free(dialogues_opt);
            return -1;
        }
        tmp_datum = toml_string_in(option_table, "next");
        if (tmp_datum.ok)
        {
            *next_stat = STATUS_DIALOGUE;
            free(tmp_datum.u.s);
            tmp_datum.u.s = NULL;
            tmp_datum.ok = 0;
        }
        else
        {
            tmp_datum = toml_string_in(option_table, "event");
            if (tmp_datum.ok)
            {
                *next_stat = STATUS_EVENT;
                free(tmp_datum.u.s);
                tmp_datum.u.s = NULL;
                tmp_datum.ok = 0;
            }
            else
            {
                debug_print("no next status\n");
                // toml_free(dialogues_opt);
                // toml_free(option_table);
                return -1;
            }
        }
        // toml_free(dialogues_opt);
        // toml_free(option_table);
        break;
    case STATU_SETTING:
        // TODO: add setting
        break;
    case STATUS_END:
        debug_print("End of the novel.\n");
        *stat = STATUS_END;
        *next_stat = STATUS_END;
        toml_table_t *ends = toml_table_in(novel, "end");
        if (!ends)
        {
            debug_print("No end.\n");
            return -1;
        }
        if (get_ending(ends, end_id, scene_name, end_text, background_path) == -1)
        {
            return -1;
        }
        return 1;
        break;
    default:
        debug_print("Error status.\n");
        return -1;
        break;
    }
    return 0;
}

int8_t get_scene(toml_table_t *scenes, const char *scene_id, toml_datum_t *scene_title, toml_datum_t *bg_path)
{
    toml_table_t *scene = toml_table_in(scenes, scene_id);
    if (!scene)
    {
        debug_print("scene is NULL");
        return -1;
    }
    *scene_title = toml_string_in(scene, "name");
    *bg_path = toml_string_in(scene, "background");
    return 0;
}
int8_t get_character(toml_table_t *characters, const char *character_id, toml_datum_t *character_name, toml_datum_t *character_avatar_path, toml_datum_t *character_tachie_path)
{
    toml_table_t *character = toml_table_in(characters, character_id);
    if (!character)
    {
        debug_print("character is NULL");
        return -1;
    }
    *character_name = toml_string_in(character, "name");
    if (character_name->ok == 0)
    {
        return -1;
    }
    *character_avatar_path = toml_string_in(character, "avatar");
    if (character_avatar_path->ok == 0)
    {
        free(character_name->u.s);
        character_name->u.s = NULL;
        character_name->ok = 0;
        return -1;
    }
    if (character_tachie_path != NULL)
    {
        *character_tachie_path = toml_string_in(character, "tachie");
        if (character_tachie_path->ok == 0)
        {
            free(character_name->u.s);
            character_name->u.s = NULL;
            character_name->ok = 0;
            free(character_avatar_path->u.s);
            character_avatar_path->u.s = NULL;
            character_avatar_path->ok = 0;
            return -1;
        }
    }
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
    *event_scene = toml_string_in(event, "scene");
    *event_dialogue = toml_string_in(event, "dialogue");
    return 0;
}
int8_t get_dialogue(toml_table_t *dialogues, const char *dialogue_id, toml_datum_t *dialogue_character, toml_datum_t *dialogue_text, toml_array_t **options)
{
    toml_table_t *dialogue = toml_table_in(dialogues, dialogue_id);
    if (!dialogue)
    {
        debug_print("dialogue is NULL");
        return -1;
    }
    if (dialogue_character != NULL)
    {

        *dialogue_character = toml_string_in(dialogue, "character");
        if (dialogue_character->ok == 0)
        {
            return -1;
        }
    }
    if (dialogue_text != NULL)
    {

        *dialogue_text = toml_string_in(dialogue, "text");
        if (dialogue_text->ok == 0)
        {
            return -1;
        }
    }
    if (options == NULL)
    {
        return 0;
    }
    *options = toml_array_in(dialogue, "options");
    return 0;
}

int8_t get_ending(toml_table_t *ends, const char *end_id, char end_title[1024], char end_text[4096], char end_bg_path[1024])
{
    toml_table_t *ending = toml_table_in(ends, end_id);
    if (!ending)
    {
        debug_print("ending is NULL");
        return -1;
    }
    toml_datum_t title = toml_string_in(ending, "title");
    toml_datum_t txt = toml_string_in(ending, "text");
    toml_datum_t end_bg = toml_string_in(ending, "background");
    if (title.ok && txt.ok && end_bg.ok)
    {
        strncpy(end_title, title.u.s, 1024);
        strncpy(end_text, txt.u.s, 1024);
        strncpy(end_bg_path, end_bg.u.s, 1024);
        free(title.u.s);
        free(txt.u.s);
        free(end_bg.u.s);
        title.u.s = NULL;
        txt.u.s = NULL;
        end_bg.u.s = NULL;
        title.ok = 0;
        txt.ok = 0;
        end_bg.ok = 0;
    }
    else
    {
        if (title.ok == 0)
        {
            debug_print("No title.\n");
        }
        else
        {
            free(title.u.s);
            title.u.s = NULL;
            title.ok = 0;
        }
        if (txt.ok == 0)
        {
            debug_print("No text.\n");
        }
        else
        {
            free(txt.u.s);
            txt.u.s = NULL;
            txt.ok = 0;
        }
        if (end_bg.ok == 0)
        {
            debug_print("No bg.\n");
        }
        else
        {
            free(end_bg.u.s);
            end_bg.u.s = NULL;
            end_bg.ok = 0;
        }
        return -1;
    }
    return 0;
}