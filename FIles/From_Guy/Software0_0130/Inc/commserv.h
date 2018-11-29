
void serve_opcode(void);

void serve_start_stop_thawing_opcode(void);
void serve_led_buzzer_opcode(void);
void serve_agitation_and_heater_opcode(void);
void serve_technician_opcode(void);


#define LED_BUZZER_OPCODE								8
#define TURN_LED_ON_SUB_CODE						30
#define TURN_LED_OFF_SUB_CODE						10


#define HEAT_AND_AGITATION_OPCODE				6	
#define HEATING_ELEMENT_ON_SUB_CODE     10
#define HEATING_ELEMENT_OFF_SUB_CODE    20
#define AGITATION_MOTOR_ON_SUB_CODE     30
#define AGITATION_MOTOR_OFF_SUB_CODE    40

         
#define TECHNICIAN_MODE_OPCODE          5
#define TECHNICIAN_MODE_ON_SUB_CODE     10
#define TECHNICIAN_MODE_OFF_SUB_CODE    20

#define START_STOP_THAWING_OPCODE       4
#define START_THAWING_SUB_CODE          40
#define STOP_THAWING_SUB_CODE           50
