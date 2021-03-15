#include <SimpleMon/server/ssl_server.h>

#include <botan/hex.h>
#include <botan/mem_ops.h>
#include <botan/tls_policy.h>
#include <botan/tls_server.h>

#include <fstream>
#include <list>

/**
 * @brief Credentials storage for the tls server.
 *
 * It returns a certificate and the associated private key to
 * authenticate the tls server to the client.
 * TLS client authentication is not requested.
 * See src/lib/tls/credentials_manager.h.
 */

std::vector<Botan::Certificate_Store *> Server_Credentials::trusted_certificate_authorities(const std::string &type,
                                                                                            const std::string &context)
{
    // if client authentication is required, this function
    // shall return a list of certificates of CAs we trust
    // for tls client certificates, otherwise return an empty list
    return std::vector<Botan::Certificate_Store *>();
}

std::vector<Botan::X509_Certificate> Server_Credentials::cert_chain(const std::vector<std::string> &cert_key_types,
                                                                    const std::string &type, const std::string &context)
{
    // return the certificate chain being sent to the tls client
    // e.g., the certificate file "botan.randombit.net.crt"
    return {Botan::X509_Certificate("../cert/ssl/cert")};
}

Botan::Private_Key *Server_Credentials::private_key_for(const Botan::X509_Certificate &cert, const std::string &type,
                                                        const std::string &context)
{
    // return the private key associated with the leaf certificate,
    // in this case the one associated with "botan.randombit.net.crt"
    return m_key.get();
}

/**
 * @brief Callbacks invoked by TLS::Channel.
 *
 * Botan::TLS::Callbacks is an abstract class.
 * For improved readability, only the functions that are mandatory
 * to implement are listed here. See src/lib/tls/tls_callbacks.h.
 */

void Callbacks::tls_emit_data(const uint8_t data[], size_t size)
{
    // send data to tls client, e.g., using BSD sockets or boost asio
    if (sock == nullptr)
    {
        throw "No socket in Callbacks";
    }
    else
    {
        std::cout << "Trying to emit data :(" << std::endl;
        // sock->send(buf, size);
    }
}

void Callbacks::tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size)
{
    // process full TLS record received by tls client, e.g.,
    // by passing it to the application
    std::vector<std::string> m_pending_output;
    std::string m_line_buf;
    for (size_t i = 0; i != size; ++i)
    {
        const char c = static_cast<char>(data[i]);
        m_line_buf += c;
        if (c == '\n')
        {
            m_pending_output.push_back(m_line_buf);
            m_line_buf.clear();
        }
    }
}

void Callbacks::tls_alert(Botan::TLS::Alert alert)
{
    // handle a tls alert received from the tls server
    std::cout << "Alert: " << alert.type_string() << std::endl;
}

bool Callbacks::tls_session_established(const Botan::TLS::Session &session)
{
    // the session with the tls client was established
    // return false to prevent the session from being cached, true to
    // cache the session in the configured session manager
    return false;
}

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