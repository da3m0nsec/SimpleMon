#pragma once

#include <SimpleMon/client/socket_client.h>
#include <SimpleMon/client/ssl_client.h>
#include <SimpleMon/config.h>
#include <SimpleMon/msg.h>

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <botan/pk_keys.h>
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/rng.h>
#include <botan/data_src.h>


std::string exec(const char *cmd);
StatusMessage fillMsg();
