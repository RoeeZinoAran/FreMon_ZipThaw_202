
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_HEATER_PWM_UNLIMITED_MAX_VALUE
  Unit: [N/A]
  Define Value: 10000u
  Description:  10,000 is the value that if sent to PWM register gives 100% duty cycle.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_HEATER_PWM_UNLIMITED_MAX_VALUE (10000u)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_HEATER_update_heaters_power(void);



