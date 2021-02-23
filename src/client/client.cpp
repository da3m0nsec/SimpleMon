#include <SimpleMon/client/client.h>

std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

StatusMessage fillMsg()
{
    std::cout << "Filling msg" << std::endl;
    StatusMessage msg;
    msg.uid = stoi(exec("id | awk '{ print $1 }' | cut -c 5-8"));
    auto hostname = exec ("hostname");
    std::copy_n(hostname.begin(), std::min(hostname.size(), sizeof(char)*32), msg.hostname.begin());
    msg.free_mem = stoi(exec("cat /proc/meminfo | grep MemFree | awk '{ print $2 }'"))/1000;
    msg.free_disk = stoi(exec("df -h | grep /dev/sda1 | awk '{ print $4 } ' | sed 's/.$//'")); // TO-DO:difference between G and M;
    //msg.free_disk = stoi(exec("df -h | grep 'D:' | awk '{ print $4 } ' | rev | cut -c 2- | rev"));
    msg.used_cpu = 100-stod(exec("top -b -i -n 1 | grep '%Cpu(s):' | awk -F',' '{print $4}' | grep -o  '[0-9]*\\.[0-9]'"));
    std::cout << "Msg filled" << std::endl;
    return msg;
}

int main()
{
    while (1)
    {
        Config conf;
        conf = parse_config ("/etc/simplemon-client/client.conf");

        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        StatusMessage msg = fillMsg();

        char buffer[1024] = {0};
        

        
        std::cout << "Sending msg" << std::endl;
        send(sock, &msg, sizeof(msg), 0);
        sleep(20);
    }
    return 0;
}