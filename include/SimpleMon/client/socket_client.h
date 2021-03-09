#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <system_error>

class Socket_Client{
    int sock, opt=1;
    struct sockaddr_in serv_addr;

public:
    Socket_Client(std::string , int );
    Socket_Client(const Socket_Client&) = delete;
    ~Socket_Client();
    void send(const char* buf, size_t length);
};