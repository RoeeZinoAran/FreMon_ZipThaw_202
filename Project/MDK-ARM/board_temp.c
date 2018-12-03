
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "board_temp.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: lm75b_returned_string
  Variable Type: uint8_t [2]
  Extern module declaration: N/A. 
  Unit: [byte]
  Default value: N/A
  Description: 2 byte return value from LM75B IR device.
*/
/*--------------------------------------------------------------------------------------------------*/
static uint8_t lm75b_returned_string[2];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: hi2c2
  Variable Type: I2C_HandleTypeDef
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: I2C System interface handler.
*/
/*--------------------------------------------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c2;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: pcb_temperature_callback
  Variable Type: unsigned short
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: PCB tempruture callback.
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short pcb_temperature_callback;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall activating the LM75B device to measure PCB temperature. An interrupt is 
 	created when results are ready.
\return Void
\DerivedDesc N/A.
\param uint8_t* str - Not used. 
*/
/*--------------------------------------------------------------------------------------------------*/
void get_board_temperature(uint8_t* str)
{
	uint8_t str2i2c[3] = {0, 0, 0};
	
	pcb_temperature_callback = 0;
	
	HAL_I2C_Master_Transmit(&hi2c2, LM75B_I2C_7BITS_ADD << 1, str2i2c, 1, 10);
	HAL_I2C_Master_Receive_IT(&hi2c2, LM75B_I2C_7BITS_ADD << 1, lm75b_returned_string, 2);

#if 0
	s1 = ((unsigned short)str2i2c[0]) << 8;
	s1 |= (unsigned short)str2i2c[1];
	
	return(s1);
#endif

}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall translating the bytes returned by the LM75B (PCB temperature measurement)
 	device to temperature in mC units.
\return signed int: PCB tempature return value.
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
signed int interpret_pcb_temperature(void)
{
	unsigned short 	s1;
	signed short 	si1;
  	signed int   	result;
	
	s1 = ((unsigned short)lm75b_returned_string[0]) << 8;
	s1 |= (unsigned short)lm75b_returned_string[1];
	s1 >>= 5;
	if (s1 & (1 << 10)) // if sign bit (bit 10)
	{
		s1 |= 0xf800; // setting (sign extend) the uppper shifted 5 bits
	}
	si1 = s1; // @@@note sign issue
	result = si1 * 125; // 125 because device resolution is 1/8 deg / LSBit and we return in mC
	
	return(result);
}
