
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_DISCREETS_DOOR_OPEN
  Unit: [N/A]
  Define Value: (0u)
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_DISCREETS_DOOR_OPEN     (0u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_DISCREETS_DOOR_CLOSED
  Unit: [N/A]
  Define Value: 1u
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_DISCREETS_DOOR_CLOSED   (1u)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

unsigned short p_DISCREETS_get_som_comm_address(void);
void p_DISCREETS_get_motor_index(void);
void p_DISCREETS_get_door_state(void);
void p_DISCREETS_rgb_leds(unsigned short red, unsigned short green, unsigned short blue);


