
#include "stm32l0xx_hal.h"
#include "discreets.h"

#include "extern_structs.h"

extern TIM_HandleTypeDef htim2;
extern TIM_OC_InitTypeDef PWM_timer2_OC_params;

volatile unsigned short motor_index;
volatile unsigned short door_state;
	
/* --------------------------------------------------------------------------
 Returning the communication address (with SOM).
-------------------------------------------------------------------------- */
unsigned short get_som_comm_address(void)
{
	unsigned short s1;
	
	s1  = HAL_GPIO_ReadPin (COMM_ADD2_GPIO_Port, COMM_ADD2_Pin) ? 1 << 2 : 0; // getting bit 2 of address
	s1 |= HAL_GPIO_ReadPin (COMM_ADD2_GPIO_Port, COMM_ADD1_Pin) ? 1 << 1 : 0; // getting bit 1 of address
	s1 |= HAL_GPIO_ReadPin (COMM_ADD2_GPIO_Port, COMM_ADD0_Pin) ? 1 << 0 : 0; // getting bit 0 of address
	
	return(s1);
}
// --------------------------------------------------------------------------


void get_motor_index(void)
{
	system_state.motor_index = HAL_GPIO_ReadPin (MOTOR_INDEX_GPIO_Port, MOTOR_INDEX_Pin); // 1 means in index
}

void get_door_state(void)
{
	system_state.door_state = HAL_GPIO_ReadPin (DOOR_STATE_GPIO_Port, DOOR_STATE_Pin);
}



void rgb_leds(unsigned short red, unsigned short green, unsigned short blue) // full strength is 10,000
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, red   ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, green ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, blue  ? GPIO_PIN_SET : GPIO_PIN_RESET);
	
	system_state.leds_state = (red ? (1 << 0) : 0) | (green ? (1 << 1) : 0) | (blue ? (1 << 2) : 0);
/*
	PWM_timer2_OC_params.Pulse = (uint32_t)red;
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_1); // RED

	PWM_timer2_OC_params.Pulse = (uint32_t)green;
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_2); // GREEN

	PWM_timer2_OC_params.Pulse = (uint32_t)blue;
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_3); // BLUE

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // RED
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // GREEN
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // BLUE
*/
}


//

