#include "../src/basic_include.h"
#include "../src/third_party/toml-c/header/toml-c.h"


int main(int argc, char *argv[])
{
    int32_t fp = open("test.toml", O_RDONLY);
    if (fp == -1)
    {
        return -1;
    }
    struct stat st;
    if(fstat(fp, &st) <0)
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
    toml_free(conf);
    free(name.u.s); // is ther any function to free the string in toml_value_t?
    
    
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