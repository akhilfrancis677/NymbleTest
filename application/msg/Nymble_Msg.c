/*
 * msg.c
 *
 */
#include "Nymble_Msg.h"
#include "nrf.h"


typedef struct
{
    volatile uint32_t flags[MSG_TYPE_MAX];
}MSG_State_t;


static MSG_State_t MSG_state;


void MSG_SetFlag(MSG_TYPE_t type, uint32_t flag)
{
    uint32_t uwPRIMASK_Bit = __get_PRIMASK();   /**< backup PRIMASK bit */
    __disable_irq();                            /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

    MSG_state.flags[type] |= (uint32_t)flag;

    __set_PRIMASK(uwPRIMASK_Bit);               /**< Restore PRIMASK bit*/
}


bool MSG_CheckAndClearFlag(MSG_TYPE_t type, uint32_t flag)
{
    bool ret = false;

    uint32_t uwPRIMASK_Bit = __get_PRIMASK();   /**< backup PRIMASK bit */
    __disable_irq();                            /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

    if(MSG_state.flags[type] & (uint32_t)flag)
    {
        MSG_state.flags[type] &= ~(uint32_t)flag;
        ret = true;
    }
    __set_PRIMASK(uwPRIMASK_Bit);               /**< Restore PRIMASK bit*/

    return ret;
}


bool MSG_CheckFlag(MSG_TYPE_t type, uint32_t flag)
{
    bool ret = false;

    uint32_t uwPRIMASK_Bit = __get_PRIMASK();   /**< backup PRIMASK bit */
    __disable_irq();                            /**< Disable all interrupts by setting PRIMASK bit on Cortex*/

    if(MSG_state.flags[type] & (uint32_t)flag)
    {
        ret = true;
    }
    __set_PRIMASK(uwPRIMASK_Bit);               /**< Restore PRIMASK bit*/

    return ret;
}

bool MSG_AreAllFlagsReset(void)
{
    uint16_t type = 0u;
    bool ret = true;

    for (type = 0u; type < (uint16_t)(MSG_TYPE_MAX); type++)
    {
        if(MSG_state.flags[type] != 0u)
        {
            ret = false;
        }
    }

    return ret;
}


bool MSG_AreFlagsSet(MSG_TYPE_t type)
{
    bool ret = false;

    if(MSG_state.flags[type] != 0u)
    {
        ret = true;
    }

    return ret;
}
