
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_MISCEL_int_to_str(signed int din, char *str, unsigned short len, unsigned short after_dec);
uint8_t p_MISCEL_calculate_crc8 (uint8_t *msg_array, uint8_t msg_size);


