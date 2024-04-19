#ifndef STYLE_H
#define STYLE_H

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

#define LABELS\
    X(INFO)\
    X(ERROR)\

#define X(name) name,
typedef enum {
    LABELS
} Label;
#undef X

#define BOLD "\033[1;1m"
#define UNDERLINE "\033[1;4m"
#define BLINK "\033[1;5m"
#define BLACK "\033[1;30m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN  "\033[1;36m"
#define WHITE "\033[1;37m"
#define ESC "\033[1;0m"

void print_info(Label label, const char *msg);
void print_loading_bar();

#endif // !STYLE_H
