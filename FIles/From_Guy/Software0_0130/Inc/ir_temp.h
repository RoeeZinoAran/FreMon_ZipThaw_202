#include "stm32l0xx_hal.h"


#define IR_I2C_7BITS_ADD			0x5A


// ----------------------------------------------------------
#define IR_TOBJ1_CMD          0x07
// INTERNAL RAM ADDRESS FOR TOBJ1  i.e. Object temperature 1
// ----------------------------------------------------------


void get_ir_temperature(void);
signed int interpret_ir_temperature(void);
