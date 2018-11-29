
#include "stm32l0xx_hal.h"


// --------------------------------------------------------------------------
// Definitions for the  'one_second_passed_bits' variable
// --------------------------------------------------------------------------
//#define PID_LOOP_PERIOD_TIMER_BIT 				(1 <<  0)
//#define AGITATION_MOTOR_TASK_TIMER_BIT		(1 <<  3)
#define PROGRESS_TIMER_BIT								(1 <<  4)
// --------------------------------------------------------------------------


void stage_and_rpogress(void);
uint8_t temp_to_percent(void);

void standby_stage(void);
void thawing_stage(void);
void after_thawing_stage(void);

