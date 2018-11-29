
#include "stm32l0xx_hal.h"


// --------------------------------------------------------------------------
#define NUMBER_OF_HEADER_BYTES_FROM_SOM   4
// 2 bytes of pre-amble, and two bytes of length.
// --------------------------------------------------------------------------
#define MAX_MESSAGE_LENGTH       					135
// --------------------------------------------------------------------------
#define NUM_OF_HEADER_BYTES               3
// Number of bytes in the header of the message recived from SOM. IT is the
// number of bytes until the field of message length is received (including
// the message length bytes).
// --------------------------------------------------------------------------
//#define FROM_SOM_MESSAGE_BODY_LENGTH      28
// Nimber of bytes from SOM that follow the header bytes including CRC.
// The total message length (in bytes) is this number + NUM_OF_HEADER_BYTES
// --------------------------------------------------------------------------
#define NO_SOM_MESSAGE_TIMO               1500
// The time in mSec that if no message from SOM (header or message body)
// then communication with SOM is considered as faild.
// --------------------------------------------------------------------------


uint8_t Calculate_crc8 (uint8_t *msg_array, uint8_t msg_size);

unsigned short get_from_som_message_header_values(void);

uint8_t Calculate_crc8 (uint8_t *msg_array, uint8_t msg_size);

void serve_host_comm(void);
