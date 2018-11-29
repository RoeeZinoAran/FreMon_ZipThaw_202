
#include "heater.h"
#include "discreets.h"

#include "extern_structs.h"

extern TIM_HandleTypeDef htim3;
extern TIM_OC_InitTypeDef PWM_timer3_OC_params;

extern unsigned short safety_error;

/* --------------------------------------------------------------------------
 Updating heater PWM for both cushins according to data in system_state structure.
-------------------------------------------------------------------------- */
void update_heaters_power(void)
{
	// If door is open then shut PWM off (0% duty cycle):
	// -------------------------------------------------
	if ((system_state.door_state == DOOR_OPEN) || (safety_error >= 1000)) {
		system_state.cushion_duty_cycle[CUSHION0] = 0;
		system_state.cushion_duty_cycle[CUSHION1] = 0;
		HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_RESET);
	}
	
	else {
		HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_SET);
		
//  PWM_timer3_OC_params.Pulse = PWM_UNLIMITED_MAX_VALUE - system_state.cushion_duty_cycle[CUSHION0];
		PWM_timer3_OC_params.Pulse = system_state.cushion_duty_cycle[CUSHION0];
		if (PWM_timer3_OC_params.Pulse >= 10000) {
			PWM_timer3_OC_params.Pulse = 9999;
			HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_RESET);
		}
		HAL_TIM_PWM_ConfigChannel(&htim3, &PWM_timer3_OC_params, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

//		PWM_timer3_OC_params.Pulse = PWM_UNLIMITED_MAX_VALUE - system_state.cushion_duty_cycle[CUSHION1];
		PWM_timer3_OC_params.Pulse = system_state.cushion_duty_cycle[CUSHION1];
		if (PWM_timer3_OC_params.Pulse >= 10000) {
			PWM_timer3_OC_params.Pulse = 9999;
			HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_RESET);
		}
		HAL_TIM_PWM_ConfigChannel(&htim3, &PWM_timer3_OC_params, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	}
}
// --------------------------------------------------------------------------
// PE3 - heating element near door. Timer channel 1
// PE4 - heating element inside.  Timer channel 2.
