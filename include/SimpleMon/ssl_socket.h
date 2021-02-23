#include <string>
#include <iostream>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>


int create_server_socket(int port);

int create_client_socket(std::string ip_address, int port);

void init_openssl();

void cleanup_openssl();

SSL_CTX *create_context(std::string method_op);

void configure_context(SSL_CTX *ctx);

void DisplayCerts(SSL *ssl);


