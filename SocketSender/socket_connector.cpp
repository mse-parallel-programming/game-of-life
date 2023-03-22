#include "socket_connector.h"

#pragma comment(lib, "ws2_32.lib")


int** generate_random_array() {
    int** arr = new int*[rows];
    srand(time(NULL));
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            arr[i][j] = rand() % 2;
        }
    }
    return arr;
}

void send_array_and_message(SOCKET sockfd, int **arr) {
    if (**arr == -1) {
        const char* message = "exit";
        send(sockfd, message, strlen(message), 0);
    } else {
        int length = rows * cols + 1;
        int buffer[length];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                buffer[i * cols + j] = arr[i][j];
            }
        }
        buffer[length - 1] = 2;
        send(sockfd, (const char*)buffer, sizeof(buffer), 0);
    }
}

SOCKET init_and_connect() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::

        cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    SOCKET sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        std::cerr << "connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return -1;
    }

    return sockfd;
}

void close(SOCKET sockfd) {
    closesocket(sockfd);
    WSACleanup();
}
