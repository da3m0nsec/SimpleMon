#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using StatusVector = std::vector<std::pair<std::string, int>>;

struct SQL_Config
{
    int port = 6540;
    std::string logs = "none";
    std::string ip_address = "127.0.0.1";
    std::string password = "";
};

void ingestToSql(StatusVector msg);
SQL_Config parse_sql_config(std::string filename);
