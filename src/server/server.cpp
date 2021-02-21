#include <SimpleMon/server/server.h>
#include <SimpleMon/config.h>
#include <SimpleMon/server/sql_handler.h>

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
    Config conf;
    conf = parse_config ("../config/server.conf");
    
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[5120] = {};
    StatusMessage msg;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    // Forcefully attaching socket to the port 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(conf.port);

    // Forcefully attaching socket to the port 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while (1)
    { 
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
        
        std::cout << "Receiving" << std::endl;
        valread = read(new_socket, buffer, 5120);
        //printf("%s\n",buffer );
        memcpy(&msg, buffer, sizeof(msg));
        ingestToSql(msgToSql(msg));
    }
    return 0;
}

StatusVector msgToSql(StatusMessage& msg){
    StatusVector ret;

    ret.emplace_back(msg.hostname.data(), 0);
    ret.emplace_back("uid", msg.uid);
    ret.emplace_back("free_mem", msg.free_mem);
    ret.emplace_back("free_disk", msg.free_disk);
    ret.emplace_back("used_cpu", msg.used_cpu);

    return ret;
}
