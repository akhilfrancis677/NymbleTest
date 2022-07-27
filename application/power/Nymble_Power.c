#include "Nymble_Power.h"
#include "nrf_pwr_mgmt.h"
#include "app_uart.h"


void power_management_init(void)
{
  ret_code_t err_code;
  err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
}

void idle_state_handle(void)
{
  nrf_pwr_mgmt_run();
}