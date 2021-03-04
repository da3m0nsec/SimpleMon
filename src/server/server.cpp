#include <SimpleMon/server/server.h>

int main(int argc, char const *argv[])
{
    Config conf;
    conf = parse_config ("../config/server.conf");
    
    char buffer[5120] = {};
    StatusMessage msg;

    std::shared_ptr<Socket_Server> s = std::make_shared<Socket_Server>(conf.port);

    // prepare all the parameters
	Callbacks callbacks;
    callbacks.sock = s
	Botan::AutoSeeded_RNG rng;
	Botan::TLS::Session_Manager_In_Memory session_mgr(rng);
	Server_Credentials creds;
	Botan::TLS::Strict_Policy policy;

	// accept tls connection from client
	Botan::TLS::Server server(  callbacks,
								session_mgr,
								creds,
								policy,
								rng);

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

    ret.reserve(5);

    ret.emplace_back(msg.hostname.data(), 0);
    ret.emplace_back("uid", msg.uid);
    ret.emplace_back("free_mem", msg.free_mem);
    ret.emplace_back("free_disk", msg.free_disk);
    ret.emplace_back("used_cpu", msg.used_cpu);

    return ret;
}
