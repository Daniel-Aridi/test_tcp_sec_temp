#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * shortcut for checking two arrays if equal.
 * 
 * [input]
 *  str1: first element.
 *  str2: second element.
 *  length: size of arrays of equal size.
 *  
 * [return]
 *  1: success
 *  0: fail 
*/
int compare(const uint8_t* str1, const uint8_t* str2, int length);

/**
 * fills an array with random bytes.
 * 
 * [input]
 * 
 * length: length of array.
 * 
 * [output]
 * 
 * array: array to be filled with random bytes.
*/
void generate_random_array(uint8_t *array, int length); 

/**
 * for debuging.
 * 
 * [input]
 * array: to be printed in consol.
 * length: size of array elements.
*/
void print_array(uint8_t *array, int length);

/**
 * xors to arrays.
 * 
 * [input]
 * element1
 * element2
 *  
 * [output]
 * out
*/
void calculate_XOR(uint8_t out[32], uint8_t element1[32], uint8_t element2[32]);


#endif