#pragma once

#include <SimpleMon/msg.h>

#include <vector>
#include <string>

#define PORT 6540

int main(int argc, char const *argv[]);

std::vector<std::pair<std::string, int>> msgToSql(StatusMessage& msg);