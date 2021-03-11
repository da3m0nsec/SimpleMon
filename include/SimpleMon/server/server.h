#pragma once

#include <SimpleMon/config.h>
#include <SimpleMon/msg.h>
#include <SimpleMon/server/socket_server.h>
#include <SimpleMon/server/sql_handler.h>
#include <SimpleMon/server/ssl_server.h>

#include <botan/auto_rng.h>
#include <botan/certstor_system.h>
#include <botan/hex.h>
#include <botan/ocsp.h>
#include <botan/tls_client.h>
#include <botan/tls_policy.h>
#include <botan/x509path.h>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[]);

std::vector<std::pair<std::string, int>> msgToSql(StatusMessage &msg);
