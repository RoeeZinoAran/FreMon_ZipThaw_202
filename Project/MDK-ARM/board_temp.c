
#include "stm32l0xx_hal.h"
#include "board_temp.h"

static uint8_t lm75b_returned_string[2]; // the 2 bytes returned by the LM75B IR  device

extern I2C_HandleTypeDef hi2c2;
extern volatile unsigned short pcb_temperature_callback;

/* --------------------------------------------------------------------------
 Activating the LM75B device to measure PCB temperature. An interrupt is 
 created when results are ready.
-------------------------------------------------------------------------- */
void get_board_temperature(uint8_t *str)
{
	uint8_t str2i2c[3] = {0, 0, 0};
	
	pcb_temperature_callback = 0;
	
	HAL_I2C_Master_Transmit(&hi2c2, LM75B_I2C_7BITS_ADD << 1, str2i2c, 1, 10);
	HAL_I2C_Master_Receive_IT(&hi2c2, LM75B_I2C_7BITS_ADD << 1, lm75b_returned_string, 2);

/*	
	s1 = ((unsigned short)str2i2c[0]) << 8;
	s1 |= (unsigned short)str2i2c[1];
	
	return(s1);
*/
}
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Translating the bytes returned by the LM75B (PCB temperature measurement)
 device to temperature in mC units.
-------------------------------------------------------------------------- */
signed int interpret_pcb_temperature(void)
{
	unsigned short s1;
	  signed short si1;
	  signed int   result;
	
	s1 = ((unsigned short)lm75b_returned_string[0]) << 8;
	s1 |= (unsigned short)lm75b_returned_string[1];
	s1 >>= 5;
	if (s1 & (1 << 10)) // if sign bit (bit 10)
		s1 |= 0xf800; // setting (sign extend) the uppper shifted 5 bits
	si1 = s1; // @@@note sign issue
	result = si1 * 125; // 125 because device resolution is 1/8 deg / LSBit and we return in mC
	
	return(result);
}
// --------------------------------------------------------------------------


