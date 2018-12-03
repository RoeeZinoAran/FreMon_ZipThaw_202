
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "pid.h"
#include "pid_params.h"
#include "heater.h"
#include "discreets.h"
#include "main.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_PID_error
  Variable Type: unsigned int [2]
  Unit: [N/A]
  Default value: N/A
  Description: Desired temperatire minus Measured temperature in  8 mC units. (Each bit
			   represents 0.008 C).
               It is an unsigned value because that it is used only when measured
               temperature is lower the target temperature.
			   Size = 2 one for each cushion.
*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned int g_PID_error[2]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_PID_error_mx
  Variable Type: unsigned int [2]
  Unit: [N/A]
  Default value: N/A
  Description: The errors N samples before.
*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned int g_PID_error_mx[2]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_PID_pid_integral
  Variable Type: signed int [2]
  Unit: [N/A]
  Default value: N/A
  Description: 2 one for each cushion.
*/
/*--------------------------------------------------------------------------------------------------*/
static signed int g_PID_pid_integral[2]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_PID_integral_history
  Variable Type: signed int [2][PID_INTEGRAL_DEPTH]
  Unit: [N/A]
  Default value: N/A
  Description: 2 one for each cushion.
*/
/*--------------------------------------------------------------------------------------------------*/
static signed int g_PID_integral_history[2][PID_INTEGRAL_DEPTH]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_PID_integral_pointer
  Variable Type: unsigned short
  Unit: [N/A]
  Default value: 0
  Description: points to the next index where new data will be written tos
               Integral of errors. Increases (becomes more positive) when 'g_PID_error'
               is positive. 
*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned short g_PID_integral_pointer = 0; 

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall Control word calculation.  The function returns the required duty cycle
 	of the heating elemnts' PWM.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void g_PID_calc_pid(void)
{

	static unsigned short 	calc_num;
	unsigned short 			cushion, s1;
	unsigned int   			i1, i2;
	unsigned int   			result[2];
	
	/* Loop for the two cushions */
	for(cushion = 0; cushion < 2; cushion++) 
	{ 	
		/* If temperature is very high then disable heating */
		if (system_state.ntc_temp[cushion] >= HIGHEST_TEMP_THAT_PWM_CAN_OPERATE) 
		{
			result[cushion] = 0;
			for(s1 = 0; s1 < PID_INTEGRAL_DEPTH; s1++)
			{
				/* resetting the integral */
				g_PID_integral_history[cushion][s1] = 0; 
			}
			g_PID_pid_integral[cushion] = 0;
			continue;
		}
		
		/* Find value of Error */
		g_PID_error[cushion] = (HIGHEST_TEMP_THAT_PWM_CAN_OPERATE / 8) - (system_state.ntc_temp[cushion] / 8); // div 8 because we calculate in 8mC (and not 1 mC) resolution
		
		/* Calculate I part of PID */
		/* The new value to be added to Integral */

		i1 = g_PID_error[cushion] * pid_i_contst[cushion];

#if 0
		g_PID_pid_integral[cushion] -= g_PID_integral_history[cushion][g_PID_integral_pointer]; // subtracting the oldest value
		g_PID_pid_integral[cushion] += i1; // adding to the new value
#endif
		
		g_PID_integral_history[cushion][g_PID_integral_pointer] = i1; // inserting the newest value instead of the oldest one in the history log
		if (cushion) { // if now the second cushion is treated (the first was already just treated)
			g_PID_integral_pointer++;
			if (g_PID_integral_pointer >= PID_INTEGRAL_DEPTH)
				g_PID_integral_pointer = 0;
		}
		i1 = 0;
		
		for(s1 = 0; s1 < PID_INTEGRAL_DEPTH; s1++)
		{
			i1 += g_PID_integral_history[cushion][s1];
		}
			
		g_PID_pid_integral[cushion] = i1;
		
		/* Calculating the P part of PID and adding it to the I part with clamping as needed */
		i1 = 0xffffffff - g_PID_pid_integral[cushion]; /* see how close is the Integral to max 32 bit value */
		i2 = g_PID_error[cushion] * pid_p_contst[cushion];
		if (i2 > i1)
		{
			result[cushion] = 0xffffffff; /* clamp */
		}
		else
			result[cushion] = g_PID_pid_integral[cushion] + i2;
		
		/* Calculating the D part of PID and adding it to the result with clamping as needed */
		if (calc_num == NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID) 
		{
			
			if (g_PID_error[cushion] >= g_PID_error_mx[cushion])	/* if error became bigger then at last time was checked */
			{
				i1 = 0xffffffff - result[cushion]; 				/* see how close is the Integral to max 32 bit value */
				i2 = (g_PID_error[cushion] - g_PID_error_mx[cushion]) * pid_d_contst[cushion];
				if (i2 > i1)
					result[cushion] = 0xffffffff; 				/* clamp */
				else
					result[cushion] -= i2;
			}
			else 												/* if error became bigger then at last time was checked */
			{ 
				i2 = (g_PID_error_mx[cushion] - g_PID_error[cushion]) * pid_d_contst[cushion];
				if (i2 > result[cushion])
					result[cushion] = 0;
				else
					result[cushion] += i2;
			}
			
			/* Store error as was N loops before for caalculating the Derivative part of PID: */
			g_PID_error_mx[cushion] = g_PID_error[cushion]; 
		}
	}
	
	/* Update loop (calculation) number */
	if (calc_num >= NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID)
	{
		calc_num = 0;
	}
	else
	{
		calc_num++;
	}
	
	/* Update results in structure and clump as needed */
	if (system_state.g_DISCREETS_door_state == C_DISCREETS_DOOR_OPEN) 
	{
		system_state.cushion_duty_cycle[CUSHION0] = 0;
		system_state.cushion_duty_cycle[CUSHION1] = 0;
	}
	else 
	{
		system_state.cushion_duty_cycle[CUSHION0] = (result[CUSHION0] / 2048) > PWM_MAX_VALUE ? PWM_MAX_VALUE : result[CUSHION0] / 2048;
		system_state.cushion_duty_cycle[CUSHION1] = (result[CUSHION1] / 2048) > PWM_MAX_VALUE ? PWM_MAX_VALUE : result[CUSHION1] / 2048;;
	}
}
