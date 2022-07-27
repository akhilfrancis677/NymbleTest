#ifndef __NYMBLE_CRC8_H_
#define __NYMBLE_CRC8_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "application_config.h"
#include "Nymble_Eeprom.h"

bool CRC8_Compute ( EeepromMemory_t **ptr, int32_t length);


#endif 