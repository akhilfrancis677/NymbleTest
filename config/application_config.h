#ifndef __APPLICATION_CONFIG_H
#define __APPLICATION_CONFIG_H



/*UART Configs*/
#define RX_PIN_NUMBER           8
#define TX_PIN_NUMBER           6
#define CTS_PIN_NUMBER          7
#define RTS_PIN_NUMBER          5
#define HWFC                    true
#define UART_BAUD_RATE          UART_BAUDRATE_BAUDRATE_Baud2400 
#define UART_TX_BUF_SIZE        2048                                    /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE        2048                                    /**< UART RX buffer size. */


/*EEPROM Configs*/

#define __1KB_MEM 1024
#define __2KB_MEM 2048
#define __4KB_MEM 4096

#endif 