/* 
    file for managing hardware abstraction layer
    gpio levels for relay signaling
*/

#include <string.h>
#include "driver/gpio.h"
#include "IOC.h"

#define RELAY_PIN_1   27


void setup_gpio(){

    gpio_set_direction(RELAY_PIN_1, GPIO_MODE_OUTPUT);
}

void set_relay_state(int relayNumber, int state){

    if(relayNumber == 1 && (state == 0 || state == 1)){

        gpio_set_level(RELAY_PIN_1, state);
    }
    
}

