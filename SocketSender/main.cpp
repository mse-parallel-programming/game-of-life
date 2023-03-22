#include <iostream>
#include "socket_connector.h"

int main() {
    SOCKET fd = init_and_connect();
    if(fd == -1) {
        std::cerr << "Exiting..."<< std::endl;
        return 1;
    }

    int length = 500;
    for(int i = 0; i < length; i++) {
        int** arr = generate_random_array();
        send_array_and_message(fd, arr);
        usleep(200);
    }

    //Exit key
    int** exit = new int*[1];
    exit[0] = new int[1];
    exit[0][0] = -1;

    send_array_and_message(fd, exit);

    close(fd);
    return 0;
}
