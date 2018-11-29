
#include "stm32l0xx_hal.h"

#define MAX_MESSAGE_LENGTH_TO_SOM  135

// --------------------------------------------------------------------------
#define NUM_OF_TO_SOM_BYTES        128
// The exact total number of bytes sent to SOM in one message.
// --------------------------------------------------------------------------


void build_and_send_message_to_som(void);
