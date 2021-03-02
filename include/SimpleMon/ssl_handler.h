#pragma once

#include <string>
#include <iostream>
#include <memory>

#include <botan/base64.h>
#include <botan/hex.h>
#include <botan/rng.h>

#include <botan/auto_rng.h>
#include <botan/pk_keys.h>
#include <botan/x509_key.h>
#include <botan/pk_algs.h>
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/data_src.h>
#include <botan/der_enc.h>
#include <botan/ber_dec.h>
#include <botan/oids.h>
#include <botan/aead.h>
#include <botan/pem.h>
#include <botan/ecdsa.h>

#include <botan/tls_client.h>
#include <botan/tls_callbacks.h>
#include <botan/tls_session_manager.h>
#include <botan/tls_policy.h>
#include <botan/auto_rng.h>
#include <botan/certstor.h>
#include <botan/pk_keys.h>


std::vector<uint8_t> encrypt ();

bool decrypt ();

Botan::SecureVector<uint8_t> open_key (std::string path_to_key);



