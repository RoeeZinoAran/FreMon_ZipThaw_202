
#include "stm32l0xx_hal.h"
#include "constant.h"
#include "host_from_comm.h"
#include "host_to_comm.h"
#include "init.h"
#include "main.h"
#include "miscel.h"
#include "commserv.h"
#include "extern_structs.h"

extern UART_HandleTypeDef huart2;

extern unsigned short uart_tx_callback[5];
extern unsigned short uart_rx_callback[5];

extern unsigned short no_somm_message_timer;

uint8_t som_rx_buffer[MAX_MESSAGE_LENGTH];

unsigned short from_som_data_start_index; // The index in 'som_rx_buffer' where data begins

volatile uint8_t comm_err_array[100];
volatile uint8_t comm_err_array_index = 0;

/* --------------------------------------------------------------------------
 Top level of receiving full message from SOM.
-------------------------------------------------------------------------- */
void serve_host_comm(void)
{	
	// If a cmpleete message arrived: (complete does not mean that the 
	// messge is OK. It only means that all bytes arrived):
	// ---------------------------------------------------
	if (system_state.som_comm_state == COMPLETE_MESSAGE_RECEIVED) {
		system_state.som_comm_state = ANALYZING;
		get_from_som_message_header_values(); // Parse the header values
		
		// If after parsing found that addressed to me and all (CRC, PreAmble etc..) all OK:
		// --------------------------------------------------------------------------------
		if (from_som_message_state.result == MESSAGE_OK) {
			serve_opcode(); // Serve the command
			build_and_send_message_to_som();
			no_somm_message_timer = 0;
		}
		else { // if not a proper message addressed for me then
			init_receiving_from_som(); // start waiting for a new message
		}
	}

	// @@@wr: treat status FAIL
}
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Extracting header values of the message that was received received from SOM.
 This function assumes that the message is in som_rx_buffer[].

 The function updates all fields related to message header even if there is
 an error in the message, and even if the message is addressed to other board.

 The function returns OK if a full message received with CRC OK, and the
 message is addressed to me.
-------------------------------------------------------------------------- */
unsigned short get_from_som_message_header_values(void)
{	
	unsigned short index = 0;
	unsigned short crc_expected, crc_received;
	
	
	// Checking the preamble bytes:
	// ---------------------------
	if (som_rx_buffer[index] != '$') {
		from_som_message_state.result = PREAMBLE_ERROR;
		comm_err_array[comm_err_array_index] = 111;
		if (comm_err_array_index < 99)
			comm_err_array_index++;
		return FAIL1;
	}
	index++;
	if (som_rx_buffer[index] != '$') {
		from_som_message_state.result = PREAMBLE_ERROR;
		comm_err_array[comm_err_array_index] = 122;
		if (comm_err_array_index < 99)
			comm_err_array_index++;
		return FAIL1;
	}
	index++;
	
	// Message length:
	// --==;------------
//	from_som_message_state.message_length = (((uint16_t)som_rx_header_buffer[index]) << 8) | ((uint16_t)som_rx_header_buffer[index+1]);
	index += 1;
	
	
	// Message ID:
	// ----------
	from_som_message_state.message_id = (((uint16_t)som_rx_buffer[index]) << 8) | ((uint16_t)som_rx_buffer[index+1]);
	index += 2;
	
	// Sender and receiver IDs:
	// -----------------------
	from_som_message_state.sender_id   = som_rx_buffer[index];
	from_som_message_state.receiver_id = som_rx_buffer[index + 1];
	index += 2;
	
	
	// Checking if message was addressed to us:
	// ---------------------------------------
	if (from_som_message_state.receiver_id != system_state.my_som_comm_add) { // if Not addressed to me
		from_som_message_state.result = NOT_ADDRESSED_TO_ME;
		return(OK);
	}
	
	
	// OpCode type and subtype:
	// -----------------------
	from_som_message_state.opcode_type     = som_rx_buffer[index];
	from_som_message_state.opcode_sub_type = som_rx_buffer[index + 1];
	index += 2;
	
	
	// setting the index in 'som_rx_buffer' where data begins:
	// ------------------------------------------------------
	from_som_data_start_index = index;
	
	
	// Checking CRC:
	// ------------
	crc_expected = calculate_crc8(som_rx_buffer, from_som_message_state.message_length -1); // -1 because without the last byte which is the CRC itself
	crc_received = som_rx_buffer[from_som_message_state.message_length - 1];
	if (crc_expected != crc_received) {
		from_som_message_state.result = CRC_ERROR;
		comm_err_array[comm_err_array_index] = 133;
		if (comm_err_array_index < 99)
			comm_err_array_index++;		
		return(FAIL1);
	}
	
	from_som_message_state.result = MESSAGE_OK; // MESSAGE_OK means that message addressed to me and it is OK (CRC, PreAmble...  OK).
	return(OK);
}
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
