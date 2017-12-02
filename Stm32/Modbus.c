

/**
 *  Modbus Implementation Function
 */

//_______________________________Includes__________________________________________

#include "Response_Constant.h"
#include "stm32f0xx_hal.h"
#include "Modbus.h"
#include <string.h>

//---------------------------------extern Variables----------------------------------

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

//--------------------------------Receive_buffer ---------------------------------------

#define rbufferSize 9

uint8_t rbuffer[rbufferSize];

//-----------------------------------------------------------------------------------

/**
 *  Resonse Strcture intial Values 
 */

ModbusFloat modbusFloat;


//-------------------------------------------------------------------------------------

/*
	Modbus CRC calculator 

	@buf[] = data whose CRC is to be calculated
	@len = length of buffer  
 */
uint16_t ModRTU_CRC(uint8_t buf[],uint8_t size)
{
	uint16_t crc = 0xFFFF;
	
	
	for (uint16_t pos = 0; pos < size; pos++) {
		crc ^= (uint16_t)buf[pos];          // XOR byte into least significant byte of crc
		
		for (uint16_t i = 8; i != 0; i--) {    // Loop over each bit
			if ((crc & 0x0001) != 0) {         // If the LSB is set
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


/**
 * @device address  = address of the slave device
 * @fuctionalCodeP  = functional Code (data register series to access)
 * @offsetHP        = Higher byte of Offset
 * @offsetLP        = lower byte of Offset 
 * @readLengtHP     = High byte of number explaning (no of register to read)
 * @readLengthLP    = Lower byte of number explaning (no of register to read)
 */	
void Master_ModbusGetRequest(uint8_t deviceAddressP, uint8_t functionCodeP,
							 uint8_t offsetHP ,uint8_t offsetLP, 
							 uint8_t readLengthHP, uint8_t readLengthLP ){
	
	// just for calculating CRC 
	uint8_t subreader [6]= {deviceAddressP,functionCodeP,offsetHP,offsetLP,readLengthHP,readLengthLP};
	
	uint16_t calCRC =0;
	uint8_t CRCLow = 0;
	uint8_t CRCHigh =0;
	
	calCRC = ModRTU_CRC(subreader,6);
	
	CRCHigh = calCRC & 0x00FF;
	
	CRCLow = (calCRC &  0xFF00)>>8;
	
	uint8_t packet_request_Array[8] = {0};
	
	/**
	 *  Total 8 bytes are sent by master to slave
	 */
	packet_request_Array[0] = deviceAddressP; // device Address  is  1-byte
	packet_request_Array[1] = functionCodeP;  // functional Code is  1-byte
	packet_request_Array[2] = offsetHP;       // device Address  is  2-bytes
	packet_request_Array[3] = offsetLP;
	packet_request_Array[4] = readLengthHP;   // No of bytes to read is 2-bytes
	packet_request_Array[5] = readLengthLP;
	packet_request_Array[6] = CRCHigh;         // CRC is 2-bytes
	packet_request_Array[7] = CRCLow;
		
	// Normal Transmit (not interrupt or DMA driven)
	HAL_UART_Transmit(&huart1,packet_request_Array, 8, 500);

}


/**
 * @All the parameter are same as that of Master_ModbusGetRequest function written above
 * @return - Mbus Response structure for debugging 
 */
void Modbus_master_Request_RS485(uint8_t deviceAddressP, 
			  				 uint8_t functionCodeP, uint8_t offsetHP ,uint8_t offsetLP, 
			  				 uint8_t readLengthHP, uint8_t readLengthLP){

		RS485_TxEnable();
		
		Master_ModbusGetRequest(deviceAddressP,functionCodeP,offsetHP ,offsetLP, 
							 readLengthHP,readLengthLP );

			RS485_RxEnable();
			
			if( HAL_UART_Receive(&huart1,rbuffer,9,500) == HAL_OK){
				
					//HAL_UART_Transmit(&huart2,rbuffer,9,400); // For looiking the actual packet response 
				
				  float sendValue = modbusFloat_Parser(rbuffer);
				
				  uint8_t sendBuffer [20] = {0};
					
					sprintf(sendBuffer," frequency is %f /n" ,sendValue);
					
					HAL_UART_Transmit(&huart2,sendBuffer,20,400);
					
					
			}


}// Modbus_master_Request_RS485 end 



void RS485_TxEnable(){

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

}


void RS485_RxEnable(){

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

}


float modbusFloat_Parser(uint8_t dataArray[]){

    char tempArray[4] = {0};

      for(int i = 0 ; i< 4 ; i++){

        tempArray[i] = dataArray[i + 3];
    }


    uint32_t cb = 0;

    cb = (tempArray[0]<<24) |(tempArray[1]<<16)| (tempArray[2]<<8)| tempArray[3];

    //cout<<"0x"<<hex << cb <<endl;

    //cout << "0x" <<hex<<cb <<endl;


   	modbusFloat.modbus_integer_Float = cb;

    return modbusFloat.modbus_parsed_Float;

}
