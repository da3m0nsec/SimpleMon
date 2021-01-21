#include <SimpleMon/server/server.h>

#include <SimpleMon/server/sql_handler.h>
#include <SimpleMon/msg.h>
#include <SimpleMon/client/client.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>

int main(int argc, char const *argv[])
{

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[5120] = {};
    char *hello = "ID:TEST,{RAM:TEST, HDD:TEST, TEMP:33}";
    StatusMessage msg;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    std::cout << "Creating send thread" << std::endl;

    std::thread send_thread (client);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "4" << std::endl;

    while (1) {
        int rest = sqlTest();
        sleep(10);
    }

    while (1)
    {
        std::cout << "receiving.." << std::endl;
        valread = read(new_socket, buffer, 5120);
        //printf("%s\n",buffer );
        memcpy(&msg, buffer, sizeof(msg));
        std::cout << msg.system_id << std::endl;
        std::cout << msg.uid << std::endl;
    };
    return 0;
}