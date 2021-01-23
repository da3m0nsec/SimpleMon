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
    std::cout << "Filling msg" << std::endl;
    StatusMessage msg;
    msg.uid = stoi(exec("id -u osboxes"));
    msg.free_mem = stoi(exec("cat /proc/meminfo | grep MemFree | awk '{ print $2 }'"))/1000;
    msg.free_disk = stoi(exec("df -h | grep /dev/sda1 | awk '{ print $4 } ' | sed 's/.$//'")); // TO-DO:difference between G and M;
    msg.used_cpu = 100-stod(exec("top -b -i -n 1 | grep Cpu | awk '{ print $ 8 }'"));
    std::cout << "Msg filled" << std::endl;
    return msg;
}

void client()
{
    while (1)
    {

        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        StatusMessage msg = fillMsg();

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

        
        std::cout << "Sending msg" << std::endl;
        send(sock, &msg, sizeof(msg), 0);
        sleep(20);

        /*
        valread = read( sock , buffer, 1024); 
        printf("%s\n",buffer ); 
        */
    }
    return;
}