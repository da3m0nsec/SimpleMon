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
    auto hostname = exec("hostname");
    std::copy_n(hostname.begin(), std::min(hostname.size(), sizeof(char) * 32), msg.hostname.begin());
    msg.free_mem = stoi(exec("cat /proc/meminfo | grep MemFree | awk '{ print $2 }'")) / 1000;
    msg.free_disk =
        stoi(exec("df -h | grep /dev/sda1 | awk '{ print $4 } ' | sed 's/.$//'")); // TO-DO:difference G and M;
    // msg.free_disk = stoi(exec("df -h | grep 'D:' | awk '{ print $4 } ' | rev | cut -c 2- | rev"));
    msg.used_cpu =
        100 - stod(exec("top -b -i -n 1 | grep '%Cpu(s):' | awk -F',' '{print $4}' | grep -o  '[0-9]*\\.[0-9]'"));
    return msg;
}

int main(int argc, char const *argv[])
{
    while (1)
    {
        Config conf;
        conf = parse_config("/etc/simplemon-client/config/client.conf");
        StatusMessage msg = fillMsg();
        char buffer[768] = {0};
        std::unique_ptr<Socket_Client> s = std::make_unique<Socket_Client>(conf.ip_address, conf.port);
        
        std::unique_ptr<Botan::RandomNumberGenerator> rng(new Botan::AutoSeeded_RNG);

        // load keypair
        Botan::DataSource_Stream in("/etc/simplemon-client/keys/client.priv");
        std::unique_ptr<Botan::Private_Key> priv(Botan::PKCS8::load_key(in, conf.key_password));
        std::unique_ptr<Botan::Public_Key> pub(Botan::X509::load_key("/etc/simplemon-client/keys/server.pub"));

        Botan::PK_Encryptor_EME enc(*pub, *rng.get(), "EME1(SHA-256)");
        Botan::PK_Signer signer(*priv, *rng.get(), "EMSA1(SHA-256)");

        std::vector<uint8_t> ct = enc.encrypt((const unsigned char *)&msg, sizeof(msg), *rng.get());
        

        signer.update(ct);
        std::vector<uint8_t> signature = signer.signature(*rng.get());
        if (conf.logs != "none") {
            std::cout << "Max enc size = " << enc.maximum_input_size() << std::endl;
            std::cout << "Msg size = " << ct.size() << std::endl;
            std::cout << "Signature size = " << signature.size() << std::endl;
            std::cout << "Msg + Signature size = " << ct.size() + signature.size() << std::endl;
        }
        memcpy(&buffer, ct.data(), ct.size());
        memcpy(&buffer[384], signature.data(), signature.size());

        // Send encrypted and signed msg
        s->send((const char *)&buffer, sizeof(buffer));

        sleep(conf.resend_period);
    }
}