#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#include "tcpIp.h"
#include "TcpRequest.h"

#define PORT 55555


static const char *TAG_tcp = "tcpIp";

char addr_str[128];
int ip_protocol = 0;
struct sockaddr_storage dest_addr;
int listen_sock;
int len;
uint8_t rx_buffer[128];


void tcp_send(int sock, uint8_t *dataToSend, uint8_t length)
{
    int to_write = length; 
    while (to_write > 0) {
        int written = send(sock, dataToSend + (length - to_write), to_write, 0);
            if (written < 0) {
            ESP_LOGE("from request", "Error occurred during sending: errno %d", errno);
            break;
        }
        to_write -= written;
    }
}

static void start_receiving(const int sock)
{
    do
    {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);

        if (len < 0) {

            ESP_LOGE(TAG_tcp, "Error occurred during receiving: errno %d", errno);
        } 

        else if (len == 0) {
            ESP_LOGW(TAG_tcp, "Connection closed");
        } 

        else {

            //rx_buffer[len] = 0; 
            //ESP_LOGI(TAG_tcp, "Received %d bytes: %s", len, rx_buffer);

            manage_incomming_message(sock, rx_buffer, len);
        }

        //int to_write = len;
        //while (to_write > 0) {
        //    int written = send(sock, temp + (len - to_write), to_write, 0);
        //    if (written < 0) {
        //        ESP_LOGE(TAG_tcp, "Error occurred during sending: errno %d", errno);
        //        // Failed to retransmit, giving up
        //        return;
        //    }
        //    to_write -= written;
        //}
    }
    while (len>0);
    
}


static void handle_connection(void *arg)
{
    int sock = (int)arg;
    start_receiving(sock);
    ESP_LOGI(TAG_tcp, "shutting down connection");
    shutdown(sock, 0);
    close(sock);
    vTaskDelete(NULL);
}


static void run_tcp_server(void *pvParameters)
{
    int addr_family = (int)pvParameters;

    if (addr_family == AF_INET) {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    }


    listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG_tcp, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ESP_LOGI(TAG_tcp, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG_tcp, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(TAG_tcp, "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    ESP_LOGI(TAG_tcp, "Socket bound, port %d", PORT);

    err = listen(listen_sock, 5);
    if (err != 0) {
        ESP_LOGE(TAG_tcp, "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }

    while (1) {

        ESP_LOGI(TAG_tcp, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG_tcp, "Unable to accept connection: errno %d", errno);
            break;
        }


        if (source_addr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }


        ESP_LOGI(TAG_tcp, "Socket accepted ip address: %s", addr_str);

        xTaskCreate(handle_connection, "handle_connection", 4096, (void *)sock, 5, NULL);

        // start_receiving(sock);

        // ESP_LOGI(TAG_tcp, "shutting down connection");

        // shutdown(sock, 0);
        // close(sock);
    }

CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}




void start_tcp_server()
{
    xTaskCreate(run_tcp_server, "tcp_server", 4096,(void*)AF_INET, 5, NULL);
}