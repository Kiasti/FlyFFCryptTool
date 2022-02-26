#include "CRC16.h"

unsigned short cryptEngine::flyff::crc16(const unsigned char *data, unsigned int size)
{
    unsigned short out = 0;
    int bits_read = 0, bit_flag;
    constexpr unsigned short nPoly = 0x8005;

	if(data == nullptr)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;
        out <<= 1;
        out |= (*data >> bits_read) & 1; 

    	bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }
        if(bit_flag)
            out ^= nPoly;
    }

    int i;
    for (i = 0; i < 16; ++i) 
    {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= nPoly;
    }

	i = 0x8000;
    unsigned short crc = 0;
    for (int j = 0x0001; i != 0; i >>=1, j <<= 1) 
        if (i & out) 
            crc |= j;

    return crc;
}