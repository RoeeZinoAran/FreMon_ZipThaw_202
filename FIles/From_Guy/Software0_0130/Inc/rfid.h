
#include "stm32l0xx_hal.h"

void get_rfid_version(uint8_t *str);
void get_rfid_page(uint8_t add, uint8_t *str);
signed int rfid_temp_ascii_to_int(void);

void get_rfid_temperature(uint8_t *str);
void get_rfid_counter(uint8_t *str);
void increment_rfid_counter(uint8_t *str);
void reset_rfid_counter(uint8_t *str);

#define RFID_STRING_ARRAY_SIZE      40


// --------------------------------------------------------------------------
#define RFID_TIMO										300
// Time in mS that if RFID does not answer then it is retried
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
#define RFID_COMM_ERROR_RETURNED_TEMPERATURE			99900
#define RFID_NO_ANSWER_RETURNED_TEMPERATURE				88800
// Temperatures (in mC) that are returned when there are RFID problems
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 List of RFID tasks:
-------------------------------------------------------------------------- */
#define RESET_COUNTER_RFID_TASK								0
#define INCREMENT_COUNTER_RFID_TASK						1
#define READ_COUNTER_RFID_TASK                2
#define READ_TEMPERATURE_RFID_TASK						3
// --------------------------------------------------------------------------


//
