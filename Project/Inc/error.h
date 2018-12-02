
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: DOOR_CLOSED_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define DOOR_CLOSED_ERROR_BIT_NUM                      (0u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: AGITATION_MOTOR_TIMO_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define AGITATION_MOTOR_TIMO_ERROR_BIT_NUM             (2u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: P3V3_VOLTAGE_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define P3V3_VOLTAGE_ERROR_BIT_NUM                     (3u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: P24V0_VOLTAGE_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define P24V0_VOLTAGE_ERROR_BIT_NUM                    (4u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEAT_ELEMENT_0_TEMP_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEAT_ELEMENT_0_TEMP_ERROR_BIT_NUM              (8u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: CUSHION_0_TEMP_SENSOR_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define CUSHION_0_TEMP_SENSOR_ERROR_BIT_NUM				(9u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: CUSHION_0_SAFETY_SENSOR_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define CUSHION_0_SAFETY_SENSOR_ERROR_BIT_NUM         	(10u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEAT_ELEMENT_1_TEMP_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEAT_ELEMENT_1_TEMP_ERROR_BIT_NUM				(11u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: CUSHION_1_TEMP_SENSOR_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define CUSHION_1_TEMP_SENSOR_ERROR_BIT_NUM				(12u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: CUSHION_1_SAFETY_SENSOR_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define CUSHION_1_SAFETY_SENSOR_ERROR_BIT_NUM			(13u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: RFID_SENSOR_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description: Sensor is bad or temperature is bad ?
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_SENSOR_ERROR_BIT_NUM						(14u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: IR_SENSOR_ERROR_BIT_NUM
  Unit: [N/A]
  Define Value: 
  Description: Sensor is bad or temperature is bad
*/
/*--------------------------------------------------------------------------------------------------*/
#define IR_SENSOR_ERROR_BIT_NUM							(15u)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void errors_top(void);
