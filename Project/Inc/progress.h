
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: PROGRESS_TIMER_BIT
  Unit: [bytes]
  Define Value: (1 <<  4)
  Description: Definitions for the  'one_second_passed_bits' variable
--------------------------------------------------------------------------------------------------*/
#define PROGRESS_TIMER_BIT								(1 <<  4)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_PROGRESS_stage_and_rpogress(void);
uint8_t p_PROGRESS_temp_to_percent(void);
void p_PROGRESS_standby_stage(void);
void p_PROGRESS_thawing_stage(void);
void p_PROGRESS_after_thawing_stage(void);






/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UNUSED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

//#define PID_LOOP_PERIOD_TIMER_BIT 				(1 <<  0)
//#define AGITATION_MOTOR_TASK_TIMER_BIT		(1 <<  3)
