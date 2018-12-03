
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HOST_FROM_COMM_NUMBER_OF_HEADER_BYTES_FROM_SOM
  Unit: [N/A]
  Define Value: 4
  Description:  2 bytes of pre-amble, and two bytes of length.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HOST_FROM_COMM_NUMBER_OF_HEADER_BYTES_FROM_SOM   	(4)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HOST_FROM_COMM_MAX_MESSAGE_LENGTH
  Unit: [N/A]
  Define Value: 135
  Description: Maximum allawed message length.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HOST_FROM_COMM_MAX_MESSAGE_LENGTH    				(135)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HOST_FROM_COMM_NUM_OF_HEADER_BYTES
  Unit: [N/A]
  Define Value: 3
  Description: Number of bytes in the header of the message recived from SOM. IT is the
               number of bytes until the field of message length is received (including
               the message length bytes).
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HOST_FROM_COMM_NUM_OF_HEADER_BYTES					(3)


/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HOST_FROM_COMM_NO_SOM_MESSAGE_TIMO
  Unit: [N/A]
  Define Value: 1500
  Description: The time in mSec that if no message from SOM (header or message body)
			   then communication with SOM is considered as faild.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HOST_FROM_COMM_NO_SOM_MESSAGE_TIMO               	(1500)

#if 0
  #define FROM_SOM_MESSAGE_BODY_LENGTH      28
#endif

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

uint8_t p_MISCEL_calculate_crc8 (uint8_t *msg_array, uint8_t msg_size);
unsigned short p_HOST_FROM_COMM_get_from_som_message_header_values(void);
void p_HOST_FROM_COMM_serve_host_comm(void);
