#include "../src/basic_include.h"
#include "../src/third_party/toml-c/header/toml-c.h"

int8_t *get_key(int32_t deep, toml_table_t *input_table, toml_table_t **output_table)
{
    if (input_table)
    {
        int keylen;
        const char *key = toml_table_key(input_table, i, &keylen);
        toml_table_t *each = toml_table_table(dialogue, key);
        if (each)
        {
            int le = toml_table_len(each);
            printf("\tkey #%d: %s\n", i, key);
            for (int j = 0; j < le; j++)
            {
                int keyle;
                const char *each_key = toml_table_key(each, j, &keyle);
                printf("\t\tkey #%d: %s\n", j, each_key);
            }
        }
    }
    else
    {
        return -1;
    }
}

int main(int argc, char *argv[])
{
    int32_t fp = open("test.toml", O_RDONLY);
    if (fp == -1)
    {
        return -1;
    }
    struct stat st;
    if (fstat(fp, &st) < 0)
    {
        return -1;
    }
    char *buf = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fp, 0);
    char err[200];

    toml_table_t *conf = toml_parse(buf, err, sizeof(err));
    close(fp);
    if (conf == NULL)
    {
        return -1;
    }
    toml_value_t name = toml_table_string(conf, "name");
    printf("name = \"%s\"\n", name.u.s);
    free(name.u.s); // is ther any function to free the string in toml_value_t?
    int len_of = toml_table_len(conf);
    for (int x = 0; x < len_of; x++)
    {
        int keyl;
        const char *k = toml_table_key(conf, x, &keyl);
        toml_table_t *dialogue = toml_table_table(conf, k);
        printf("key #%d: %s\n", x, k);

        if (dialogue)
        {
            int l = toml_table_len(dialogue);
            for (int i = 0; i < l; i++)
            {
                int keylen;
                const char *key = toml_table_key(dialogue, i, &keylen);
                toml_table_t *each = toml_table_table(dialogue, key);
                if (each)
                {
                    int le = toml_table_len(each);
                    printf("\tkey #%d: %s\n", i, key);
                    for (int j = 0; j < le; j++)
                    {
                        int keyle;
                        const char *each_key = toml_table_key(each, j, &keyle);
                        printf("\t\tkey #%d: %s\n", j, each_key);
                    }
                }
            }
        }
    }
    toml_free(conf);

    // toml_array_t *arr = toml_table_array(conf, "options");
    // int32_t l = toml_array_len(arr);
    // for (int32_t i = 0; i < l; i++)
    // {
    //     toml_table_t *t = toml_array_table(arr, i);
    // 	toml_value_t v = toml_table_string(t, "text");
    // 	printf("dialogue.emotion_implementation.options[%d].text = \"%s\"\n", i, v.u.s);
    // }

    return 0;
}