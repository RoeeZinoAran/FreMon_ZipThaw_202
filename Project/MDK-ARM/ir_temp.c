
#include "stm32l0xx_hal.h"
#include "host_from_comm.h"
#include "ir_temp.h"

uint8_t crc88(uint8_t inCrc, uint8_t inData);
static uint8_t ir_returned_string[4]; // the 3 bytes returned by the MLX90614 IR  device

extern I2C_HandleTypeDef hi2c1;
extern volatile unsigned short ir_conversion_callback;

/* --------------------------------------------------------------------------
 Activating the MLX90614 IR  device to return object temperature. The device
 is activated and an interrupt is created when result is ready.
-------------------------------------------------------------------------- */
void get_ir_temperature(void)
{
	ir_conversion_callback = 0;
	
//HAL_I2C_Master_Transmit(&hi2c1, IR_I2C_7BITS_ADD << 1, str1i2c, 4, 10);
//HAL_I2C_Master_Receive(&hi2c1, IR_I2C_7BITS_ADD << 1, str1i2c, 3, 10);
	
	HAL_I2C_Mem_Read_IT(&hi2c1, IR_I2C_7BITS_ADD << 1, IR_TOBJ1_CMD, 1, ir_returned_string, 3);
}
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Translating the bytes returned by the MLX90614 IR  device to temperature
 in mC units.
-------------------------------------------------------------------------- */
signed int interpret_ir_temperature(void)
{
	unsigned short s1;
	  signed int   si1;
	  signed int   result;
	
	s1 = (((unsigned short)ir_returned_string[1]) << 8) | ((unsigned short)ir_returned_string[0]); // building one 16 bits value from two bytes returned by the sensor
	si1 = (signed int)s1; // @@@note sign issue
	if (si1 >= 0x27AD) {  // 0x27AD means -70 degrees
		si1 -= 0x27AD; // this makes -70 deg to be 0.
		result = (((int)si1) * 20) - 70000; // *20 because one LSBit is 1/50 (and not 1/1000) C.  70000 is 70C in mC units
	}
	else { // if too cold i.e. under -70C
		result = -70000; // - 99C in mC units
	}
	
	return(result);
}
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
uint8_t crc88(uint8_t inCrc, uint8_t inData)
{
	uint8_t i;
	uint8_t data;
	data = inCrc ^ inData;
	for ( i = 0; i < 8; i++ )	{
		if (( data & 0x80 ) != 0 ) {
			data <<= 1;
			data ^= 0x07;
		}
		else
			data <<= 1;
	}
	return data;
}
// --------------------------------------------------------------------------

