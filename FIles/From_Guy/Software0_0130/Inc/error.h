
#define DOOR_CLOSED_ERROR_BIT_NUM                      0
#define AGITATION_MOTOR_TIMO_ERROR_BIT_NUM             2
#define P3V3_VOLTAGE_ERROR_BIT_NUM                     3
#define P24V0_VOLTAGE_ERROR_BIT_NUM                    4

#define HEAT_ELEMENT_0_TEMP_ERROR_BIT_NUM              8
#define CUSHION_0_TEMP_SENSOR_ERROR_BIT_NUM            9
#define CUSHION_0_SAFETY_SENSOR_ERROR_BIT_NUM         10

#define HEAT_ELEMENT_1_TEMP_ERROR_BIT_NUM             11
#define CUSHION_1_TEMP_SENSOR_ERROR_BIT_NUM           12
#define CUSHION_1_SAFETY_SENSOR_ERROR_BIT_NUM         13

#define RFID_SENSOR_ERROR_BIT_NUM                     14
// @@@note  Sensor is bad or temperature is bad ?

#define IR_SENSOR_ERROR_BIT_NUM                       15
// @@@note  Sensor is bad or temperature is bad ?


void errors_top(void);
