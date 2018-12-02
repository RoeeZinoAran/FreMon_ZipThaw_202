
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: LED_BUZZER_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define LED_BUZZER_OPCODE				8

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: TURN_LED_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define TURN_LED_ON_SUB_CODE			30

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: TURN_LED_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define TURN_LED_OFF_SUB_CODE			10

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEAT_AND_AGITATION_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEAT_AND_AGITATION_OPCODE		6	

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEATING_ELEMENT_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEATING_ELEMENT_ON_SUB_CODE     10

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEATING_ELEMENT_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEATING_ELEMENT_OFF_SUB_CODE	20

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: AGITATION_MOTOR_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define AGITATION_MOTOR_ON_SUB_CODE		30

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: AGITATION_MOTOR_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define AGITATION_MOTOR_OFF_SUB_CODE	40

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: TECHNICIAN_MODE_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define TECHNICIAN_MODE_OPCODE          5

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: TECHNICIAN_MODE_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define TECHNICIAN_MODE_ON_SUB_CODE     10

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: TECHNICIAN_MODE_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define TECHNICIAN_MODE_OFF_SUB_CODE    20

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: START_STOP_THAWING_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define DOOR_CLOSED_ERROR_BIT_NUM       4

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: START_THAWING_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define START_THAWING_SUB_CODE			40

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: STOP_THAWING_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define STOP_THAWING_SUB_CODE			50

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void serve_opcode(void);
void serve_start_stop_thawing_opcode(void);
void serve_led_buzzer_opcode(void);
void serve_agitation_and_heater_opcode(void);
void serve_technician_opcode(void);


