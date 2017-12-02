/**
 * in this Code Bytes of CRC are reversed 
 * So all other Bytes are also reversed in order to make everthing consistent 
 * So LSB starts form Left and MSB is at the Right end 
 */

uint8_t deviceAddress = 0x11;
uint8_t functionCode = 0x01;
uint8_t offsetH = 0x00;
uint8_t offsetL = 0x13;
uint8_t readLengthL = 0x00; 
uint8_t readLengthH = 0x25;  

uint8_t readcount = 10; 
uint16_t readByte =0;


void setup()
{

	Serial2.begin(9600);
	Serial.begin(9600);

}


void loop()
{
	//sendModbusQuery
	readModbusCoil_request(deviceAddress,functionCode,offsetH,offsetL,readLengthH,readLengthL);
	delay(500);

	if (Serial2.available())
	{
		uint8_t reader[readcount] ={0};

		Serial2.readBytes(reader,readcount);

		if((reader[0] == deviceAddress) && (reader[1] == functionCode)){

			Serial.print("Byte 1 is : ");  
			Serial.print(reader[3],HEX);

			Serial.print(" Byte 2 is : ");  
			Serial.print(reader[4],HEX);

			Serial.print(" Byte 3 is : ");
			Serial.print(reader[5],HEX);

			Serial.print(" Byte 4 is : ");
			Serial.print(reader[6],HEX);

			Serial.print(" Byte 5 is : ");
			Serial.println(reader[7],HEX);



		}

	}

}


void readModbusCoil_request(uint8_t deviceAddressP, uint8_t functionCodeP, uint8_t offsetHP ,uint8_t offsetP, uint8_t readLengthHP, uint8_t readLengthLP ){
	
	// just for calculating CRC 
	uint16_t calCRC =0;
	uint8_t CRCLow = 0;
	uint8_t CRCHigh =0;

  	uint8_t subreader [6]= {deviceAddressP,functionCodeP,offsetHP,offsetLP,readLengthHP,readLengthLP};

	// Following Code may look like (form masking prespective that calCRC & 0x00FF = Low byte and vice versa 
	// but Our CRC Function return Bytes in revese order So here we are Correcting that inconsistency 
	
	calCRC = ModRTU_CRC(subreader,6);

	CRCHigh = calCRC & 0x00FF;

	CRCLow = (calCRC &  0xFF00)>>8;

	Serial2.write(deviceAddressP);
	Serial2.write(functionCodeP);
	Serial2.write(offsetHP);
	Serial2.write(offsetLP);
	Serial2.write(readLengthHP);
	Serial2.write(readLengthLP);
	Serial2.write(CRCHigh);
	Serial2.write(CRCLow);
	
}


uint16_t ModRTU_CRC(uint8_t buf[], uint8_t len)
{
	uint16_t crc = 0xFFFF;
	
	
	for (uint16_t pos = 0; pos < len; pos++) {
		crc ^= (uint16_t)buf[pos];          // XOR byte into least significant byte of crc
		
		for (uint16_t i = 8; i != 0; i--) {    // Loop over each bit
			if ((crc & 0x0001) != 0) {      // If the LSB is set
				crc >>= 1;                    // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else                            // Else LSB is not set
			crc >>= 1;                    // Just shift right
		}
	}
	// Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
	return crc;
}
