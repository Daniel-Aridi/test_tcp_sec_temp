#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <stdbool.h>

typedef uint8_t u8;

bool x25519_generate_publicKey(u8 *publicKey, u8 *privatekey);
bool x25519_generate_sharedKey(u8 *out, const u8 *publicKey, const u8 *privatekey);

#endif