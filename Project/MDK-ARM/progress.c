
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "pid_params.h"
#include "discreets.h"
#include "rfid.h"
#include "progress.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: one_second_passed_bits
  Variable Type: unsigned short
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short one_second_passed_bits;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall treating issues related to the current stage of the machine (Thawing, Standby
 	etc...
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_PROGRESS_stage_and_rpogress(void)
{
	if (system_state.tasks_in_progress & C_EXTERN_STRUCTS_STANDBY_TASK_BIT)
	{
		p_PROGRESS_standby_stage();
	}
	else if (system_state.tasks_in_progress & C_EXTERN_STRUCTS_THAWING_TASK_BIT)
	{
		p_PROGRESS_thawing_stage();
	}
	else if (system_state.tasks_in_progress & C_EXTERN_STRUCTS_AFTER_THAWING_TASK_BIT)
	{
		p_PROGRESS_after_thawing_stage();
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall put the system in stand-by mode.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_PROGRESS_standby_stage(void)
{
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall treating the THAWING stage (task). This is the stage from start until end of
 thawing.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_PROGRESS_thawing_stage(void)
{
	uint8_t c0;
	
	c0 = p_PROGRESS_temp_to_percent();
	
	if (c0 == 0xff) /* if error in RFID */
	{
		return;
	}
	else
	{
		if (c0 >= system_state.progress)
		{
			system_state.progress = (unsigned short)c0;
		}
	}
	
	if (system_state.progress >= 100) /* if 100% i.e. thawing compleeted */ 
	{ 
		system_state.tasks_in_progress = C_EXTERN_STRUCTS_AFTER_THAWING_TASK_BIT;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall treating the AFTER_THAWING stage (task). This is the stage from end of thawing
 	untl opening the door.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_PROGRESS_after_thawing_stage(void)
{
	/* If time to updated that one more second passed */
	if (one_second_passed_bits & PROGRESS_TIMER_BIT) {
		one_second_passed_bits &=  (~PROGRESS_TIMER_BIT); 	/* clear the bit that indicates that one sedond passed */
		if (system_state.after_thawing_timer < 0xffff) 		/* increment one second */
			system_state.after_thawing_timer++;
	}
	
	/* If door was open then go to standby state */
	if (system_state.g_DISCREETS_door_state == C_DISCREETS_DOOR_OPEN) {
		system_state.tasks_in_progress = C_EXTERN_STRUCTS_STANDBY_TASK_BIT;
		system_state.after_thawing_timer = 0;
		system_state.progress = 0xff; // inactive
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall telling the percentage of the thawing process as function of RFID temperature.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
uint8_t p_PROGRESS_temp_to_percent(void)
{
	signed int   is1;
	
	/* If RFID is not readable then exit */
	if ((system_state.rfid_temp == C_RFID_COMM_ERROR_RETURNED_TEMPERATURE) || (system_state.rfid_temp == C_RFID_NO_ANSWER_RETURNED_TEMPERATURE))
	{
		return 0xff; /* Error */	
	}
		
	
	is1 = (system_state.rfid_temp - LOWEST_PLASMA_TEMPERATURE) * 100;
	is1 /= (THAWING_TARGET_TEMPERATURE - LOWEST_PLASMA_TEMPERATURE);
	
	if (is1 < 0)
	{
		is1 = 0;
	}
	if ((system_state.rfid_temp >= THAWING_TARGET_TEMPERATURE) || (is1 > 100))
	{
		is1 = 100; /* 100% */
	}
	
	return ((uint8_t)is1);
}
