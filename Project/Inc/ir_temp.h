
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: IR_I2C_7BITS_ADD
  Unit: [bytes]
  Define Value: 0x5A
  Description:  Infra red 7 bits register address.
*/
/*--------------------------------------------------------------------------------------------------*/
#define IR_I2C_7BITS_ADD			(0x5A)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: IR_TOBJ1_CMD
  Unit: [bytes]
  Define Value: 0x07
  Description:  INTERNAL RAM ADDRESS FOR TOBJ1  i.e. Object temperature 1
*/
/*--------------------------------------------------------------------------------------------------*/
#define IR_TOBJ1_CMD				(0x07)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void get_ir_temperature(void);
signed int interpret_ir_temperature(void);
