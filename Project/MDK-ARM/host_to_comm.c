
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "host_to_comm.h"
#include "miscel.h"
#include "main.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short time_from_thawing_end;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
uint8_t som_tx_buffer[MAX_MESSAGE_LENGTH_TO_SOM + 5];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile unsigned int debug_sdlfjh83345 = 0;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall building the message buffer for data to be transmitted to SOM and sending it.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void build_and_send_message_to_som(void)
{
	unsigned short 			s1;
 	signed short 			si1;
	static unsigned short 	blinking_counter;
	uint8_t*				pt;
	
	/* Updating blinking LED to show answering to SOM: */
	blinking_counter++;
	if ((blinking_counter % 16) == 0)
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
	else if ((blinking_counter % 16) == 8)
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	
	
	pt = som_tx_buffer;
	
	/* Building message header */
	*pt++ = '$';
	*pt++ = '$';
	*pt++ = NUM_OF_TO_SOM_BYTES;
	*pt++ = from_som_message_state.message_id >> 8;
	*pt++ = from_som_message_state.message_id & 0xff;
	*pt++ = system_state.my_som_comm_add; 		/* sender ID (me) */
	*pt++ = from_som_message_state.sender_id; 	/* receiver ID (SOM - the one who has sent to me) */
	
	/* Single bit sensors */
	*pt++ = system_state.door_state  ? 1 : 0; 	/* 1 if door closed */
	*pt++ = system_state.motor_index ? 1 : 0; 	/* 1 if motor at index */
	*pt++ = 0; 									/* 1 if ready for boot */
	*pt++ = 0; 									/* spare */
	*pt++ = 0; 									/* spare */
	
	/* Data from sensors */
	*pt++ = 0; 									/* spare */
	*pt++ = 0; 									/* spare */
	
	si1 = (signed short)(system_state.ntc_temp[CUSHION0] / 100); 	/* dividing by 100 because we return in 1/10C units and not in mC units */
	*pt++ = (uint8_t)(si1 >> 8);   									/* Temp. sensor #1 – cushion #1 MSByte in 1/10 C units */
	*pt++ = (uint8_t)(si1 & 0xff); 									/* Temp. sensor #1 – cushion #1 LSByte in 1/10 C units */
	*pt++ = 0; 														/* spare */
	*pt++ = 0; 														/* spare */
	*pt++ = 0; 														/* spare */
	*pt++ = 0; 														/* spare */ 
	
	si1 = (signed short)(system_state.ntc_temp[CUSHION1] / 100); 	/* dividing by 100 because we return in 1/10C units and not in mC units */
	*pt++ = (uint8_t)(si1 >> 8);   // Temp. sensor #1 – cushion #2 MSByte in 1/10 C units
	*pt++ = (uint8_t)(si1 & 0xff); // Temp. sensor #1 – cushion #2 LSByte in 1/10 C units
	*pt++ = 0; // spare
	*pt++ = 0; // spare
	
	si1 = (signed short)(system_state.rfid_temp / 100); // dividing by 100 because we return in 1/10C units and not in mC units
	*pt++ = (uint8_t)(si1 >> 8);   // RFID Temp. sensor MSByte in 1/10 C units
	*pt++ = (uint8_t)(si1 & 0xff); // RFID Temp. sensor LSByte in 1/10 C units
	
	si1 = (signed short)(system_state.ir_temp / 100); // dividing by 100 because we return in 1/10C units and not in mC units
	*pt++ = (uint8_t)(si1 >> 8);   // IR Temp. sensor MSByte in 1/10 C units
	*pt++ = (uint8_t)(si1 & 0xff); // IR Temp. sensor LSByte in 1/10 C units
	
	si1 = (signed short)(system_state.pcb_temp / 100); // dividing by 100 because we return in 1/10C units and not in mC units
	*pt++ = (uint8_t)(si1 >> 8);   // PCB Temp. sensor MSByte in 1/10 C units
	*pt++ = (uint8_t)(si1 & 0xff); // PCB Temp. sensor LSByte in 1/10 C units
	
	s1 = (signed short)system_state.weight; // dividing by 100 because we return in 1/10C units and not in mC units
	*pt++ = (uint8_t)(s1 >> 8);   // Bag weight in grams MSByte
	*pt++ = (uint8_t)(s1 & 0xff); // Bag weight in grams MSByte
	
	*pt++ = (uint8_t)(system_state.after_thawing_timer >> 8);   // Time from End of Thawing process  MSByte in seconds
	*pt++ = (uint8_t)(system_state.after_thawing_timer & 0xff); // Time from End of Thawing process  MSByte in seconds

	for(s1 = 0; s1 < 10; s1++)
		*pt++ = 0; // spare

	s1 = (signed short)system_state.p3v3;
	*pt++ = (uint8_t)(s1 >> 8);
	*pt++ = (uint8_t)(s1 & 0xff);
	
	s1 = (signed short)system_state.p24v0;
	*pt++ = (uint8_t)(s1 >> 8);
	*pt++ = (uint8_t)(s1 & 0xff);
	

	for(s1 = 0; s1 < 22; s1++)
		*pt++ = 0; // spare
	
	
	/* Process Status (0xff means inactive) */
	*pt++ = 0xff; 					/* Pre-heating chamber process status */
	*pt++ = 0; 						/* standby chamber process status */
	*pt++ = system_state.progress; 	/* thawing chamber process status */

	for(s1 = 0; s1 < 46; s1++) // @@@wr write process progress
	{
		*pt++ = 0; // spare
	}
	
	
	/* FirmWare version: */
	*pt++ = (uint8_t)(FIRMWARE_VERSION >> 8);
	*pt++ = (uint8_t)(FIRMWARE_VERSION & 0xff);

	
	/* Error Status table */
	for(s1 = 0; s1 < 2; s1++) 	/* write error bits */
	{
		*pt++ = 0;
	}
	
	for(s1 = 0; s1 < 4; s1++)
	{
		*pt++ = 0; // spare
	}

	
	/* ADDING CRC */
	som_tx_buffer[NUM_OF_TO_SOM_BYTES - 1] = calculate_crc8(som_tx_buffer, NUM_OF_TO_SOM_BYTES - 1);
	
	
	/* TRANSMITTING */
	system_state.som_comm_state = TRANSMITTING;
	HAL_UART_Transmit_DMA(&huart2, som_tx_buffer, NUM_OF_TO_SOM_BYTES);
}
