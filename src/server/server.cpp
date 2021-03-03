#include <SimpleMon/server/server.h>

int main(int argc, char const *argv[])
{
    Config conf;
    conf = parse_config ("../config/server.conf");
    
    char buffer[5120] = {};
    StatusMessage msg;

    Socket_Server sock(conf.port);

    while (1)
    { 
        sock.read(buffer, sizeof(msg));
        memcpy(&msg, buffer, sizeof(msg));
        ingestToSql(msgToSql(msg));
    }
    return 0;
}

StatusVector msgToSql(StatusMessage& msg){
    StatusVector ret;

    ret.emplace_back(msg.hostname.data(), 0);
    ret.emplace_back("uid", msg.uid);
    ret.emplace_back("free_mem", msg.free_mem);
    ret.emplace_back("free_disk", msg.free_disk);
    ret.emplace_back("used_cpu", msg.used_cpu);

    return ret;
}
