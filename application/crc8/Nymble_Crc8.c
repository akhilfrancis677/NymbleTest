#include "Nymble_Crc8.h"
#include "stdlib.h"

/*
Compute 8 bit CRC
*/
bool CRC8_Compute ( EeepromMemory_t **ptr, int32_t length)
{
  char crc = 0x00;
  char data;
  char sum;
  for (int i = 0; i < length; i++) {
    data = (*ptr)->EeepromCells[i].data;
    for (char tempI = 8; tempI; tempI--) {
      sum = (crc ^ data) & 0x01;
      crc >>= 1;
      if (sum)
        crc ^= 0x07;
      data >>= 1;
    }
  }
 return !crc;
}
