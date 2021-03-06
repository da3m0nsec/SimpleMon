#include <SimpleMon/config.h>

Config parse_config(std::string filename)
{
    Config conf;
    std::ifstream cFile(filename);
    if (cFile.is_open())
    {
        std::string line;
        while (getline(cFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if (line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            // Parse options
            if (name == "ip_address")
            {
                conf.ip_address = value;
            }
            else if (name == "port")
            {
                conf.port = stoi(value);
            }
            else if (name == "resend_period")
            {
                conf.resend_period = stoi(value);
            }
            else if (name == "logs")
            {
                conf.logs = value;
            }
            else if (name == "sql")
            {
                conf.sql = value;
            }
            else if (name == "monitor")
            {
                conf.monitor = value;
            }
            else if (name == "key_password")
            {
                conf.key_password = value;
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open " + filename + " for reading.\n";
    }
    return conf;
}
