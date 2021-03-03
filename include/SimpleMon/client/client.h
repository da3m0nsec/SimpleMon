#pragma once

#include <SimpleMon/msg.h>
#include <SimpleMon/config.h>
#include <SimpleMon/client/socket_client.h>



#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

#define PORT 6540

std::string exec(const char *cmd);
StatusMessage fillMsg();
