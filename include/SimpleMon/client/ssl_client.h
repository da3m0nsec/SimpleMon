#pragma once

#include <SimpleMon/client/socket_client.h>

#include <botan/auto_rng.h>
#include <botan/certstor_system.h>
#include <botan/hex.h>
#include <botan/ocsp.h>
#include <botan/tls_client.h>
#include <botan/tls_policy.h>
#include <botan/x509path.h>

#include <memory>

class Client_Credentials : public Botan::Credentials_Manager
{
  public:
    Client_Credentials();
    std::vector<Botan::Certificate_Store *> trusted_certificate_authorities(const std::string &type,
                                                                            const std::string &context) override;
    std::vector<Botan::X509_Certificate> cert_chain(const std::vector<std::string> &cert_key_types,
                                                    const std::string &type, const std::string &context) override;
    Botan::Private_Key *private_key_for(const Botan::X509_Certificate &cert, const std::string &type,
                                        const std::string &context) override;

  private:
    std::vector<Botan::Certificate_Store *> m_stores;
};

class Callbacks : public Botan::TLS::Callbacks
{
  public:
    std::shared_ptr<Socket_Client> sock = nullptr;

    void tls_emit_data(const uint8_t buf[], size_t size) override;
    void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override;
    void tls_alert(Botan::TLS::Alert alert) override;
    bool tls_session_established(const Botan::TLS::Session &session) override;
};