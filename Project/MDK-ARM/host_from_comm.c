
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "host_to_comm.h"
#include "stm32l0xx_hal.h"
#include "constant.h"
#include "host_from_comm.h"
#include "init.h"
#include "main.h"
#include "miscel.h"
#include "commserv.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: huart2
  Variable Type: UART_HandleTypeDef
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_MAIN_uart_tx_callback
  Variable Type: unsigned short [5]
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern unsigned short g_MAIN_uart_tx_callback[5];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_MAIN_uart_rx_callback
  Variable Type: unsigned short [5]
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern unsigned short g_MAIN_uart_rx_callback[5];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_MAIN_no_somm_message_timer
  Variable Type: unsigned short
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern unsigned short g_MAIN_no_somm_message_timer;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_HOST_FROM_COMM_som_rx_buffer
  Variable Type: uint8_t [C_HOST_FROM_COMM_MAX_MESSAGE_LENGTH]
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
uint8_t g_HOST_FROM_COMM_som_rx_buffer[C_HOST_FROM_COMM_MAX_MESSAGE_LENGTH];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_HOST_FROM_COMM_from_som_data_start_index
  Variable Type: unsigned short
  Unit: [N/A]
  Default value: N/A
  Description: The index in 'g_HOST_FROM_COMM_som_rx_buffer' where data begins
*/
/*--------------------------------------------------------------------------------------------------*/
unsigned short g_HOST_FROM_COMM_from_som_data_start_index; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_HOST_FROM_COMM_comm_err_array
  Variable Type: uint8_t [100]
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile uint8_t g_HOST_FROM_COMM_comm_err_array[100];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_HOST_FROM_COMM_comm_err_array_index
  Variable Type: uint8_t
  Unit: [N/A]
  Default value: 0
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile uint8_t g_HOST_FROM_COMM_comm_err_array_index = 0;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall handle top level of receiving full message from SOM.	
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_HOST_FROM_COMM_serve_host_comm(void)
{	
	/* If a cmpleete message arrived: (complete does not mean that the 
	   messge is OK. It only means that all bytes arrived) */

	if (system_state.som_comm_state == COMPLETE_MESSAGE_RECEIVED) 
	{
		system_state.som_comm_state = ANALYZING;
		p_HOST_FROM_COMM_get_from_som_message_header_values(); 	/* Parse the header values */
		
		/* If after parsing found that addressed to me and all (CRC, PreAmble etc..) all OK */
		if (from_som_message_state.result == MESSAGE_OK) 
		{
			p_COMMSERV_serve_opcode(); 					/* Serve the command */
			p_HOST_TO_COMM_build_and_send_message_to_som();
			g_MAIN_no_somm_message_timer = 0;
		}
		else		/* if not a proper message addressed for me then */ 
		{ 
			p_INIT_receiving_from_som(); /* start waiting for a new message */
		}
	}

	// @@@wr: treat status FAIL
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall unpack header values of the message that was received received from SOM.
 	The function shall updates all fields related to message header even if there is
 	an error in the message, and even if the message is addressed to other board.
\return unsigned short: OK if a full message received with CRC OK, and the message is addressed to me, 
        Otherwise the function shall return status FAIL1.
\DerivedDesc The function assumes that the income message is in g_HOST_FROM_COMM_som_rx_buffer[].
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
unsigned short p_HOST_FROM_COMM_get_from_som_message_header_values(void)
{	
	unsigned short index = 0;
	unsigned short crc_expected, crc_received;
	
	
	/* Checking the preamble bytes */
	if (g_HOST_FROM_COMM_som_rx_buffer[index] != '$') 
	{
		from_som_message_state.result = PREAMBLE_ERROR;
		g_HOST_FROM_COMM_comm_err_array[g_HOST_FROM_COMM_comm_err_array_index] = 111;
		if (g_HOST_FROM_COMM_comm_err_array_index < 99)
		{
			g_HOST_FROM_COMM_comm_err_array_index++;
		}
		return FAIL1;
	}

	index++;
	if (g_HOST_FROM_COMM_som_rx_buffer[index] != '$') 
	{
		from_som_message_state.result = PREAMBLE_ERROR;
		g_HOST_FROM_COMM_comm_err_array[g_HOST_FROM_COMM_comm_err_array_index] = 122;
		if (g_HOST_FROM_COMM_comm_err_array_index < 99)
		{
			g_HOST_FROM_COMM_comm_err_array_index++;
		}
		return FAIL1;
	}
	index++;
	
	/* Message length */
	/* from_som_message_state.message_length = (((uint16_t)som_rx_header_buffer[index]) << 8) | ((uint16_t)som_rx_header_buffer[index+1]); */
	index += 1;
	
	
	/* Message ID */
	from_som_message_state.message_id = (((uint16_t)g_HOST_FROM_COMM_som_rx_buffer[index]) << 8) | ((uint16_t)g_HOST_FROM_COMM_som_rx_buffer[index+1]);
	index += 2;
	
	/* Sender and receiver IDs */
	from_som_message_state.sender_id   = g_HOST_FROM_COMM_som_rx_buffer[index];
	from_som_message_state.receiver_id = g_HOST_FROM_COMM_som_rx_buffer[index + 1];
	index += 2;
	
	
	/* Checking if message was addressed to us */
	if (from_som_message_state.receiver_id != system_state.my_som_comm_add) 
	{
		/* if Not addressed to me */
		from_som_message_state.result = NOT_ADDRESSED_TO_ME;
		return(OK);
	}
	
	
	/* OpCode type and subtype */
	from_som_message_state.opcode_type     = g_HOST_FROM_COMM_som_rx_buffer[index];
	from_som_message_state.opcode_sub_type = g_HOST_FROM_COMM_som_rx_buffer[index + 1];
	index += 2;
	
	
	/* setting the index in 'g_HOST_FROM_COMM_som_rx_buffer' where data begins */
	g_HOST_FROM_COMM_from_som_data_start_index = index;
	
	
	/* Checking CRC */
	crc_expected = p_MISCEL_calculate_crc8(g_HOST_FROM_COMM_som_rx_buffer, from_som_message_state.message_length -1); // -1 because without the last byte which is the CRC itself
	crc_received = g_HOST_FROM_COMM_som_rx_buffer[from_som_message_state.message_length - 1];
	if (crc_expected != crc_received) 
	{
		from_som_message_state.result = CRC_ERROR;
		g_HOST_FROM_COMM_comm_err_array[g_HOST_FROM_COMM_comm_err_array_index] = 133;
		if (g_HOST_FROM_COMM_comm_err_array_index < 99)
		{
			g_HOST_FROM_COMM_comm_err_array_index++;		
		}
		return(FAIL1);
	}
	
	/* MESSAGE_OK means that message addressed to me and it is OK (CRC, PreAmble...  OK). */
	from_som_message_state.result = MESSAGE_OK; 
	return(OK);
}
