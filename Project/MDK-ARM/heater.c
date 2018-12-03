
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "heater.h"
#include "discreets.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: htim3
  Variable Type: TIM_HandleTypeDef
  Unit: [N/A]
  Extern module declaration: Main. 
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: PWM_timer3_OC_params
  Variable Type: TIM_OC_InitTypeDef
  Extern module declaration: Heater. 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern TIM_OC_InitTypeDef PWM_timer3_OC_params;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: safety_error
  Variable Type: unsigned short
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: Indicate safety error bit.
*/
/*--------------------------------------------------------------------------------------------------*/
extern unsigned short safety_error;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall updating heater PWM for both cushins according to data in system_state structure.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void update_heaters_power(void)
{
	/* If door is open then shut PWM off (0% duty cycle). */
	if ((system_state.door_state == DOOR_OPEN) || (safety_error >= 1000)) 
	{
		system_state.cushion_duty_cycle[CUSHION0] = 0;
		system_state.cushion_duty_cycle[CUSHION1] = 0;
		HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_RESET);
	}
	
	else 
	{
		HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_SET);
		
		/* PWM_timer3_OC_params.Pulse = PWM_UNLIMITED_MAX_VALUE - system_state.cushion_duty_cycle[CUSHION0]; */
		PWM_timer3_OC_params.Pulse = system_state.cushion_duty_cycle[CUSHION0];
		if (PWM_timer3_OC_params.Pulse >= 10000) 
		{
			PWM_timer3_OC_params.Pulse = 9999;
			HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_RESET);
		}
		HAL_TIM_PWM_ConfigChannel(&htim3, &PWM_timer3_OC_params, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

		/* PWM_timer3_OC_params.Pulse = PWM_UNLIMITED_MAX_VALUE - system_state.cushion_duty_cycle[CUSHION1]; */
		PWM_timer3_OC_params.Pulse = system_state.cushion_duty_cycle[CUSHION1];
		if (PWM_timer3_OC_params.Pulse >= 10000) 
		{
			PWM_timer3_OC_params.Pulse = 9999;
			HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_RESET);
		}
		HAL_TIM_PWM_ConfigChannel(&htim3, &PWM_timer3_OC_params, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	}
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UNUSED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#if 0
// PE3 - heating element near door. Timer channel 1
// PE4 - heating element inside.  Timer channel 2.

#endif
