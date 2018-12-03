
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_BOARD_TEMP_LM75B_I2C_7BITS_ADD
  Unit: [N/A]
  Define Value: 0x48
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_BOARD_TEMP_LM75B_I2C_7BITS_ADD		(0x48)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_BOAD_TEMP_get_board_temperature(uint8_t *str);
signed int p_BOAD_TEMP_interpret_pcb_temperature(void);
