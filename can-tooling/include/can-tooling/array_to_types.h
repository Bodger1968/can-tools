#ifndef ARRAY_TO_TYPES_H
#define ARRAY_TO_TYPES_H
#include <stdint.h>

uint32_t FourBytesMSB(unsigned char* array);
uint32_t ThreeBytesMSB(const unsigned char* array);
uint16_t TwoBytesMSB(const unsigned char* array);

uint32_t FourBytesLSB(const unsigned char* array);
uint32_t ThreeBytesLSB(const unsigned char* array);
uint16_t TwoBytesLSB(const unsigned char* array);

#endif // ARRAY_TO_TYPES_H
