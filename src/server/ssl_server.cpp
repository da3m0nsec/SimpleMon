#include <SimpleMon/ssl_server.h>

#include <botan/tls_server.h>
#include <botan/tls_policy.h>
#include <botan/hex.h>
#include <botan/internal/os_utils.h>
#include <botan/mem_ops.h>

#include <list>
#include <fstream>

/**
 * @brief Credentials storage for the tls server.
 *
 * It returns a certificate and the associated private key to
 * authenticate the tls server to the client.
 * TLS client authentication is not requested.
 * See src/lib/tls/credentials_manager.h.
 */

class Server_Credentials : public Botan::Credentials_Manager
{
   public:
        Server_Credentials() : m_key(Botan::PKCS8::load_key("botan.randombit.net.key"))
        {
        }

      std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(
         const std::string& type,
         const std::string& context) override
         {
            // if client authentication is required, this function
            // shall return a list of certificates of CAs we trust
            // for tls client certificates, otherwise return an empty list
            return std::vector<Certificate_Store*>();
         }

      std::vector<Botan::X509_Certificate> cert_chain(
        const std::vector<std::string>& cert_key_types,
        const std::string& type,
        const std::string& context) override
        {
            // return the certificate chain being sent to the tls client
            // e.g., the certificate file "botan.randombit.net.crt"
            return { Botan::X509_Certificate("botan.randombit.net.crt") };
        }

      Botan::Private_Key* private_key_for(const Botan::X509_Certificate& cert,
         const std::string& type,
         const std::string& context) override
         {
         // return the private key associated with the leaf certificate,
         // in this case the one associated with "botan.randombit.net.crt"
            return &m_key;
         }

      private:
         std::unique_ptr<Botan::Private_Key> m_key;
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
            // send data to tls client, e.g., using BSD sockets or boost asio
            ssize_t sent = ::send(m_socket, buf, static_cast<sendrecv_len_type>(length), MSG_NOSIGNAL);

            if(sent == -1)
                {
                error_output() << "Error writing to socket - " << err_to_string(errno) << std::endl;
                }
            else if(sent != static_cast<ssize_t>(length))
                {
                error_output() << "Packet of length " << length << " truncated to " << sent << std::endl;
                }
            }

      void tls_record_received(uint64_t seq_no, const uint8_t data[], size_t size) override
         {
         // process full TLS record received by tls client, e.g.,
         // by passing it to the application
            for(size_t i = 0; i != input_len; ++i)
            {
                const char c = static_cast<char>(input[i]);
                m_line_buf += c;
                if(c == '\n')
                {
                    m_pending_output.push_back(m_line_buf);
                    m_line_buf.clear();
                }
            }
         }

      void tls_alert(Botan::TLS::Alert alert) override
         {
         // handle a tls alert received from the tls server
         output() << "Alert: " << alert.type_string() << std::endl;
         }

      bool tls_session_established(const Botan::TLS::Session& session) override
         {
         // the session with the tls client was established
         // return false to prevent the session from being cached, true to
         // cache the session in the configured session manager
         return false;
         }
};

/*
Botan::TLS::Server build_TLS_Server () { //CANT USE THIS
	// prepare all the parameters
	Callbacks callbacks;
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
    return server;

	// read data received from the tls client, e.g., using BSD sockets or boost asio
	// and pass it to server.received_data().
	// ...

	// send data to the tls client using server.send_data()
}
*/