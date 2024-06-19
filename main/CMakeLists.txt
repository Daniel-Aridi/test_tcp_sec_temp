set(component_srcs  main.c 
                    ../proto-c/tcpMessage.pb-c.c)

idf_component_register(SRCS "security.c" "tcpRequest.c" "IOC.c" "main.c" "tcpIp.c" "utils.c" "wifi.c" "cryptography.c" "${component_srcs}"

                        REQUIRES  nvs_flash wifi_provisioning protocomm esp_https_server driver protobuf-c

                        INCLUDE_DIRS "." "../include" "../proto-c")



