
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_IR_TEMP_7BITS_ADD
  Unit: [bytes]
  Define Value: 0x5A
  Description: Infra red 7 bits register address.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_IR_TEMP_7BITS_ADD			(0x5A)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_IR_TEMP_IR_TOBJ1_CMD
  Unit: [bytes]
  Define Value: 0x07
  Description: INTERNAL RAM ADDRESS FOR TOBJ1  i.e. Object temperature 1
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_IR_TEMP_IR_TOBJ1_CMD				(0x07)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_IR_TEMP_get_temperature(void);
signed int p_IR_TEMP_interpret_ir_temperature(void);
