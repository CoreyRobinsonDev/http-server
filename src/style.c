#include "include/style.h"
#include <stdio.h>

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
