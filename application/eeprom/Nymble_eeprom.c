#include "stddef.h"
#include "Nymble_Eeprom.h"
#include "Nymble_Msg.h"
#include "nrf.h"
#include "string.h"


/*
nrf52832 stack itself simulating as an EEPROM memory.
UART RX data will be stored into these stack memory 
*/

static EeepromMemory_t EepromMemory;
static volatile int32_t Eeprom_DataSize = EMPTY;
static volatile bool RecievedEndOfString = false;
static volatile bool RecievedCrc8 = false;

/*
Return the last index of the array 
*/
int32_t EEPROM_GetDataLength(void)
{
  return Eeprom_DataSize;
}

/*
Reset the memory and index and other flags 
for next operation
*/
void EEPROM_Erase(void)
{
  Eeprom_DataSize = EMPTY;
  memset(&EepromMemory,0, sizeof(EepromMemory));
  RecievedEndOfString = false;
  RecievedCrc8 = false;
}

/*
Return true if EEPROM received the string end paramter else false
*/
bool EEPROM_WriteFinish(void)
{
  return RecievedCrc8;
}

/*
Return head of the array
*/
void    EEPROM_Head(EeepromMemory_t **ptr)
{
  *ptr = &EepromMemory;
  (void) (*ptr);
}


/*
Write uart Rx data into array
*/
void EEPROM_Write(int8_t  data)
{
  if (!EepromMemory.EeepromCells[Eeprom_DataSize].uart_status && !RecievedCrc8)
  {
    uint32_t uwPRIMASK_Bit = __get_PRIMASK();   /**< backup PRIMASK bit */
    __disable_irq();   
    EepromMemory.EeepromCells[Eeprom_DataSize].data = data;     
    (void) (EepromMemory);    
    if (RecievedEndOfString)
    {
      RecievedCrc8 = true;
      EepromMemory.EeepromCells[Eeprom_DataSize].uart_status = false;
      MSG_SetFlag(MSG_TYPE_UART, SM_FLAG_EEPROM_WRITE_FINISH);
    }
    else
    {
      EepromMemory.EeepromCells[Eeprom_DataSize].uart_status = true;
      
    }    
    if (EepromMemory.EeepromCells[Eeprom_DataSize-1].data==0x0D && EepromMemory.EeepromCells[Eeprom_DataSize].data==0x0A)
    {
      RecievedEndOfString = true;
    }
    Eeprom_DataSize++;    
    __set_PRIMASK(uwPRIMASK_Bit);               /**< Restore PRIMASK bit*/
  }
} 

/*
Read data from array
*/
void EEPROM_Read(EeepromSingleCell_t  *data, int32_t index)
{
  uint32_t uwPRIMASK_Bit = __get_PRIMASK();   /**< backup PRIMASK bit */
  __disable_irq();    
  data->uart_status = EepromMemory.EeepromCells[index].uart_status;
  data->data = EepromMemory.EeepromCells[index].data;     
  (void) (EepromMemory);
  __set_PRIMASK(uwPRIMASK_Bit);               /**< Restore PRIMASK bit*/
}
