
/**
 *  Modbus Function Declarations
 */


/**
 * @buf[] data array whose CRC is to be calculated
 * @return - 16 bits (2-bytes) calculated CRC
 */




//_______________________________Structure Definitions______________________________________-


typedef union  {

	char modbus_float_charArray[4];
	float modbus_parsed_Float;
	int modbus_integer_Float;

}ModbusFloat;


//_________Function Declaration---------------------------

uint16_t ModRTU_CRC(uint8_t buf[],uint8_t size);


void Master_ModbusGetRequest(uint8_t deviceAddressP, 
			  				 uint8_t functionCodeP, uint8_t offsetLP ,uint8_t offsetHP, 
			  				 uint8_t readLengthLP, uint8_t readLengthHP );


void Modbus_master_Request_RS485(uint8_t deviceAddressP, 
			  				 uint8_t functionCodeP, uint8_t offsetLP ,uint8_t offsetHP, 
			  				 uint8_t readLengthLP, uint8_t readLengthHP);

float modbusFloat_Parser(uint8_t dataArray[]);

void RS485_TxEnable(void);

void RS485_RxEnable(void);


