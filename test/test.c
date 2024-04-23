#include "../src/basic_include.h"
#include "../src/third_party/toml-c/header/toml-c.h"

int8_t get_key(int32_t deep,  toml_table_t *input_table, toml_table_t **output_table)
{
    if (deep < 0)
    {
        *output_table = input_table;
        return 0;
    }
    if (input_table)
    {

        int len = toml_table_len(input_table);

        for (int i = 0; i < len; i++)
        {
            int keylen;
            const char *key = toml_table_key(input_table, i, &keylen);
            toml_table_t *each = toml_table_table(input_table, key);
            if (each)
            {
                for(int32_t j = 0; j < deep; j++)
                {
                    printf(" ");
                }
                printf("key #%d: %s\n", i, key);
                get_key(deep + 1,  each, output_table);
            }
        }
        return 0;
    }
    else
    {
        *output_table = input_table;
        return 1;
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
    // toml_value_t name = toml_table_string(conf, "name");
    // printf("name = \"%s\"\n", name.u.s);
    // free(name.u.s); // is ther any function to free the string in toml_value_t?
    toml_table_t *get_tables;
    int8_t a = get_key(0, conf, &get_tables);
    int32_t len = toml_table_len(get_tables);
    printf("%hhd\n", a);
    for (int32_t i = 0; i < len; i++)
    {
        int32_t keylen;
        const char *key = toml_table_key(get_tables, i, &keylen);
        printf("key #%d: %s\n", i, key);
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