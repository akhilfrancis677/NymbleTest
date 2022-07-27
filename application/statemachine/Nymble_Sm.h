/*
 * msg_flags.h
 *
 */

#ifndef __NYMBLE_SM_H_
#define __NYMBLE_SM_H_

#include <stdbool.h>
#include <stdint.h>

#include "Nymble_Eeprom.h"
#include "Nymble_Uart.h"
#include "Nymble_Power.h"
#include "Nymble_Crc8.h"
#include "Nymble_Msg.h"


void NymbleStateMachine(void);
void NymbleStateMachineInit (void);

#endif /* __MSG_H_ */
