#include "include/connect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    time_t t = time(NULL);
    printf("UTC: %s\n", asctime(gmtime(&t)));

    Server server =  init_server();
    start_server(&server);

    exit(EXIT_SUCCESS);
}
    
