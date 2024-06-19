/****************************************************
*    managing incoming requests of type proto messages
*    initiating secure sessions 
*    defining control signals
*****************************************************/

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/err.h"

#include "tcpIp.h"
#include "IOC.h"
#include "utils.h"
#include "tcpRequest.h"
#include "tcpIp.h"
#include "security.h"
#include "tcpMessage.pb-c.h"
#include "sec1.pb-c.h"

#define RESPONSE_BUFFER_SIZE 100


int8_t manage_incomming_message(const int sock, uint8_t *inputBuffer, uint8_t dataLength);
void send_message(const int sock, TCPMESSAGE__Message *responseMessage);


//recieved message.
TCPMESSAGE__Message *requestMessage = NULL;
//response message.
TCPMESSAGE__Message responseMessage = TCP__MESSAGE__MESSAGE__INIT;

uint8_t responseBuffer[RESPONSE_BUFFER_SIZE];


int8_t manage_incomming_message(const int sock, uint8_t *inputBuffer, uint8_t dataLength){

    // unpack recieved message, check for null.
    if((requestMessage = tcp__message__message__unpack(NULL, dataLength, inputBuffer)) == NULL) 
    {
        return -1;
    }

    switch(requestMessage->type)
    {
        case TCP__MESSAGE__PAYLOAD_TYPE__Alive_Request:

            responseMessage.aliveresponse = true;
            responseMessage.type = TCP__MESSAGE__PAYLOAD_TYPE__Alive_Response;
            send_message(sock, &responseMessage);
            break;

        case TCP__MESSAGE__PAYLOAD_TYPE__Relay_Command_Request:
            break;
            
        case TCP__MESSAGE__PAYLOAD_TYPE__Relay_Status_Request:
            break;

        case TCP__MESSAGE__PAYLOAD_TYPE__Security_Session_Request:

            printf("ok\n");
            responseMessage.type = TCP__MESSAGE__PAYLOAD_TYPE__Security_Session_Response;
            if(perform_handshake(sock, requestMessage, &responseMessage) == -1)
            {
                printf("it is -1\n");
            }

            break;

        default:
            break;
    }

    tcp__message__message__free_unpacked(requestMessage, NULL);
    requestMessage = NULL; 
    return 1;
}


void send_message(const int sock, TCPMESSAGE__Message *responseMessage)
{
    uint8_t packedSize = protobuf_c_message_get_packed_size(responseMessage);
    tcp__message__message__pack(responseMessage, responseBuffer);
    tcp_send(sock, responseBuffer, packedSize);
}