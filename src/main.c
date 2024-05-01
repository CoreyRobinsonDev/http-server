#include "include/connect.h"
// #include "include/style.h"
#include <stdlib.h>


int main(int argc, char* argv[]) {
    // print_loading_bar();

    Server server =  init_server();
    start_server(&server);

    exit(EXIT_SUCCESS);
}
    
