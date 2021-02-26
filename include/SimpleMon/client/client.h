#pragma once

#include <SimpleMon/msg.h>
#include <SimpleMon/config.h>

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

#define PORT 6540

std::string exec(const char *cmd);
StatusMessage fillMsg();
