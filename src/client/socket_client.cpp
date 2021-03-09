#include <SimpleMon/client/socket_client.h>

Socket_Client::Socket_Client(std::string addr, int port) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            throw std::system_error(EAGAIN,  std::generic_category(),  strerror(errno));
        }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr) <= 0)
    {
        throw std::system_error(EAGAIN,  std::generic_category(),  strerror(errno));
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << addr << " " << port << std::endl;
        throw std::system_error(EAGAIN,  std::generic_category(),  strerror(errno));
    }
}

Socket_Client::~Socket_Client(){
    close(sock);
}

void Socket_Client::send(const char* buf, size_t length){
    ::send(sock, buf, length, 0);
}