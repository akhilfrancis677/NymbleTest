#ifndef __NYMBLE_EEPROM_H_
#define __NYMBLE_EEPROM_H_

#include <stdint.h>
#include <stdbool.h>
#include "application_config.h"

#define EMPTY 0


typedef __packed   struct EeepromSingleCell
{
  int8_t  data;
  bool uart_status;
}EeepromSingleCell_t;



typedef  __packed struct EeepromMemory
{
   EeepromSingleCell_t EeepromCells[__2KB_MEM];
}EeepromMemory_t;


void EEPROM_Write(int8_t  data);
void EEPROM_Read(EeepromSingleCell_t  *data, int32_t index);

bool    EEPROM_WriteFinish(void);
int32_t EEPROM_GetDataLength(void);
void    EEPROM_Erase(void);
void    EEPROM_Head(EeepromMemory_t **ptr);
#endif 