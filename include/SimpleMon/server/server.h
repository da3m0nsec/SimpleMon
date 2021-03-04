#pragma once

#include <SimpleMon/msg.h>
#include <SimpleMon/config.h>
#include <SimpleMon/server/sql_handler.h>
#include <SimpleMon/server/socket_server.h>
#include <SimpleMon/server/ssl_server.h>


#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]);

std::vector<std::pair<std::string, int>> msgToSql(StatusMessage& msg);
