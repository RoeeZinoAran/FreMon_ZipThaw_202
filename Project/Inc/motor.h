
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_MOTOR_update_agitation_motor_speed(void);
void p_MOTOR_init_L6470(void);
void p_MOTOR_read_agitation_motor_register(uint8_t add, uint8_t rx_data[]);
void p_MOTOR_update_agitation_motor_register(uint8_t add, uint8_t d0, uint8_t d1, uint8_t d2);
