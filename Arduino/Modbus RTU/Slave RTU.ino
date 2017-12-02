/************************************************************************/
/* This code is only meant for Mode 1  (Read Coil)

IN this mode data is composed of 1 bit
but data is transmitted as a byte                                                                   */
/************************************************************************/

int my_address = 0x11;

uint8_t lowCRC  =  0;
uint8_t highCRC =  0;

uint16_t calCRC =0;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
  
		if (Serial.available())
		{
			unsigned char reader[8] = {0};
			
			unsigned char subreader[6] = {0};
			
			Serial.readBytes(reader,8);
			
			if (reader[0] == my_address)
			{
						
				for (int i = 0;i<6;i++)
				{
					subreader[i] = reader[i];
				}
				
				calCRC = ModRTU_CRC(subreader,6);
				
				highCRC = calCRC & 0x00FF;
				
				lowCRC  = (calCRC &  0xFF00)>>8;
				
				
				if ((highCRC == reader[6]) && (lowCRC == reader[7]) )
				{
					
					Serial.write(reader[0]);
					Serial.write(reader[1]);
					
					// before sending data we have to send size of data 
					Serial.write(0x05); // Since data is composed of 37bits (37 / 8 = 5 bytes approx)   
					
					// each byte contains 8 bits (coils) 
					Serial.write(0xCD); // Coils 27 - 20 (1100 1101)
					Serial.write(0x6B); // Coils 35 - 28 (0110 1011)
					Serial.write(0xB2); // Coils 43 - 36 (1011 0010)
					Serial.write(0x0E); // Coils 51 - 44 (0000 1110)
				
					Serial.write(0x1B); // 3 space holders & Coils 56 - 52 (0001 1011)
	
					Serial.write(0x45); // CRC High
					Serial.write(0xE6); // CRC Low 
					
					
				}
				
			}
	
			
		}
  
	  
	   
	   //Serial.print("Low CRC Byte : ");
	   //Serial.print(lowCRC,HEX);
	   //Serial.print("  & High CRC Byte : ");
	   //Serial.println(highCRC,HEX);
	     

}

uint16_t ModRTU_CRC(byte buf[], uint8_t len)
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
