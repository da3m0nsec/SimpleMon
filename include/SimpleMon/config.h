#include <algorithm>
#include <fstream>
#include <iostream>

struct Config
{
    int port = 6540;
    std::string logs = "none";

    std::string ip_address = "127.0.0.1";
    int resend_period = 20;
    std::string key_password = "";
};

Config parse_config(std::string filename);