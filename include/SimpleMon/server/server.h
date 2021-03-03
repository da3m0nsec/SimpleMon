#pragma once

#include <SimpleMon/msg.h>
#include <SimpleMon/config.h>
#include <SimpleMon/server/sql_handler.h>
#include <SimpleMon/server/socket_server.h>

#include <iostream>
#include <thread>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <vector>
#include <string>

#define PORT 6540

int main(int argc, char const *argv[]);

std::vector<std::pair<std::string, int>> msgToSql(StatusMessage& msg);
