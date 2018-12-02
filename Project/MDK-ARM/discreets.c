
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "discreets.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern TIM_OC_InitTypeDef PWM_timer2_OC_params;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile unsigned short motor_index;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile unsigned short door_state;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall returning the communication address (with SOM).	
\return unsigned short: SOM address.
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
unsigned short get_som_comm_address(void)
{
	unsigned short s1;
	
	s1  = HAL_GPIO_ReadPin (COMM_ADD2_GPIO_Port, COMM_ADD2_Pin) ? 1 << 2 : 0; /* getting bit 2 of address */
	s1 |= HAL_GPIO_ReadPin (COMM_ADD2_GPIO_Port, COMM_ADD1_Pin) ? 1 << 1 : 0; /* getting bit 1 of address */
	s1 |= HAL_GPIO_ReadPin (COMM_ADD2_GPIO_Port, COMM_ADD0_Pin) ? 1 << 0 : 0; /* getting bit 0 of address */
	
	return(s1);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall get motor index.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void get_motor_index(void)
{
	system_state.motor_index = HAL_GPIO_ReadPin (MOTOR_INDEX_GPIO_Port, MOTOR_INDEX_Pin); /* 1 means in index */
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall get chamber door status.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void get_door_state(void)
{
	system_state.door_state = HAL_GPIO_ReadPin (DOOR_STATE_GPIO_Port, DOOR_STATE_Pin);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall turn On / Off RGB LEDs.
\return Void
\DerivedDesc N/A.
\param unsigned short red - RED LED operation is requested.
	   Range: Full range.
	   unsigned short green - Green LED operation is requested.
	   Range: Full range.
	   unsigned short blue - Blue LED operation is requested.
	   Range: Full range.
*/
/*--------------------------------------------------------------------------------------------------*/
void rgb_leds(unsigned short red, unsigned short green, unsigned short blue) 
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, red   ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, green ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, blue  ? GPIO_PIN_SET : GPIO_PIN_RESET);
	
	system_state.leds_state = (red ? (1 << 0) : 0) | (green ? (1 << 1) : 0) | (blue ? (1 << 2) : 0);

#if 0
	PWM_timer2_OC_params.Pulse = (uint32_t)red;
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_1); // RED

	PWM_timer2_OC_params.Pulse = (uint32_t)green;
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_2); // GREEN

	PWM_timer2_OC_params.Pulse = (uint32_t)blue;
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_3); // BLUE

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // RED
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // GREEN
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // BLUE
#endif

}
