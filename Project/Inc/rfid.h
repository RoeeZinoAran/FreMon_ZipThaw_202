
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_RFID_STRING_ARRAY_SIZE
  Unit: [N/A]
  Define Value: 40
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_STRING_ARRAY_SIZE						(40)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_RFID_TIM0
  Unit: [mSec]
  Define Value: 300
  Description:  Time in mS that if RFID does not answer then it is retried
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_TIM0									(300)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_RFID_COMM_ERROR_RETURNED_TEMPERATURE
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_COMM_ERROR_RETURNED_TEMPERATURE		(99900)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_RFID_NO_ANSWER_RETURNED_TEMPERATURE
  Unit: [N/A]
  Define Value: 10000u
  Description:  Temperatures (in mC) that are returned when there are RFID problems
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_NO_ANSWER_RETURNED_TEMPERATURE			(88800)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: 
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_RESET_COUNTER_RFID_TASK						(0)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_RFID_INCREMENT_COUNTER_RFID_TASK
  Unit: [N/A]
  Define Value: 1
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_INCREMENT_COUNTER_RFID_TASK					(1)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: 
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_READ_COUNTER_RFID_TASK						(2)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: 
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_RFID_READ_TEMPERATURE_RFID_TASK					(3)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void get_rfid_version(uint8_t *str);
void get_rfid_page(uint8_t add, uint8_t *str);
signed int rfid_temp_ascii_to_int(void);
void p_RFID_get_temperature(uint8_t *str);
void p_RFID_get_counter(uint8_t *str);
void p_RFID_increment_counter(uint8_t *str);
void p_RFID_reset_counter(uint8_t *str);


