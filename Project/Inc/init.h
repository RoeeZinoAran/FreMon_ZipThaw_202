/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_INIT_ENABLE_UART_1
  Unit: [N/A]
  Define Value: N/A
  Description:  Enable UART-1 initialize routine.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_INIT_ENABLE_UART_1

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_INIT_ENABLE_UART_2
  Unit: [N/A]
  Define Value: N/A
  Description:  Enable UART-2 initialize routine.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_INIT_ENABLE_UART_2

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_INIT_ENABLE_UART_3
  Unit: [N/A]
  Define Value: N/A
  Description:  Enable UART-3 initialize routine.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_INIT_ENABLE_UART_3

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void init(void);
void p_INIT_heating_pwm(void);
void p_INIT_rgb_leds(void);
void p_INIT_receiving_from_som(void);
