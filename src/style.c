#include "include/style.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define X(name) [name] = #name,
char *label_str[] = {
    LABELS
};
void print_info(Label label, const char *msg) {
    char *color = {0};

    switch (label) {
        case INFO: color = BLUE; break;
        case ERROR: color = RED; break;
        default: color = WHITE;
    }

    printf(
        "%s%s[%s]%s\t%s%s\n", 
        UNDERLINE,
        color,
        label_str[label],
        WHITE,
        msg,
        ESC
    );
}

void print_loading_bar() {
    clear();
    int str_len = 50;
    char str[str_len+1];
    char* text = "starting server ";
    memset(&str, '\0', sizeof(char) * str_len+1);
    memset(&str, '.', sizeof(char) * str_len);

    printf("%s[%s]\n", text, str);
    for (int i = 0; i < str_len; i++) {
        gotoxy(0, 0);
        usleep(250 * (i*10));
        str[i] = '#';
        printf("%s[%s]\n", text, str);
    }
    clear();
}
