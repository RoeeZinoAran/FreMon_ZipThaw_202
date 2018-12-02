
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "host_from_comm.h"
#include "adc.h"
#include "rfid.h"
#include "init.h"
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
extern TIM_HandleTypeDef htim3;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern TIM_OC_InitTypeDef PWM_timer3_OC_params;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short no_somm_message_timer;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern uint8_t som_rx_buffer[MAX_MESSAGE_LENGTH];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern uint8_t from_rfid_string[RFID_STRING_ARRAY_SIZE];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short uart_tx_callback[5];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short uart_rx_callback[5];

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */


/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall handle general BSP initialization.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void init(void)
{
	init_heating_pwm();
	init_receiving_from_som();
	init_rgb_leds();
	//	calib_adc(); // Read 'VREFINT_CAL' value from MCU - the value used to measure VDDA for calibrating the ADC full scale
	
	/* Emulating like RFID was read inorder to allow calling RFID again in the main loop */
	uart_rx_callback[RFID_COMM_UART_NUM] = 1; // 

	//	get_rfid_counter(from_rfid_string);
	
	system_state.tasks_in_progress = STANDBY_TASK_BIT;
	system_state.after_thawing_timer = 0;
	system_state.progress = 0xff; // not active
}
// --------------------------------------------------------------------------



/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall initializing Timer 3 for PWM for Heating elements
 	Period is defined in CUBE.
 	Pulse width defined here by HAL_TIM_PWM_ConfigChannel(...);
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void init_heating_pwm(void)
{
	PWM_timer3_OC_params.OCMode = TIM_OCMODE_PWM1;
	PWM_timer3_OC_params.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWM_timer3_OC_params.OCFastMode = TIM_OCFAST_DISABLE;

	PWM_timer3_OC_params.Pulse = 120; 
	HAL_TIM_PWM_ConfigChannel(&htim3, &PWM_timer3_OC_params, TIM_CHANNEL_1);

	PWM_timer3_OC_params.Pulse = 160;
	HAL_TIM_PWM_ConfigChannel(&htim3, &PWM_timer3_OC_params, TIM_CHANNEL_2);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall initializing Timer 2 for PWM for RGB LED
 	Period is defined in CUBE.
 	Pulse width defined here by  HAL_TIM_PWM_ConfigChannel(...)
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void init_rgb_leds(void)
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);

#if 0
	PWM_timer2_OC_params.OCMode = TIM_OCMODE_PWM1;
	PWM_timer2_OC_params.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWM_timer2_OC_params.OCFastMode = TIM_OCFAST_DISABLE;

	PWM_timer2_OC_params.Pulse = 2000; // period is 10,000
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_1); // RED

	PWM_timer2_OC_params.Pulse = 1000; // period is 10,000
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_2); // GREEN

	PWM_timer2_OC_params.Pulse = 500; // period is 10,000
	HAL_TIM_PWM_ConfigChannel(&htim2, &PWM_timer2_OC_params, TIM_CHANNEL_3); // BLUE

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // RED
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // GREEN
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // BLUE
#endif

}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall initializing receiving message from SOM.
 	Intended to be used in the following cases:
	1. After reset,
	2. Recovering from communication failure,
	3. Normally - after transmission to SOM ended and we prepare for receiving
   	new message from SOM.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void init_receiving_from_som(void)
{
	no_somm_message_timer = 0;
	uart_tx_callback[SOM_COMM_UART_NUM] = 0;
	uart_rx_callback[SOM_COMM_UART_NUM] = 0;
	HAL_UART_Receive_DMA(&huart2, som_rx_buffer, NUM_OF_HEADER_BYTES);
	system_state.som_comm_state = RECEIVING_MESSAGE_HEADER;
}
// --------------------------------------------------------------------------
