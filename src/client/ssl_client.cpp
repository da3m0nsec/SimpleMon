/**
 * @brief Callbacks invoked by TLS::Channel.
 *
 * Botan::TLS::Callbacks is an abstract class.
 * For improved readability, only the functions that are mandatory
 * to implement are listed here. See src/lib/tls/tls_callbacks.h.
 */
class Callbacks : public Botan::TLS::Callbacks
{
   public:
      void tls_emit_data(const uint8_t data[], size_t size) override
         {
         // send data to tls client, e.g., using BSD sockets or boost asio
         }

      void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override
         {
         // process full TLS record received by tls client, e.g.,
         // by passing it to the application
         }

      void tls_alert(Botan::TLS::Alert alert) override
         {
         // handle a tls alert received from the tls server
         }

      bool tls_session_established(const Botan::TLS::Session& session) override
         {
         // the session with the tls client was established
         // return false to prevent the session from being cached, true to
         // cache the session in the configured session manager
         return false;
         }
};

/**
 * @brief Callbacks invoked by TLS::Channel.
 *
 * Botan::TLS::Callbacks is an abstract class.
 * For improved readability, only the functions that are mandatory
 * to implement are listed here. See src/lib/tls/tls_callbacks.h.
 */
class Callbacks : public Botan::TLS::Callbacks
{
   public:
      void tls_emit_data(const uint8_t data[], size_t size) override
        {
         // send data to tls server, e.g., using BSD sockets or boost asio
          size_t offset = 0;

            while(length)
            {
                ssize_t sent = ::send(m_sockfd, buf + offset, length, MSG_NOSIGNAL);

                if(sent == -1)
                {
                if(errno == EINTR)
                    {
                    sent = 0;
                    }
                else
                    {
                    throw CLI_Error("Socket write failed errno=" + std::to_string(errno));
                    }
                }

                offset += sent;
                length -= sent;
            }
        }

      void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override
         {
         // process full TLS record received by tls server, e.g.,
         // by passing it to the application
            for(size_t i = 0; i != buf_size; ++i)
            {
                std::cout << buf[i];
            }

         }

      void tls_alert(Botan::TLS::Alert alert) override
         {
         // handle a tls alert received from the tls server
            std::cout << "Alert: " << alert.type_string() << "\n";
         }

      bool tls_session_established(const Botan::TLS::Session& session) override
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
};

void ssl_client () {
    // prepare all the parameters
    Callbacks callbacks;
    Botan::AutoSeeded_RNG rng;
    Botan::TLS::Session_Manager_In_Memory session_mgr(rng);
    Client_Credentials creds;
    Botan::TLS::Strict_Policy policy;

    // open the tls connection
    Botan::TLS::Client client(callbacks,
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