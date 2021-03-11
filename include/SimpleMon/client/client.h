#pragma once

#include <SimpleMon/msg.h>
#include <SimpleMon/config.h>
#include <SimpleMon/client/socket_client.h>
#include <SimpleMon/client/ssl_client.h>

#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

#include <botan/pkcs8.h>
#include <botan/hex.h>
#include <botan/pk_keys.h>
#include <botan/pubkey.h>
#include <botan/auto_rng.h>
#include <botan/rng.h>


std::string exec(const char *cmd);
StatusMessage fillMsg();
