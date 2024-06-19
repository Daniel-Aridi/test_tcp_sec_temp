#include <stdio.h>
#include <string.h>
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "tcpIp.h"
#include "tcpRequest.h"
#include "cryptography.h"
#include "utils.h"
#include "security.h"
#include "sec1.pb-c.h"


#define KEY_SIZE 32
#define INIT_VEC_SIZE 16


bool generate_keyPair(uint8_t *devicePublicKey, uint8_t *devicePrivateKey, int length);
void calculate_sha256(const uint8_t *input, uint8_t output[32], size_t input_len); void send_message(const int sock, TCPMESSAGE__Message *responseMessage);
void crypt_aes_ctr(uint8_t* key, uint8_t* nonce, const uint8_t* input, size_t inputLength, uint8_t* output);

const uint8_t pop[] = "1234abcd"; //temp!!!!!!!!!
uint8_t initVector[INIT_VEC_SIZE];
uint8_t pop_sha256[KEY_SIZE];  //!!!!!!!!!!!
uint8_t sharedKey[KEY_SIZE];




int8_t perform_handshake(const uint8_t sock, TCPMESSAGE__Message *requestMessage, TCPMESSAGE__Message *responseMessage)
{
    if(requestMessage->sec1 == NULL)
    {
        return -1;
    }

    switch(requestMessage->sec1->msg)
    {
        case SEC1_MSG_TYPE__Session_Command0:      

            if((requestMessage->sec1->sc0 == NULL) || (requestMessage->sec1->sc0->client_pubkey.len != KEY_SIZE))
            {   
                return -1;
            }

            uint8_t *devicePublicKey = malloc(KEY_SIZE); 
            uint8_t *devicePrivateKey = malloc(KEY_SIZE); 

            if((devicePublicKey == NULL) || (devicePrivateKey == NULL))
            {
                free(devicePrivateKey); free(devicePublicKey); 
                return -1;
            }

            //recieved client pulic key:
            uint8_t *clientPublicKey = requestMessage->sec1->sc0->client_pubkey.data;


            //generating public, private, IVector:
            generate_keyPair(devicePublicKey, devicePrivateKey, KEY_SIZE);
            x25519_generate_sharedKey(sharedKey, clientPublicKey, devicePrivateKey);
             
            size_t pop_size = strlen((char*)pop);
            calculate_sha256(pop, pop_sha256, pop_size);
            calculate_XOR(sharedKey, sharedKey, pop_sha256);
            generate_random_array(initVector, INIT_VEC_SIZE);


            //filling in response message data:
            SessionResp0 resp0 = SESSION_RESP0__INIT;
            resp0.device_pubkey.len = KEY_SIZE;
            resp0.device_pubkey.data = devicePublicKey;
            resp0.device_random.len = INIT_VEC_SIZE;
            resp0.device_random.data = initVector;

            Sec1Payload payload = SEC1_PAYLOAD__INIT;
            payload.msg = SEC1_MSG_TYPE__Session_Response0;
            payload.payload_case = SEC1_PAYLOAD__PAYLOAD_SR0;
            payload.sr0 = &resp0;

            responseMessage->sec1 = &payload;
            size_t responseMessage_Size = tcp__message__message__get_packed_size(responseMessage);
            uint8_t *responseMessageBuffer = malloc(responseMessage_Size);
            tcp__message__message__pack(responseMessage, responseMessageBuffer);
        
            tcp_send(sock, responseMessageBuffer, responseMessage_Size);

            printf("device pubulic key:\n");
            print_array(devicePublicKey, 32);
            printf("\n\nclient public key:\n");
            print_array(clientPublicKey, 32);
            printf("\n\n shared key: \n");
            print_array(sharedKey, 32);
            printf("\n\n sha: \n");
            print_array(pop_sha256, 32);
            printf("\n\n init vector:\n");
            print_array(initVector, 16);
            printf("\n\n");

            free(devicePublicKey); free(devicePrivateKey); free(responseMessageBuffer);
            break;

        case SEC1_MSG_TYPE__Session_Command1:

            printf("command1\n");

            if((requestMessage->sec1 == NULL) || (requestMessage->sec1->sc1->client_verify_data.len != 16))
            {
                return -1;
            }

            uint8_t *clientVerify = requestMessage->sec1->sc1->client_verify_data.data;
            uint8_t *compareBuf = malloc(16);
            if(compareBuf == NULL)
            {
                free(compareBuf);
                return -1;
            }

            crypt_aes_ctr(sharedKey, initVector, clientVerify, INIT_VEC_SIZE, compareBuf);            

            if(compare(clientVerify, compareBuf, INIT_VEC_SIZE))
            {
                printf("they are equal!");
            }
            else 
                printf("they are not equal");
            
            free(clientVerify); free(compareBuf);

            break;

        default:
            break;
    }
    return 0;
}



bool generate_keyPair(uint8_t *devicePublicKey, uint8_t *devicePrivateKey, int length)
{
    generate_random_array(devicePrivateKey, length);

    if(x25519_generate_publicKey(devicePublicKey, devicePrivateKey))
    {
        return true;
    }
    return false;
}

void calculate_sha256(const uint8_t *input, uint8_t output[32], size_t input_len) 
{
    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);
    mbedtls_sha256_starts(&ctx, 0);

    mbedtls_sha256_update(&ctx, input, input_len);
    mbedtls_sha256_finish(&ctx, output);

    mbedtls_sha256_free(&ctx);
}

void crypt_aes_ctr(uint8_t* key, uint8_t* nonce, const uint8_t* input,size_t inputLength, uint8_t* output)
{
    size_t ncoff = 0;
    uint8_t stream_block[16];

    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);

    mbedtls_aes_setkey_enc(&ctx, key, 256);

    mbedtls_aes_crypt_ctr(  &ctx,
                            inputLength,
                            &ncoff,
                            nonce,
                            stream_block,
                            input,
                            output);
    
    mbedtls_aes_free(&ctx);
}