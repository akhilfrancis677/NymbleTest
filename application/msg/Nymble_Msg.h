/*
 * msg_flags.h
 *
 */

#ifndef __NYMBLE_MSG_H_
#define __NYMBLE_MSG_H_

#include <stdbool.h>
#include <stdint.h>


typedef enum
{
  SM_FLAG_EEPROM_WRITE_FINISH              = (1u << 1),
  SM_FLAG_UART_CRC_RX_ERROR                = (1u << 2),
  SM_FLAG_UART_CRC_TX_ERROR                = (1u << 3),
  SM_FLAG_UART_STRING_END_ERROR            = (1u << 4),
 
}FSM_Flag_t;

typedef enum
{
    MSG_TYPE_UART,
    MSG_TYPE_MAX
}MSG_TYPE_t;


void MSG_SetFlag(MSG_TYPE_t type, uint32_t flag);

bool MSG_CheckAndClearFlag(MSG_TYPE_t type, uint32_t flag);

bool MSG_CheckFlag(MSG_TYPE_t type, uint32_t flag);

bool MSG_AreFlagsSet(MSG_TYPE_t type);

bool MSG_AreAllFlagsReset(void);

#endif /* __MSG_H_ */
