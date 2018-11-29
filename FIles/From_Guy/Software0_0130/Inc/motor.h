
#include "stm32l0xx_hal.h"


void update_agitation_motor_speed(void);
void init_L6470(void);
void read_agitation_motor_register(uint8_t add, uint8_t rx_data[]);
void update_agitation_motor_register(uint8_t add, uint8_t d0, uint8_t d1, uint8_t d2);
