// Client side C/C++ program to demonstrate Socket programming
#include <SimpleMon/client/client.h>
#include <SimpleMon/msg.h>

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

StatusMessage fillMsg()
{
    StatusMessage msg;
    return msg;
}

void client()
{

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "ID:TEST,{RAM:TEST, HDD:TEST, TEMP:33}";
    std::string output = "";
    StatusMessage msg = fillMsg();

    msg.uid = std::stoi(exec("id -u osboxes"));

    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    while (1)
    {
        std::cout << "sending.." << std::endl;
        send(sock, &msg, sizeof(msg), 0);
        sleep(6);
    }
    /*
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    */
    return;
}