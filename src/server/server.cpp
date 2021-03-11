#include <SimpleMon/server/server.h>

int main(int argc, char const *argv[])
{
    Config conf;
    conf = parse_config("../config/server.conf");

    short buffer[1024] = {};
    StatusMessage msg;
    std::unique_ptr<Socket_Server> s = std::make_unique<Socket_Server>(conf.port);
    /*
    // prepare all the parameters
    Callbacks callbacks;
    callbacks.sock = s;
    Botan::AutoSeeded_RNG rng;
    Botan::TLS::Session_Manager_In_Memory session_mgr(rng);
    Server_Credentials creds (rng);
    Botan::TLS::Strict_Policy policy;

    // accept tls connection from client
    Botan::TLS::Server server(  callbacks,
                                session_mgr,
                                creds,
                                policy,
                                rng);
                                */

    std::unique_ptr<Botan::RandomNumberGenerator> rng(new Botan::AutoSeeded_RNG);

    std::unique_ptr<Botan::Private_Key> priv(Botan::PKCS8::load_key("../cert/ssl/nopass.key", *rng.get()));
    std::unique_ptr<Botan::Public_Key> pub(Botan::X509::load_key("../cert/ssl/nopass.cert"));

    Botan::PK_Decryptor_EME dec(*priv, *rng.get(), "EME1(SHA-256)");
    Botan::PK_Verifier verifier(*pub, "EMSA1(SHA-256)");

    while (1)
    {
        s->read((char *)buffer, 768);
        std::cout << "Msg is "
                  << (verifier.verify_message((const unsigned char *)&buffer, 384, (const unsigned char *)&buffer + 384,
                                              384)
                          ? "valid"
                          : "invalid")
                  << std::endl;
        memcpy(&msg, dec.decrypt((const unsigned char *)&buffer, 384).data(), sizeof(msg));

        // server.received_data(buffer, sizeof(msg));

        // memcpy(&msg, dec.decrypt(enc.encrypt((const unsigned char*)&msg, sizeof(msg), *rng.get())).data(),
        // sizeof(msg));

        // memcpy(&msg, buffer, sizeof(msg));
        ingestToSql(msgToSql(msg));
        std::cout << "Decrypted:" << msg.uid << " " << msg.used_cpu << " " << std::endl;
    }
}

StatusVector msgToSql(StatusMessage &msg)
{
    StatusVector ret;

    ret.reserve(5);

    ret.emplace_back(msg.hostname.data(), 0);
    ret.emplace_back("uid", msg.uid);
    ret.emplace_back("free_mem", msg.free_mem);
    ret.emplace_back("free_disk", msg.free_disk);
    ret.emplace_back("used_cpu", msg.used_cpu);

    return ret;
}
