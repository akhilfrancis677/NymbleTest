#include <stdint.h>
#include "nordic_common.h"
#include "app_uart.h"
#include "application_config.h"

#include "Nymble_Sm.h"

static void uart_event_handle(app_uart_evt_t * p_event)
{
  uint8_t data = 0;  
  switch (p_event->evt_type)
  {
  case APP_UART_DATA_READY:
    UNUSED_VARIABLE(app_uart_get(&data));
    EEPROM_Write(data);
    break;
    
  case APP_UART_COMMUNICATION_ERROR:
    if (p_event->data.error_communication == 1)
    {
      //Error Count
    }
    else
    {
      
    }
    break;
    
  case APP_UART_FIFO_ERROR:
    APP_ERROR_HANDLER(p_event->data.error_code);
    break;
    
  default:
    break;
  }
}

void uart_send(uint8_t  resp_msg)
{
  ret_code_t ret_val;
  do
  {
    ret_val = app_uart_put(resp_msg);
    if ((ret_val != NRF_SUCCESS) && (ret_val != NRF_ERROR_BUSY))
    {
      APP_ERROR_CHECK(ret_val);
    }
  } while (ret_val == NRF_ERROR_BUSY);
}



void uart_init(void)
{
  ret_code_t err_code;
  
  app_uart_comm_params_t const comm_params =
  {
    .rx_pin_no    = RX_PIN_NUMBER,
    .tx_pin_no    = TX_PIN_NUMBER,
    .rts_pin_no   = RTS_PIN_NUMBER,
    .cts_pin_no   = CTS_PIN_NUMBER,
    .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
    .use_parity   = false,
    .baud_rate    = UART_BAUD_RATE
  };
  
  APP_UART_FIFO_INIT(&comm_params,
                     UART_RX_BUF_SIZE,
                     UART_TX_BUF_SIZE,
                     uart_event_handle,
                     APP_IRQ_PRIORITY_LOWEST,
                     err_code);
  
  APP_ERROR_CHECK(err_code);
}