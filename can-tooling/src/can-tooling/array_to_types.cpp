#include "can-tooling/array_to_types.h"

uint32_t FourBytesMSB(unsigned char* array)
{
    uint32_t val = 0;
    val = array[0] << 24;
    val |= array[1] << 16;
    val |= array[2] << 8;
    val |= array[3];
    return val;
}

uint32_t ThreeBytesMSB(const unsigned char* array)
{
    uint32_t val = 0;
    val |= array[0] << 16;
    val |= array[1] << 8;
    val |= array[2];
    return val;
}

uint16_t TwoBytesMSB(const unsigned char* array)
{
    uint32_t val = 0;
    val |= array[0] << 8;
    val |= array[1];
    return val;
}

uint32_t FourBytesLSB(const unsigned char* array)
{
    uint32_t val = 0;
    val = array[0];
    val |= array[1] << 8;
    val |= array[2] << 16;
    val |= array[3] << 24;
    return val;
}

uint32_t ThreeBytesLSB(const unsigned char* array)
{
    uint32_t val = 0;
    val = array[0];
    val |= array[1] << 8;
    val |= array[2] << 16;
    return val;
}

uint16_t TwoBytesLSB(const unsigned char* array)
{
    uint32_t val = 0;
    val = array[0];
    val |= array[1] << 8;
    return val;
}



