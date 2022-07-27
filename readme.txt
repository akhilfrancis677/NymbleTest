This is an application to test UART b/w PC and the BOARD. I have used nrf52832, Arm M4 controller as my microcontroller. Firmware is written in a BareMetal code. 
Since I don’t have an EEPROM in Hand, I have simulated nrf52832 stack as EEPROM memory. 
EEPROM memory can be configured, using macros defined in config/application_config.h into application/eeprom/Nymble_Eeprom.h. 
Firmware is working as a state machine, there are state inputs to execute certain operations. If there is no state input board will be in power save mode. 
UART communication start from PC, PC must send a string with string end parameter, I have used \r\n as string end along with 8-bit CRC.   


Firmware Working.
*****************
1.	Power save mode 
2.	Receive an UART interrupt, read and store data into EEPROM memory. 
3.	Receive string end and CRC and store into EEPROM
4.	Read back all the EEPROM data and do CRC check. 
5.	If there is successful CRC verification, data will be sent back to PC. 
6.	If there is no successful CRC verification, board will be requested to PC to send data again. 
7.	After start to read the data in UART line, if board doesn’t receive a string end board will request to send data again, to check this there is 3 seconds cyclic task. 

PC Working 
*****************
1.	Execute python/NymbleTest.py  
2.	NymbleTest.py will read texts inside file python/TextToSend.txt 
3.	Calculate 8-bit CRC 
4.	Send complete string into board along with string end and 8-bit CRC.
5.	Print send bytes/second and send baud rate into console. 
6.	Wait for response from the board.
a.	If there is successful CRC verification at the PC RX side, data will be print into console. 
b.	If there is no successful CRC verification at the PC RX side, console will print 
	“Received Data with CRC mismatch” 
7.	Print receive bytes/second and receive baud rate into console. 
