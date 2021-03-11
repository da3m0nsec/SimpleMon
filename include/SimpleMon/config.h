#include <algorithm>
#include <fstream>
#include <iostream>

struct Config
{
    std::string ip_address = "127.0.0.1";
    int port = 6540;
    int resend_period = 3;
};

Config parse_config(std::string filename);