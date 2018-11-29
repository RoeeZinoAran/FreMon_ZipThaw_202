
#include "stm32l0xx_hal.h"

#define LM75B_I2C_7BITS_ADD   0x48

void get_board_temperature(uint8_t *str);
signed int interpret_pcb_temperature(void);
