
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HOST_TO_COMM_MAX_MSG_LEN_TO_SOM
  Unit: [bytes]
  Define Value: 135
  Description:  Maximum allowed message length [bytes] to SOM
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HOST_TO_COMM_MAX_MSG_LEN_TO_SOM  (135)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HOST_TO_COMM_NUM_OF_TO_SOM_BYTES
  Unit: [bytes]
  Define Value: 128
  Description: The exact total number of bytes sent to SOM in one message.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HOST_TO_COMM_NUM_OF_TO_SOM_BYTES        (128)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_HOST_TO_COMM_build_and_send_message_to_som(void);
