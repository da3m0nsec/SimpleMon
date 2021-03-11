#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <system_error>

class Socket_Server
{
    int server_fd, sock, opt = 1;
    struct sockaddr_in address;

  public:
    Socket_Server(int port);
    Socket_Server(const Socket_Server &) = delete;
    ~Socket_Server();
    void read(char *buf, size_t length);
};