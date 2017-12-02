Modbus
===================

Implementation of Modbus protocol using Arduino over RS485 [Full-Duplex]  and Ethernet (TCP/IP) and and Stm32 over RS485 (Half duplex with HAL driver) .

------------------------------------

Modbus master 
-------------

> **Master request Packet Info :**
> 
> - It contains 8 bytes out of which last 2-bytes are CRC bytes.
> - Modbus uses Big Indian format [MSB (most significant Byte not bit) is sent first] .
> > **Master request Packet :** Slave_address  | Function_Code | offset_High_Byte | offset_Low_Byte | readLength_High | readLength_Low | CRC_High | CRC_Low

----------

Modbus Slave 
-------------

> **Slave response Packet Info :**
> 
> Size of Response of is not fixed because master can request Bytes ranging from [1-255]

> > **Slave request Packet :** Slave_address  | Function_Code | No_of_ data_ bytes [not no of registers] after Slave_address byte and Function Code byte except CRC bytes|Data byte 1 | Data Byte 2 |...... | CRC_High | CRC_Low

--------------
Read this to get more info : http://www.simplymodbus.ca/FC03.htm

To read and Parse Float / long (4-Bytes) 
-------------------

* Maximum size of registers in Modbus is 16-bits i.e. 2-Bytes long. It means to read a value which is split into two registers you must read two register and then combine the value.

* To read Float or Long integers which are 4-Bytes wide values. Master must pass a request to read two register (16 bit wide each) and then combine the result of into single 32-bit register.

> **Converting Hex to Float ->**

> - The preffered way of doining this is to make union which contains int ,char array[4] and float as members (assuming int and float are 4-bytes long [size of float and int are 4-bytes in 32 bit architecture (both PC and ARM controllers)] ). Now Copy the contents of received bytes into char array of union in little endian (Modbus slave sends the data in Big endian). Then make a variable float f and then equate it to the union float member . This will automatically give you the correct float value.

> - Alternatively- Combine the received data bytes temp into int32_t variable and Now store this int32_t combined variable into int32_t member of union . Then make a variable float f and then equate it to the union float member . This will automatically give you the correct float value.

________

Modbus Exception
-------------

They are generally sent by receiver when you try to access particular register which is not valid. 

 > **Exception Packet :** Slave_address  | Function_Code |Exception Code | CRC_High | CRC_Low

Read this to get more info about Exception : http://www.simplymodbus.ca/exceptions.htm

Useful links

https://gregstoll.dyndns.org/~gregstoll/floattohex/ 
https://www.lammertbies.nl/comm/info/crc-calculation.html 
http://projectsfromtech.blogspot.in/2013/09/combine-2-bytes-into-int-on-arduino.html 
https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func
