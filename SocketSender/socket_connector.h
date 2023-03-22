#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <winsock2.h>
#include <unistd.h>

const int rows = 30;
const int cols = 30;
const int port = 3000;

int** generate_random_array();

//2 dimension array to send to UI socket
// if arr is exit = -1 then exit connection
void send_array_and_message(SOCKET sockfd, int **arr);

//Return param -1 then error happend
SOCKET init_and_connect();

void close(SOCKET sockfd);