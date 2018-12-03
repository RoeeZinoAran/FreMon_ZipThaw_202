
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_LED_BUZZER_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_LED_BUZZER_OPCODE				        (8)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_TURN_LED_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_TURN_LED_ON_SUB_CODE			      (30)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_TURN_LED_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_TURN_LED_OFF_SUB_CODE			      (10)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_HEAT_AND_AGITATION_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_HEAT_AND_AGITATION_OPCODE		    (6)	

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_HEATING_ELEMENT_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_HEATING_ELEMENT_ON_SUB_CODE     (10)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_HEATING_ELEMENT_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_HEATING_ELEMENT_OFF_SUB_CODE	  (20)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_AGITATION_MOTOR_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_AGITATION_MOTOR_ON_SUB_CODE		(30)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_AGITATION_MOTOR_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_AGITATION_MOTOR_OFF_SUB_CODE	(40)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_TECHNICIAN_MODE_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_TECHNICIAN_MODE_OPCODE        (5)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_TECHNICIAN_MODE_ON_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_TECHNICIAN_MODE_ON_SUB_CODE     (10)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_TECHNICIAN_MODE_OFF_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_TECHNICIAN_MODE_OFF_SUB_CODE    (20)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_START_STOP_THAWING_OPCODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_START_STOP_THAWING_OPCODE       (4)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_START_THAWING_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_START_THAWING_SUB_CODE			    (40)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_COMMSERV_STOP_THAWING_SUB_CODE
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_COMMSERV_STOP_THAWING_SUB_CODE			      (50)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_COMMSERV_serve_opcode(void);
void p_COMMSERV_serve_start_stop_thawing_opcode(void);
void p_COMMSERV_serve_led_buzzer_opcode(void);
void p_COMMSERV_serve_agitation_and_heater_opcode(void);
void p_COMMSERV_serve_technician_opcode(void);


