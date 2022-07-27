#include "Nymble_Sm.h"

/*
This is an application to test UART communication with PC For more details 
read the readme file.
*/
int main(void)
{
  NymbleStateMachineInit();
  for (;;)
  {
    NymbleStateMachine();
  }
}