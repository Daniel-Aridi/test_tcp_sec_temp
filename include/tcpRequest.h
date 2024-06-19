#ifndef TCP_REQUEST_H
#define TCP_REQUEST_H

#include "tcpMessage.pb-c.h"

extern uint8_t responseBuffer[];

int8_t manage_incomming_message(const int sock, uint8_t *inputBuffer, uint8_t dataLength);
void send_message(const int sock, TCPMESSAGE__Message *responseMessage);

#endif

