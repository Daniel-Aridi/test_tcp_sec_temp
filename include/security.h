#ifndef SECURITY_H
#define SECURITY_H

#include "tcpMessage.pb-c.h" 

int8_t perform_handshake(const uint8_t sock, TCPMESSAGE__Message *messageRequest, TCPMESSAGE__Message *sec1Payload);

#endif