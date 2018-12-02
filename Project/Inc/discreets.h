
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: DOOR_OPEN
  Unit: [N/A]
  Define Value: (0u)
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define DOOR_OPEN     (0u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: DOOR_CLOSED
  Unit: [N/A]
  Define Value: 1u
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define DOOR_CLOSED   (1u)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

unsigned short get_som_comm_address(void);
void get_motor_index(void);
void get_door_state(void);
void rgb_leds(unsigned short red, unsigned short green, unsigned short blue);


