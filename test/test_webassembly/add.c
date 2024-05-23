#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
    return a + b;
}
EMSCRIPTEN_KEEPALIVE
void test() {
    // return "../test.png";
    // write text to file
    FILE *f = fopen("test.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "some text\n");
    fclose(f);
    f = fopen("test.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    char c;
    while ((c = fgetc(f)) != EOF)
    {
        printf("%c", c);
    }
    fclose(f);
}

EMSCRIPTEN_KEEPALIVE
void hello(char *ptr) {
    snprintf(ptr, 100, "Hello World");
}

EMSCRIPTEN_KEEPALIVE
char *get_string() {
    static char str[] = "./test.png";
    return str;
}
