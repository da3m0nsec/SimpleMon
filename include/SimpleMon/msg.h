#include <array>
#include <cstdio>

#pragma once

struct StatusMessage {
public:
    std::array <char, 32> hostname;
    int uid = 0;
    int free_mem = 0;
    int free_disk = 0;
    int used_cpu = 0;
};