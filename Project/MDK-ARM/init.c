
#include "stm32l0xx_hal.h"
#include "host_from_comm.h"
#include "adc.h"
#include "rfid.h"
#include "init.h"

#include "extern_structs.h"

extern TIM_HandleTypeDef htim3;
extern TIM_OC_InitTypeDef PWM_timer3_OC_params;

extern UART_HandleTypeDef huart2;

extern volatile unsigned short no_somm_message_timer;
extern uint8_t som_rx_buffer[MAX_MESSAGE_LENGTH];

extern uint8_t from_rfid_string[RFID_STRING_ARRAY_SIZE];

extern volatile unsigned short uart_tx_callback[5];
extern volatile unsigned short uart_rx_callback[5];

/* --------------------------------------------------------------------------
General initialization:
-------------------------------------------------------------------------- */
void init(void)
{
	init_heating_pwm();
	init_receiving_from_som();
	init_rgb_leds();
//	calib_adc(); // Read 'VREFINT_CAL' value from MCU - the value used to measure VDDA for calibrating the ADC full scale
	
	uart_rx_callback[RFID_COMM_UART_NUM] = 1; // Emulating like RFID was read inorder to allow calling RFID again in the main loop
//	get_rfid_counter(from_rfid_string);
	
	system_state.tasks_in_progress = STANDBY_TASK_BIT;
	system_state.after_thawing_timer = 0;
	system_state.progress = 0xff; // not active
}
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Initializing Timer 3 for PWM for Heating elements
 Period is defined in CUBE.
 Pulse width defined here by  HAL_TIM_PWM_ConfigChannel(...);
-------------------------------------------------------------------------- */
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
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Initializing Timer 2 for PWM for RGB LED
 Period is defined in CUBE.
 Pulse width defined here by  HAL_TIM_PWM_ConfigChannel(...);
-------------------------------------------------------------------------- */

void init_rgb_leds(void)
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);

/*
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
*/
}
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Initializing receiving message from SOM.
 Intended to be used in the following cases:
1. After reset,
2. Recovering from communication failure,
3. Normally - after transmission to SOM ended and we prepare for receiving
   new message from SOM.s
-------------------------------------------------------------------------- */
void init_receiving_from_som(void)
{
	no_somm_message_timer = 0;
	uart_tx_callback[SOM_COMM_UART_NUM] = 0;
	uart_rx_callback[SOM_COMM_UART_NUM] = 0;
	HAL_UART_Receive_DMA(&huart2, som_rx_buffer, NUM_OF_HEADER_BYTES);
	system_state.som_comm_state = RECEIVING_MESSAGE_HEADER;
}
// --------------------------------------------------------------------------


//
