#pragma once 

#include <SimpleMon/server/socket_server.h>

#include <botan/tls_server.h>
#include <botan/tls_policy.h>
#include <botan/hex.h>
#include <botan/internal/os_utils.h>
#include <botan/mem_ops.h>

#include <memory>

class Server_Credentials : public Botan::Credentials_Manager{
public:
    Server_Credentials():
        m_key(Botan::PKCS8::load_key("botan.randombit.net.key"))
        {};
    std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(
         const std::string& type,
         const std::string& context) override;
    std::vector<Botan::X509_Certificate> cert_chain(
         const std::vector<std::string>& cert_key_types,
         const std::string& type,
         const std::string& context) override;
    Botan::Private_Key* private_key_for(const Botan::X509_Certificate& cert,
         const std::string& type,
         const std::string& context) override;
    private:
         std::unique_ptr<Botan::Private_Key> m_key;
};

class Callbacks : public Botan::TLS::Callbacks{
public:
    std::shared_ptr <Socket_Client> sock = nullptr;

    void tls_emit_data(const uint8_t buf[], size_t size) override;
    void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override;
    void tls_alert(Botan::TLS::Alert alert) override;
    bool tls_session_established(const Botan::TLS::Session& session) override;
};


