
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "technician.h"
#include <string.h>
#include "stm32l0xx_hal.h"
#include "adc.h"
#include "miscel.h"
#include "rfid.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: 
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern uint32_t g_ADC_adc_raw_results[NUM_OF_ADC_CHANNELS];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: 
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern uint8_t g_MAIN_from_rfid_str[C_RFID_STRING_ARRAY_SIZE]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: 
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: 
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
char g_TECHNICIAN_str_to_debug[500];

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_TECHNICIAN_send_debug_values(void)
{
	unsigned short 	s1;
	char	str1[20];
	char	title0[80] = "  Heat0     Heat1     NTC IN    NTC OUT   WEIGHT    P3V3      P24V0  \n\r";
  	char 	title1[80] = "  [Amp]     [Amp]      [C]       [C]       [gr]     [V]        [V]   \n\r";

#if 0
	g_TECHNICIAN_str_to_debug[0] = 12; // clear screen
	g_TECHNICIAN_str_to_debug[0] = 27;
	g_TECHNICIAN_str_to_debug[1] = 'c';
#endif
	
	strcpy(g_TECHNICIAN_str_to_debug, "\033[2J");
//	g_TECHNICIAN_str_to_debug[0] = 12;
	strcat(g_TECHNICIAN_str_to_debug, "\n\n\n\r\r\r\r");
	strcat(g_TECHNICIAN_str_to_debug, "\n\r");
	strcat(g_TECHNICIAN_str_to_debug, title0);
	strcat(g_TECHNICIAN_str_to_debug, title1);

	p_MISCEL_int_to_str(system_state.heater_current[CUSHION0], str1,  7, 3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	p_MISCEL_int_to_str(system_state.heater_current[CUSHION1], str1, 10, 3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	p_MISCEL_int_to_str(system_state.ntc_temp[CUSHION0], str1, 10, 3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	p_MISCEL_int_to_str(system_state.ntc_temp[CUSHION1], str1, 10, 3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	p_MISCEL_int_to_str(system_state.weight, str1, 10, 0);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	p_MISCEL_int_to_str(system_state.p3v3, str1, 10,3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	p_MISCEL_int_to_str(system_state.p24v0, str1, 10,3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	
	strcat(g_TECHNICIAN_str_to_debug, "\n\r\n\r COMM. ADD:");
	p_MISCEL_int_to_str(system_state.my_som_comm_add, str1, 5, 0);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	
	strcat(g_TECHNICIAN_str_to_debug, "\n\r\n\r RFID: ");
	g_MAIN_from_rfid_str[14] = 0;
	if (system_state.rfid_temp > 99900) /* if error */
	{
		strcat(g_TECHNICIAN_str_to_debug, "99.999");
	}
	else
	{
		strcat(g_TECHNICIAN_str_to_debug, (char *)(&g_MAIN_from_rfid_str[8]));
	}
	
	strcat(g_TECHNICIAN_str_to_debug, "       IR:");
	p_MISCEL_int_to_str(system_state.ir_temp, str1, 10,3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	
	strcat(g_TECHNICIAN_str_to_debug, "       PCB_TEMP:");
	p_MISCEL_int_to_str(system_state.pcb_temp, str1, 8,3);
	strcat(g_TECHNICIAN_str_to_debug, str1);
	
	strcat(g_TECHNICIAN_str_to_debug, system_state.g_DISCREETS_door_state ? "\n\r\n\r Door Closed" : "\n\r\n\r Door Open  ");
	strcat(g_TECHNICIAN_str_to_debug, system_state.g_DISCREETS_motor_index ? "        Motor IN Index" : "        Motor OUT of Index");
	
	strcat(g_TECHNICIAN_str_to_debug, "\n\r\n\r");
	
	for(s1 = 0; g_TECHNICIAN_str_to_debug[s1]; s1++);
	
	HAL_UART_Transmit_DMA (&huart1, (uint8_t *)g_TECHNICIAN_str_to_debug, s1);
}

