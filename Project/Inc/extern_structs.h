
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "miscel.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: STANDBY_TASK_BIT
  Unit: [N/A]
  Define Value: (1 <<  0)
  Description: Stand-by Task field in 'system_state.tasks_in_progress'.
*/
/*--------------------------------------------------------------------------------------------------*/
#define STANDBY_TASK_BIT				(1 <<  0)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: THAWING_TASK_BIT
  Unit: [N/A]
  Define Value: (1 <<  1)
  Description:  Thaing task field in 'system_state.tasks_in_progress'.
*/
/*--------------------------------------------------------------------------------------------------*/
#define THAWING_TASK_BIT				(1 <<  1)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: AFTER_THAWING_TASK_BIT
  Unit: [N/A]
  Define Value: (1 <<  2)
  Description:  After Thawing field in 'system_state.tasks_in_progress'.
*/
/*--------------------------------------------------------------------------------------------------*/
#define AFTER_THAWING_TASK_BIT			(1 <<  2)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: TECHNICIAN_TASK_BIT
  Unit: [N/A]
  Define Value: (1 << 15)
  Description: Technician task field in 'system_state.tasks_in_progress'.
*/
/*--------------------------------------------------------------------------------------------------*/
#define TECHNICIAN_TASK_BIT				(1 << 15)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ENUMS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$ENUMS$-------------------------------------------------------------------------------------------*/
/*
  Enum Name: comm_rx_status
  Description: Data structure for describing the system's state. (measurements, commnucation etc...).
               Values for  --- 'system_state.som_comm_state' fields.
*/
/*--------------------------------------------------------------------------------------------------*/
enum comm_rx_status
{ // 
   TRANSMITTING, 
   RECEIVING_MESSAGE_HEADER, 
   RECEIVING_MESSAGE_BODY, 
   COMPLETE_MESSAGE_RECEIVED, 
   ANALYZING, 
   FAIL
};

/*$ENUMS$-------------------------------------------------------------------------------------------*/
/*
  Enum Name: from_som_result
  Description: Data structure for the data of the header as received from SOM's last message to MCU.
               Values for  --- 'from_som_message_state.result' fields.
*/
/*--------------------------------------------------------------------------------------------------*/
enum from_som_result 
{
	MESSAGE_OK, 
	NOT_ADDRESSED_TO_ME, 
	CRC_ERROR, 
	PREAMBLE_ERROR
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% STRUCTURES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$STRUCTURES$-----------------------------------------------------------------------------------------*/
/*
  Structure Type: sys_state
  Names and Description: 
  	   heater_current - Heater current in mAmps | 
       ntc_temp - in mC | 
       weight - in grams |
       p3v3 - in mV | 
       p24v0 - in mV | 
       ntc_temp - in mC | 
       weight - in grams |
       rfid_temp - in mC |
       ir_temp -  in mC |
	     pcb_temp - in mV | 
       motor_index - 1 means in index | 
       door_state - 1 means door closed |
       leds_state - Bit 0 R,  bit 1 Green,  bit 2 Blue |
       cushion_duty_cycle -  10,000 means 100% PWM. |
	     my_som_comm_add - The specific address of this board (PCB) as defined by jumpers | 
       som_comm_state - Tells if we are waiting for receiving message, or waiting that transmit to SOM will end etc... | 
       tasks_in_progress - Each bit represents one task. See defines that define each bit |
       after_thawing_timer - Counts time in seconde from end of thawing until door is opened. |
       progress -  Progress of the current task. 0 for unsupported task. 
                   represent percentage i.e. 0 to 100 except for 'AFTER_THAWING'
                   stage in which it counts seconds from stage start. |

*/
/*--------------------------------------------------------------------------------------------------*/
extern struct sys_state 
{
	unsigned int   			heater_current[2];	
	signed int   			  ntc_temp[2];     		
	unsigned int   			weight;          		
	unsigned int   			p3v3;            		
	unsigned int   			p24v0;           		
 	signed int   			  rfid_temp;       		
  signed int   			  ir_temp;         		
  signed int   			  pcb_temp;        		
  unsigned short			motor_index;     		
	unsigned short			door_state;      		
	unsigned short			leds_state;        	
	unsigned short			cushion_duty_cycle[2]; 
  unsigned short 			my_som_comm_add; 		
	enum comm_rx_status		som_comm_state;  		 
	unsigned short			tasks_in_progress;	
	unsigned short			after_thawing_timer; 
  unsigned short      progress;            
} system_state;

/*$STRUCTURES$-----------------------------------------------------------------------------------------*/
/*
  Structure Type: from_som_msg
  Names and Description: 
  	   message_length -  | 
       message_id -  | 
       sender_id -  |
       receiver_id -  | 
       opcode_type -  | 
       opcode_sub_type -  | 
       result - tells the status of thelast received message (CRC_ERROR, PREAMBLE_ERROR, MESSAGE_OK etc...).       
*/
/*--------------------------------------------------------------------------------------------------*/
extern struct from_som_msg 
{
	uint16_t 				message_length;
	uint16_t 				message_id;
	uint8_t  				sender_id;
	uint8_t  				receiver_id;
	uint8_t  				opcode_type;
	uint8_t  				opcode_sub_type;
	enum from_som_result	result;
} from_som_message_state;


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UNUSED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#if 0
extern struct prog_stat {
	unsigned short standby_progress;
	unsigned short thawing_progress;
} progress_status;

#endif


