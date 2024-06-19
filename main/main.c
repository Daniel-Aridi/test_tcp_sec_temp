#include <stdio.h>

#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/stream_buffer.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "mdns.h"
#include "driver/gpio.h"
#include "protocomm.h"

#include "tcpip.h"
#include "wifi.h"
#include "IOC.h"
#include "cryptography.h"



//void proto_task(){
//
//
//    YourPackageName__PayloadMessage msg = YOUR_PACKAGE_NAME__PAYLOAD_MESSAGE__INIT;
//
//    msg.payload = "SZIA";
//    msg.type = YOUR_PACKAGE_NAME__PAYLOAD_MESSAGE__MESSAGE_TYPE__TYPE_CAPITAL;
//
//    unsigned serialized_size = protobuf_c_message_get_packed_size(&msg);
//    printf("Total size of the serialized message: %zu bytes\n", serialized_size);
//
//    uint8_t *serialized_data = malloc(serialized_size);
//
//    your_package_name__payload_message__pack(&msg, serialized_data);
//
//    YourPackageName__PayloadMessage *deserializedMessage = your_package_name__payload_message__unpack(NULL, serialized_size, serialized_data);
//
//    // Access the values in the deserialized message
//    printf("Payload: %s, Type: %d\n", deserializedMessage->payload, deserializedMessage->type);
//
//    // Free memory
//    free(serialized_data);
//    your_package_name__payload_message__free_unpacked(deserializedMessage, NULL);
//
//}


void init_connection(){

    init_NVS();
    esp_netif_init();
    setup_gpio();
    register_event_loops();
    connect_to_network();
    
    start_tcp_server();
    start_mdns_service();
}



uint8_t publicKey[33];
uint8_t privatekey[33];



void app_main(void)
{
    //generate_keypair(publicKey, privatekey);
    init_connection();
    
    printf("hello hello hello hello \n");
}
