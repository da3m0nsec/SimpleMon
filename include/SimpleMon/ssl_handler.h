#pragma once

#include <string>

#include <botan/base64.h>
#include <botan/hex.h>
#include <botan/rng.h>

#include <botan/pk_keys.h>
#include <botan/x509_key.h>
#include <botan/pk_algs.h>
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/internal/workfactor.h>
#include <botan/data_src.h>
#include <botan/der_enc.h>
#include <botan/ber_dec.h>
#include <botan/oids.h>
#include <botan/aead.h>
#include <botan/pem.h>

std::vector <uint8_t> encrypt ();
