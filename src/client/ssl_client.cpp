#include <SimpleMon/client/ssl_client.h>

/**
 * @brief Credentials storage for the tls client.
 *
 * It returns a list of trusted CA certificates from a local directory.
 * TLS client authentication is disabled. See src/lib/tls/credentials_manager.h.
 */
Client_Credentials::Client_Credentials()
    {
    // Here we base trust on the system managed trusted CA list
    m_stores.push_back(new Botan::System_Certificate_Store);
    }

std::vector<Botan::Certificate_Store*> Client_Credentials::trusted_certificate_authorities(
    const std::string& type,
    const std::string& context) override
    {
    // return a list of certificates of CAs we trust for tls server certificates
    // ownership of the pointers remains with Credentials_Manager
    return m_stores;
    }

std::vector<Botan::X509_Certificate> Client_Credentials::cert_chain(
    const std::vector<std::string>& cert_key_types,
    const std::string& type,
    const std::string& context) override
    {
    // when using tls client authentication (optional), return
    // a certificate chain being sent to the tls server,
    // else an empty list
    return std::vector<Botan::X509_Certificate>();
    }

Botan::Private_Key* Client_Credentials::private_key_for(const Botan::X509_Certificate& cert,
    const std::string& type,
    const std::string& context) override
    {
    // when returning a chain in cert_chain(), return the private key
    // associated with the leaf certificate here
    return nullptr;
    }


/**
 * @brief Callbacks invoked by TLS::Channel.
 *
 * Botan::TLS::Callbacks is an abstract class.
 * For improved readability, only the functions that are mandatory
 * to implement are listed here. See src/lib/tls/tls_callbacks.h.
 */

void Callbacks::tls_emit_data(const uint8_t buf[], size_t size) override
{
    // send data to tls server, e.g., using BSD sockets or boost asio
    if (sock == nullptr){
        throw "No socket in Callbacks";
    }
    else {
        sock->send(buf, size);
    }
}

void Callbacks::tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override
    {
    // process full TLS record received by tls server, e.g.,
    // by passing it to the application
    for(size_t i = 0; i != buf_size; ++i)
    {
        std::cout << buf[i];
    }

    }

void Callbacks::tls_alert(Botan::TLS::Alert alert) override
    {
    // handle a tls alert received from the tls server
    std::cout << "Alert: " << alert.type_string() << "\n";
    }

bool Callbacks::tls_session_established(const Botan::TLS::Session& session) override
    {
    // the session with the tls server was established
    // return false to prevent the session from being cached, true to
    // cache the session in the configured session manager
        std::cout << "Handshake complete, " << session.version().to_string()
            << " using " << session.ciphersuite().to_string() << "\n";

    if(!session.session_id().empty())
        {
        std::cout << "Session ID " << Botan::hex_encode(session.session_id()) << "\n";
        }

    if(!session.session_ticket().empty())
        {
        std::cout << "Session ticket " << Botan::hex_encode(session.session_ticket()) << "\n";
        }

    if(flag_set("print-certs"))
        {
        const std::vector<Botan::X509_Certificate>& certs = session.peer_certs();

        for(size_t i = 0; i != certs.size(); ++i)
        {
            std::cout << "Certificate " << i + 1 << "/" << certs.size() << "\n";
            std::cout << certs[i].to_string();
            std::cout << certs[i].PEM_encode();
        }
        }

    return true;
    }


    /*
void ssl_client (std::shared_ptr <Socket_Client> s) {
   // prepare all the parameters
   Callbacks callbacks;
   callbacks.sock = s; //TODO: move this to constructor
   Botan::AutoSeeded_RNG rng;
   Botan::TLS::Session_Manager_In_Memory session_mgr(rng);
   Client_Credentials creds;
   Botan::TLS::Strict_Policy policy;

    // open the tls connection
    Botan::TLS::Client client(  callbacks,
                                session_mgr,
                                creds,
                                policy,
                                rng,
                                Botan::TLS::Server_Information("botan.randombit.net", 443),
                                Botan::TLS::Protocol_Version::TLS_V12);

    while(!client.is_closed())
        {
        // read data received from the tls server, e.g., using BSD sockets or boost asio
        // ...

        // send data to the tls server using client.send_data()
        }
}
*/