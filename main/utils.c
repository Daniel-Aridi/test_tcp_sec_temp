
#include "utils.h"
#include "esp_system.h"
#include "esp_random.h"


int compare(const uint8_t* str1, const uint8_t* str2, int length) {
    
    for (int i = 0; i < length; ++i) {
        if (str1[i] != str2[i]) {
            return 0;  // Strings are not equal
        }
    }
    return 1;  // Strings are equal
}

void generate_random_array(uint8_t *array, int length){

    for(int i = 0; i < length; i++){

        array[i] = esp_random() & 0xff;
    }
}

void print_array(uint8_t *array, int length){

    for(int i = 0; i < length; i++){
        printf("%02X ", array[i]);
    }
    printf("\n");
}

void calculate_XOR(uint8_t out[32], uint8_t element1[32], uint8_t element2[32])
{
    for(uint8_t i = 0; i < 32; i++)
    {
        out[i] = element1[i] ^ element2[i];
    }
}