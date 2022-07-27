import serial 
import time 
from ctypes.wintypes import BYTE

TERMINATOR = bytes([0x0D, 0x0A])  
SPLIT      = bytes([0x0A])     
polynomial = 0x07
CRC_CHECK    = "CRC_ERROR\r\n"
STRING_CHECK = "STRING_END_ERROR\r\n";


# Below Function will calculate 8 Bit CRC for incoming and outgoing datastrems
def calculateCRC8(data, length, Rx, prevCrc, crcCheck):
    crc = crcCheck
    for i in range(0, length):
        if Rx == True :
            convertedvalue = bytearray(data)
        else :
            convertedvalue = bytearray(data[i].encode())
        size = len(convertedvalue)
        j = 0
        while j < size:
            if Rx == False :
                if size != 1:
                    if i == length-1:
                        if j == size-1:
                            if  convertedvalue[j-2] != 0x0D :
                                if  convertedvalue[j-1] != 0x0A:
                                    if  convertedvalue[j] != 0x00:
                                        convertedvalue.append(TERMINATOR[0])
                                        convertedvalue.append(TERMINATOR[1])
                                        convertedvalue.append(0x00)
                                        size = size+3
                elif size == 1:
                        convertedvalue.append(TERMINATOR[0])
                        convertedvalue.append(TERMINATOR[1])
                        convertedvalue.append(0x00)
                        size = size+3                    
            else :
                if prevCrc!=0:
                    if i == length-1:
                        if j == size-1:
                            if  convertedvalue[j-1] == 0x0D :
                                if  convertedvalue[j] == 0x0A:
                                    convertedvalue.append(prevCrc)
                                    size = size+1
            bytevalue = (convertedvalue[j])
            count = 0
            sumvalue = 0
            while count < 8 :
                sumvalue = (crc ^ bytevalue) & 0x01;
                crc >>= 1;
                if (sumvalue):
                    crc ^= 0x07;
                bytevalue >>= 1;
                count = count+1
            j = j+1
    return crc 


# Below Function will Send Data to the Board
def SendDataToBoard(lines, crc): 
    length = 0
    for i in range(0, len(lines)):
        convertedvalue = bytearray(lines[i].encode())
        size = len(convertedvalue)
        length = length+size
        j = 0
        while j<size :
            if size != 1:
                if i == len(lines) -1:
                    if j == size - 1:
                        if  convertedvalue[j-1] != 0x0D :
                            if  convertedvalue[j] != 0x0A:
                                convertedvalue.append(TERMINATOR[0])
                                convertedvalue.append(TERMINATOR[1])
                                size = size+2
                elif size == 1:
                    if i == len(lines) -1:
                        if j == size - 1:
                            convertedvalue.append(TERMINATOR[0])
                            convertedvalue.append(TERMINATOR[1])
                            size = size+2   
            uart.write(BYTE(convertedvalue[j]))
            j = j+1
    uart.write(BYTE(Sendcrc))
    return length+1


               
while (True): 
    Sendcrc = 0
    firstName = input('Enter "START" for sending text to the board\n')
    if firstName == 'START':
        with serial.Serial('COM6',2400) as uart:
            if uart.is_open== False:
                uart.open() 
            with open('TextToSend.txt') as file:
                lines = file.readlines()
                Sendcrc = calculateCRC8(lines, len(lines),False,0,Sendcrc) 
            count = 0;
            prevtime = time.time()
            length = 0
            print("Sending TexToSend.txt to the Board through UART\n")
            length = SendDataToBoard(lines,Sendcrc)
            CurrentTime = time.time()
            DataPacketTime = CurrentTime  - prevtime
            byterate = length/DataPacketTime;
            print("PC Send bytes/Seconds     = ", byterate)
            print("PC Send Detected Baudrate = ", byterate*8)
            print("\n")
            prevtime = time.time()
            length = 0
            crc = 0
            ReciveCRC = 0 
            while(uart.is_open):
                data=uart.readline()
                checkMessage = data.decode()
                if checkMessage == CRC_CHECK:
                    print("Board Requesting to send the TexToSend.txt Again due to CRC Mismatch")
                    print("\n")
                    uart.close() 
                elif checkMessage == STRING_CHECK:
                    print("Board Requesting to send the TexToSend.txt Again due to String end Missing")
                    print("\n")
                    uart.close() 
                else:
                    index = len(data);
                    if data[index-2]==13:
                        if data[index-1]==10:
                            ReciveCRC = calculateCRC8(data, 1,True,Sendcrc,ReciveCRC)
                        else:
                            ReciveCRC = calculateCRC8(data, 1,True,0,ReciveCRC)
                    else :
                            ReciveCRC = calculateCRC8(data, 1,True,Sendcrc,ReciveCRC)
                    length = length+len(data);
                    packet, terminator = data.split(SPLIT, 1) 
                    print(packet.decode()) 
                    if data[index-2]==13:
                        if data[index-1]==10:
                            CurrentTime = time.time()
                            DataPacketTime = CurrentTime  - prevtime
                            byterate = length/DataPacketTime;
                            if  ReciveCRC !=0:
                                print("Received Data with CRC mismatch\n") 
                            print("\n")
                            print("PC Recieve bytes/Seconds     = ", byterate)
                            print("PC Recieve Detected Baudrate = ", byterate*8)
                            print("\n")
                            uart.close()
