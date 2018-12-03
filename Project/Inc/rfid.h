
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: RFID_STRING_ARRAY_SIZE
  Unit: [N/A]
  Define Value: 40
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_STRING_ARRAY_SIZE						(40)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: RFID_TIMO
  Unit: [mSec]
  Define Value: 300
  Description:  Time in mS that if RFID does not answer then it is retried
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_TIMO									(300)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: RFID_COMM_ERROR_RETURNED_TEMPERATURE
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_COMM_ERROR_RETURNED_TEMPERATURE		(99900)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: RFID_NO_ANSWER_RETURNED_TEMPERATURE
  Unit: [N/A]
  Define Value: 10000u
  Description:  Temperatures (in mC) that are returned when there are RFID problems
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_NO_ANSWER_RETURNED_TEMPERATURE			(88800)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: 
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define RESET_COUNTER_RFID_TASK						(0)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: INCREMENT_COUNTER_RFID_TASK
  Unit: [N/A]
  Define Value: 1
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define INCREMENT_COUNTER_RFID_TASK					(1)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: 
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define READ_COUNTER_RFID_TASK						(2)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: 
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define READ_TEMPERATURE_RFID_TASK					(3)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void get_rfid_version(uint8_t *str);
void get_rfid_page(uint8_t add, uint8_t *str);
signed int rfid_temp_ascii_to_int(void);
void get_rfid_temperature(uint8_t *str);
void get_rfid_counter(uint8_t *str);
void increment_rfid_counter(uint8_t *str);
void reset_rfid_counter(uint8_t *str);


