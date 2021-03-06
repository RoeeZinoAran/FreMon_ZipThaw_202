
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "commserv.h"
#include "host_from_comm.h"
#include "discreets.h"
#include "motor.h"
#include "heater.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_HOST_FROM_COMM_som_rx_buffer
  Variable Type: uint8_t [C_HOST_FROM_COMM_MAX_MESSAGE_LENGTH].
  Extern module declaration: Host_from_com.
  Unit: [N/A]
  Default value: N/A
  Description: Message from SOM (RX to system) buffer.
*/
/*--------------------------------------------------------------------------------------------------*/
extern uint8_t g_HOST_FROM_COMM_som_rx_buffer[C_HOST_FROM_COMM_MAX_MESSAGE_LENGTH];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_HOST_FROM_COMM_from_som_data_start_index
  Variable Type: unsigned short
  Extern module declaration: Host_from_com.
  Unit: [N/A]
  Default value: N/A
  Description: From Som data start index.
*/
/*--------------------------------------------------------------------------------------------------*/
extern unsigned short g_HOST_FROM_COMM_from_som_data_start_index;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall be called for executing command that was requested by the
 	SOM. This function is intended to be called after enire message is received
 	and found OK i.e.  CRC is OK, the message is addressed to me etc...
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_COMMSERV_serve_opcode(void)
{	
	switch (from_som_message_state.opcode_type)
	 {
		case C_COMMSERV_LED_BUZZER_OPCODE: 		/* a command from SOM to turn on or off the LEDs or the Buzzer */
			p_COMMSERV_serve_led_buzzer_opcode();
			break;

		case C_COMMSERV_HEAT_AND_AGITATION_OPCODE: /* Technician command to turn on or off the heating elements or the aggitation motor. */
			p_COMMSERV_serve_agitation_and_heater_opcode();
			break;

		case C_COMMSERV_TECHNICIAN_MODE_OPCODE: 	/* command to enter into technician mode. */
			p_COMMSERV_serve_technician_opcode();
			break;

		case C_COMMSERV_START_STOP_THAWING_OPCODE: /* command from SOM to start or stop thawing. */
			p_COMMSERV_serve_start_stop_thawing_opcode();
			break;

		default: 
			break;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall serving the command from SOM for starting or for stopping thawing.	
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_COMMSERV_serve_start_stop_thawing_opcode(void)
{
	if (from_som_message_state.opcode_sub_type == C_COMMSERV_START_THAWING_SUB_CODE) 		/* if command to start thawing */
	{ 
		system_state.tasks_in_progress = C_EXTERN_STRUCTS_THAWING_TASK_BIT;
		system_state.progress = 0; 	/* Reset */
		system_state.after_thawing_timer = 0;
	}
	else if (from_som_message_state.opcode_sub_type == C_COMMSERV_STOP_THAWING_SUB_CODE) 	/* if command to stop thawing */
	{ 
		system_state.tasks_in_progress = C_EXTERN_STRUCTS_AFTER_THAWING_TASK_BIT;
		system_state.progress = 0xff; /* not used */
		system_state.after_thawing_timer = 0;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall serving the command from SOM for entering or exiting technician mode.
 This function is to be called after command from SOM was decoded.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_COMMSERV_serve_technician_opcode(void)
{
	if (from_som_message_state.opcode_sub_type == C_COMMSERV_TECHNICIAN_MODE_ON_SUB_CODE) 
	{
		system_state.tasks_in_progress = C_EXTERN_STRUCTS_TECHNICIAN_TASK_BIT;
		system_state.progress = 0xff; /* inactive */
		system_state.after_thawing_timer = 0;
	}
	else 
	{
		system_state.tasks_in_progress = C_EXTERN_STRUCTS_STANDBY_TASK_BIT;
	}
	
	system_state.cushion_duty_cycle[CUSHION0] = 0;
	system_state.cushion_duty_cycle[CUSHION1] = 0;
	p_HEATER_update_heaters_power();
	
	//@@@wr progress 0,  I of PID=0
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall serving the command from SOM for turning on or off a LED color.
 	This function is to be called after command from SOM was decoded.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_COMMSERV_serve_led_buzzer_opcode(void)
{
	unsigned short s1;

	s1 = system_state.leds_state;
	
	if (from_som_message_state.opcode_sub_type == C_COMMSERV_TURN_LED_ON_SUB_CODE) 
	{
		s1 |= (unsigned short)g_HOST_FROM_COMM_som_rx_buffer[g_HOST_FROM_COMM_from_som_data_start_index];
	}
	else 
	{
		s1 &= (~((unsigned short)g_HOST_FROM_COMM_som_rx_buffer[g_HOST_FROM_COMM_from_som_data_start_index]));
	}
	p_DISCREETS_rgb_leds(s1 & (1 << 0), s1 & (1 << 1), s1 & (1 << 2));
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall serving the command from SOM for turning on or off the aggitation motor,
 	or for turning On or Off the heating element.
 	This function is to be called after command from SOM was decoded.	
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_COMMSERV_serve_agitation_and_heater_opcode(void)
{

#if 0
	// If not in technician mode then ignore and error:
	// -----------------------------------------------
	if (system_state.tasks_in_progress != C_EXTERN_STRUCTS_TECHNICIAN_TASK_BIT) {
		return; // @@@wr inform error - illegal command for this state
	}
	
	if (from_som_message_state.opcode_sub_type == C_COMMSERV_AGITATION_MOTOR_ON_SUB_CODE) {
		p_MOTOR_update_agitation_motor_register(0x50, 0x00, 0x1c, 0x00); // Run. bit 0 of first byte (of four) is direction.  Last 20 bits are speed.
	}
	
	else if (from_som_message_state.opcode_sub_type == C_COMMSERV_AGITATION_MOTOR_OFF_SUB_CODE) {
		p_MOTOR_update_agitation_motor_register(0x50, 0x00, 0x00, 0x00); // Run. bit 0 of first byte (of four) is direction.  Last 20 bits are speed.
	}
	
	else if (from_som_message_state.opcode_sub_type == C_COMMSERV_HEATING_ELEMENT_ON_SUB_CODE) {
		if (g_HOST_FROM_COMM_som_rx_buffer[g_HOST_FROM_COMM_from_som_data_start_index] & (1 << 0))
			system_state.cushion_duty_cycle[CUSHION0] = 7500;
		if (g_HOST_FROM_COMM_som_rx_buffer[g_HOST_FROM_COMM_from_som_data_start_index] & (1 << 1))
			system_state.cushion_duty_cycle[CUSHION1] = 7500;
		p_HEATER_update_heaters_power();
	}

	else if (from_som_message_state.opcode_sub_type == C_COMMSERV_HEATING_ELEMENT_OFF_SUB_CODE) {
		if (g_HOST_FROM_COMM_som_rx_buffer[g_HOST_FROM_COMM_from_som_data_start_index] & (1 << 0))
			system_state.cushion_duty_cycle[CUSHION0] = 0;
		if (g_HOST_FROM_COMM_som_rx_buffer[g_HOST_FROM_COMM_from_som_data_start_index] & (1 << 1))
			system_state.cushion_duty_cycle[CUSHION1] = 0;
		p_HEATER_update_heaters_power();
	}
#endif

}
