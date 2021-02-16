#include <array>
#include <cstdio>

#pragma once

struct StatusMessage {
public:
    //int system_id = 66;
    //char nick[256] = "nick";
    //char ls[256] = "ls";
    int uid = 0;
    int free_mem = 0;
    int free_disk = 0;
    int used_cpu = 0;
};