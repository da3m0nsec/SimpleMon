#include <array>
#include <cstdio>

#pragma once

struct StatusMessage {
public:
    int system_id = 66;
    char nick[256] = "nick";
    char ls[256] = "ls";
    int uid = 0;
};