#include <SimpleMon/server/socket_server.h>

Socket_Server::Socket_Server(int port) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        //perror("socket failed");
        throw std::system_error(EAGAIN,  std::generic_category(),  strerror(errno));
    }

    // Forcefully attaching socket to the port 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                &opt, sizeof(opt)))
    {
        //perror("setsockopt");
        throw std::system_error(EAGAIN,  std::generic_category(),  strerror(errno));
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        //perror("bind failed");
        throw std::system_error(EAGAIN,  std::generic_category(),  strerror(errno));
    }
}

Socket_Server::~Socket_Server(){
    close(server_fd);
}

void Socket_Server::read(char* buf, size_t length){
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int addrlen = sizeof(address);
    if ((sock = accept4(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen, SOCK_NONBLOCK)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
        
    std::cout << "Receiving" << std::endl;
    ::read(sock, buf, length);
}