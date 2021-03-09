#include <array>
#include <cstdio>

#pragma once

struct StatusMessage {
public:
    std::array <char, 32> hostname;
    short uid = 0;
    short free_mem = 0;
    short free_disk = 0;
    short used_cpu = 0;
};