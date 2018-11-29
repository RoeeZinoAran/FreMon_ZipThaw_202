
#include "stm32l0xx_hal.h"

#include "miscel.h"

// --------------------------------------------------------------------------
// Defining the values for  'system_state.tasks_in_progress' field:
// --------------------------------------------------------------------------
#define STANDBY_TASK_BIT                (1 <<  0)
#define THAWING_TASK_BIT       					(1 <<  1)

#define AFTER_THAWING_TASK_BIT     			(1 <<  2)
// Thawing ended (normally or stopped by user) but door is still closed

#define TECHNICIAN_TASK_BIT        			(1 << 15)
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Data structure for describing the system's state. (measurements, commnucation etc...):
-------------------------------------------------------------------------- */
enum comm_rx_status { // Values for  --- 'system_state.som_comm_state' ---   field:
   TRANSMITTING, RECEIVING_MESSAGE_HEADER, RECEIVING_MESSAGE_BODY, COMPLETE_MESSAGE_RECEIVED, ANALYZING, FAIL
};
// --------------------------------------------------------------------------
extern struct sys_state {
	unsigned int   			heater_current[2];	// Heater current in mAmps
	  signed int   			ntc_temp[2];     		// in mC
	unsigned int   			weight;          		// in grams
	unsigned int   			p3v3;            		// in mV
	unsigned int   			p24v0;           		// in mV
	
	  signed int   			rfid_temp;       		// in mC
	  signed int   			ir_temp;         		// in mC
	  signed int   			pcb_temp;        		// in mC
     
  unsigned short			motor_index;     		// 1 means in index
	unsigned short			door_state;      		// 1 means door closed
	
	unsigned short			leds_state;        	// Bit 0 R,  bit 1 Green,  bit 2 Blue
	
	unsigned short			cushion_duty_cycle[2]; // 10,000 means 100% PWM.
	
  unsigned short 			my_som_comm_add; 		// The specific address of this board (PCB) as defined by jumpers.
	enum comm_rx_status	som_comm_state;  		// Tells if we are waiting for receiving message, or waiting that transmit to SOM will end etc...
	
	unsigned short			tasks_in_progress;	// Each bit represents one task. See defines that define each bit
	
	unsigned short			after_thawing_timer;// Counts time in seconde from end of thawing until door is opened.
	unsigned short      progress;           // Progress of the current task. 0 for unsupported task. 
																					// represent percentage i.e. 0 to 100 except for 'AFTER_THAWING'
																					// stage in which it counts seconds from stage start.
	
} system_state;
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Data structure for the data of the header as received from SOM's last message to MCU:
-------------------------------------------------------------------------- */
enum from_som_result { // Values for  --- 'from_som_message_state.result' ---   field:
	MESSAGE_OK, NOT_ADDRESSED_TO_ME, CRC_ERROR, PREAMBLE_ERROR
};
// --------------------------------------------------------------------------
extern struct from_som_msg {
				uint16_t 				message_length;
				uint16_t 				message_id;
				uint8_t  				sender_id;
				uint8_t  				receiver_id;
				uint8_t  				opcode_type;
				uint8_t  				opcode_sub_type;
	enum	from_som_result	result; // tells the status of thelast received message (CRC_ERROR, PREAMBLE_ERROR, MESSAGE_OK etc...)
} from_som_message_state;
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
//extern struct prog_stat {
//	unsigned short standby_progress;
//	unsigned short thawing_progress;
//} progress_status;
// --------------------------------------------------------------------------


//
