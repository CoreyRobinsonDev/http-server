#include "include/connect.h"
#include "include/style.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    print_loading_bar();

    time_t t = time(NULL);
    printf("UTC: %s\n", asctime(gmtime(&t)));

    Server server =  init_server();
    start_server(&server);

    exit(EXIT_SUCCESS);
}
    
