#include "Nymble_Sm.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"

#pragma diag_suppress=Pm023
APP_TIMER_DEF(CyclicCheckTimer);
#pragma  diag_default=Pm023

#define CYCLIC_CHECK_TIME_MS   (3000)

const uint8_t *crc_error = "CRC_ERROR\r\n";
const uint8_t *string_end_error = "STRING_END_ERROR\r\n";

volatile int32_t prevIndex = 0;
volatile int32_t CurrentIndex = 0;

static void TimerCallBack(void* pContext);


/*
Intialize State machine 
*/
void NymbleStateMachineInit (void)
{
  ret_code_t err_code;
  
  /*Clock Initialization*/
  nrf_drv_clock_lfclk_request(NULL);
  err_code = nrf_drv_clock_init();
  err_code = app_timer_init();
  
  /*UART Initialization*/
  uart_init();  
  power_management_init();
  APP_ERROR_CHECK(err_code);
  APP_ERROR_CHECK(app_timer_create(&CyclicCheckTimer, APP_TIMER_MODE_REPEATED, &TimerCallBack));
  /*2 Seconds timer interrupt Start*/
  err_code = app_timer_start(CyclicCheckTimer, APP_TIMER_TICKS(CYCLIC_CHECK_TIME_MS), NULL);
}

/*
Run State machine 
*/
void NymbleStateMachine(void)
{
  if(MSG_AreFlagsSet(MSG_TYPE_UART))
  {
    /*
    Checking does Board Written a complete string into EEPROM
    */
    if(MSG_CheckAndClearFlag(MSG_TYPE_UART, SM_FLAG_EEPROM_WRITE_FINISH))
    {
      int32_t count = 0, EepromDataSize=0;
      EeepromMemory_t *MemorySection[1];        
      EepromDataSize= EEPROM_GetDataLength();           /*Get the total length of the EEPROM data*/
      EEPROM_Head(MemorySection);                       /*Get the head of the EEPROM data*/
      if (CRC8_Compute(MemorySection,EepromDataSize))   /*compute crc of the complete received data*/
      {
        while(count<=EepromDataSize)                    /*Iterate till last data if CRC check passed*/
        {
          EeepromSingleCell_t __EeepromSingleCell;
          EEPROM_Read(&__EeepromSingleCell,count);
          if (MemorySection[0]->EeepromCells[count].uart_status)
          {
            uart_send(MemorySection[0]->EeepromCells[count].data);        /*Send data through UART*/
            MemorySection[0]->EeepromCells[count].uart_status = false;
          }
          count++;
        }
      }
      else
      {
        MSG_SetFlag(MSG_TYPE_UART, SM_FLAG_UART_CRC_RX_ERROR);
      }
      EEPROM_Erase();
      prevIndex = 0;
      CurrentIndex = 0;
    }
    
    /*
    Checking does Board receieved a string with CRC mismatch
    */    
    if(MSG_CheckAndClearFlag(MSG_TYPE_UART, SM_FLAG_UART_CRC_RX_ERROR))
    {
      for (uint8_t i =0 ; i < 11; i++)
      {
        EEPROM_Erase();
        prevIndex = 0;
        CurrentIndex = 0;
        uart_send(crc_error[i]); 
      }
    }
    
    /*
    Checking does Board receieved a string without string end parameter (string end is appended from the PC, which is \r\n)
    */      
    if(MSG_CheckAndClearFlag(MSG_TYPE_UART, SM_FLAG_UART_STRING_END_ERROR))
    {
      for (uint8_t i =0 ; i < 18; i++)
      {
        EEPROM_Erase();
        prevIndex = 0;
        CurrentIndex = 0;
        uart_send(string_end_error[i]); 
      }
    }
  }
  
  else
  {
    idle_state_handle(); /*Power down Mode*/
    
  }
}

/*
Timer callback to check does  Board missed  string end 
*/
static void TimerCallBack(void* pContext)
{
  if ((CurrentIndex = EEPROM_GetDataLength()))
  {
#pragma diag_suppress=Pa082
    if (CurrentIndex == prevIndex)
    {
      MSG_SetFlag(MSG_TYPE_UART, SM_FLAG_UART_STRING_END_ERROR);
    }
#pragma diag_suppress=Pa082
    prevIndex = CurrentIndex;
  }
}