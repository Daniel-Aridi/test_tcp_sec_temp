#ifndef TCPIP
#define TCPIP

void start_tcp_server();
void tcp_send(int sock, uint8_t *dataToSend, uint8_t length);


#endif