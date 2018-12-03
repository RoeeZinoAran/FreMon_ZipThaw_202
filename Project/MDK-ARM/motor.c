
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "motor.h"
#include "stm32l0xx_hal.h"
#include "discreets.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: hspi1
  Variable Type: SPI_HandleTypeDef
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: motor1092
  Variable Type: unsigned short
  Unit: [N/A]
  Default value: 0
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile unsigned short motor1092 = 0;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: L6470_spi_arg_len
  Variable Type: uint8_t [32]
  Unit: [N/A]
  Default value: N/A
  Description: A table that contains the number of data (argument only without the 'command')
               bytes of a command.
*/
/*--------------------------------------------------------------------------------------------------*/
const  uint8_t L6470_spi_arg_len[32] = 
{
	0, 3, 2, 3,    3, 2, 2, 2,  // 0 to  7
	2, 1, 1, 1,    1, 2, 1, 1,  // 8 to 0x0f
	1, 1, 1, 1,    1, 2, 1, 1,  // 0x10 to 0x17 
	2, 2, 0, 0,    0, 0, 0, 0   // 0x18 to 0x1f
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall update agitation motor with Hard - coded values. 
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void update_agitation_motor_speed(void)
{
	static unsigned short door_closed_time_cnt = 0;
#define DOOR_DEBOUNCH_CONSTANT  2
	
	if (system_state.door_state == DOOR_OPEN) /* If door is open then shut PWM off (0% duty cycle) */ 
	{
		/* Run. bit 0 of first byte (of four) is direction.  Last 20 bits are speed. */
		update_agitation_motor_register(0x50, 0x00, 0x00, 0x00); 
		door_closed_time_cnt = 0;
	}
	else	/* if door closed */ 
	{ 
		if (door_closed_time_cnt == DOOR_DEBOUNCH_CONSTANT) 
		{
			/* Run. bit 0 of first byte (of four) is direction.  Last 20 bits are speed. */
			update_agitation_motor_register(0x50, 0x00, 0x1c, 0x00); 
		}
		else 
		{
			if (door_closed_time_cnt < 0xffff) 
			{
				door_closed_time_cnt++;
			}
		}
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall initializing the L6470 stepper L6470 driver.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void init_L6470(void)
{
	
	update_agitation_motor_register(0, 0, 0, 0);
	update_agitation_motor_register(0, 0, 0, 0);
	update_agitation_motor_register(0, 0, 0, 0);
	update_agitation_motor_register(0, 0, 0, 0);

	/* 	Updating reset (Default) values.
   		Those lines are only for restoring the default (after reset) values of the registers */

	update_agitation_motor_register(0x01, 0x00, 0x00, 0x00);
	update_agitation_motor_register(0x02, 0x00, 0x00, 0x00);
	update_agitation_motor_register(0x03, 0x00, 0x00, 0x00);
	update_agitation_motor_register(0x04, 0x00, 0x00, 0x00);
	update_agitation_motor_register(0x05, 0x00, 0x8a, 0x00);
	update_agitation_motor_register(0x06, 0x00, 0x8a, 0x00);
	update_agitation_motor_register(0x07, 0x00, 0x41, 0x00);
	update_agitation_motor_register(0x08, 0x00, 0x00, 0x00);
	update_agitation_motor_register(0x09, 0x29, 0x00, 0x00);
	update_agitation_motor_register(0x0a, 0x29, 0x00, 0x00);
	update_agitation_motor_register(0x0b, 0x29, 0x00, 0x00);
	update_agitation_motor_register(0x0c, 0x29, 0x00, 0x00);
	update_agitation_motor_register(0x0d, 0x04, 0x08, 0x00);
	update_agitation_motor_register(0x0e, 0x19, 0x00, 0x00);
	update_agitation_motor_register(0x0f, 0x29, 0x00, 0x00);
	update_agitation_motor_register(0x10, 0x29, 0x00, 0x00);
	update_agitation_motor_register(0x11, 0x00, 0x00, 0x00);

#if 0
	update_agitation_motor_register(0x12, 0x00, 0x00, 0x00);
#endif
	
	update_agitation_motor_register(0x13, 0x08, 0x00, 0x00);
	update_agitation_motor_register(0x14, 0x40, 0x00, 0x00);
	update_agitation_motor_register(0x15, 0x00, 0x27, 0x00);
	update_agitation_motor_register(0x16, 0x07, 0x00, 0x00);
	update_agitation_motor_register(0x17, 0xff, 0x00, 0x00);
	update_agitation_motor_register(0x18, 0x2e, 0x88, 0x00);
   // End of restoring reset values of registers
	
	
	update_agitation_motor_register(0x16, 0x27,    0, 0); // STEP_MODE (bits 2:0 = 7 mean 1/128 step)
	update_agitation_motor_register(0x07, 0x00, 0x20, 0); // MAX_SPEED

#if 0
	update_agitation_motor_register(0x15, 0x03, 0xff, 0); // FS_SPD
#endif

	update_agitation_motor_register(0x15, 0x01, 0xff, 0); // FS_SPD
	update_agitation_motor_register(0x05, 0x03, 0xfd, 0); // ACC
	update_agitation_motor_register(0x06, 0x03, 0xfd, 0); // DEC
	update_agitation_motor_register(0x13, 0x0f, 0xfd, 0); // OCD_TH
	update_agitation_motor_register(0x18, 0x00, 0x10, 0); // CONFIG
	update_agitation_motor_register(0x08, 0x00, 0x02, 0); // MIN_SPEED
	update_agitation_motor_register(0x07, 0x03, 0xf0, 0); // MAX_SPEED
	update_agitation_motor_register(0x07, 0x03, 0xf0, 0); // MAX_SPEED

#if 0
	pdate_agitation_motor_register(0x0A, 0x70, 0x00, 0); // KVAL_RUN
	update_agitation_motor_register(0x0A, 0x86, 0x00, 0); // KVAL_RUN
#endif

	update_agitation_motor_register(0x0A, 0x96, 0x00, 0); // KVAL_RUN
	HAL_Delay(50);
	update_agitation_motor_register(0x50, 0x00, 0x1c, 0x00); // Run. bit 0 of first byte (of four) is direction.  Last 20 bits are speed.

#if 0
	update_agitation_motor_register(0x50, 0x00, 0x00, 0x00); // Run. bit 0 of first byte (of four) is direction.  Last 20 bits are speed.
#endif

}


/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall writing one command (data to one register) to motor driver (L6470) via SPI.
 	The function "knows" how many data bytes are used for each command (Address).
 	In case of a one byte data command,  d1  and  d2  are ignored. In case of
	a 2 bytes command d2 is ignored.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void update_agitation_motor_register(uint8_t add, uint8_t d0, uint8_t d1, uint8_t d2)
{
	uint8_t 		num_of_bytes; /* total number of bytes including the command */
	uint8_t 		str1[4];
	unsigned short 	s1;
	
	if (add >= 0x20)
	{
		num_of_bytes = 1 + 3;
	}
	else
	{
		num_of_bytes = 1 + L6470_spi_arg_len[add];
	}
	
	str1[0] = add;
	str1[1] = d0;
	str1[2] = d1;
	str1[3] = d2;
	
	
	for(s1 = 0; s1 < num_of_bytes; s1++) 
	{
		HAL_GPIO_WritePin(SPI_CS_n_GPIO_Port, SPI_CS_n_Pin, GPIO_PIN_RESET); 	/* Assert (active low) CS~ */
		HAL_SPI_Transmit(&hspi1, &str1[s1], 1, 70); 							/* Blocking mode */
		HAL_GPIO_WritePin(SPI_CS_n_GPIO_Port, SPI_CS_n_Pin, GPIO_PIN_SET); 		/* De-assert (active low) CS~ */
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall reading a register (via SPI) from the stepper L6470 driver.
	Data is returned in 'rx_data[]'.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void read_agitation_motor_register(uint8_t add, uint8_t rx_data[])
{

#if 0
	uint8_t num_of_bytes; // total number of bytes including the command
	uint8_t tx_data[4] = {0, 0, 0, 0};
	
	unsigned short s1;
	
	num_of_bytes = 1 + L6470_spi_arg_len[add];

	tx_data[0] = 0x20 | add;
	
	for(s1 = 0; s1 < num_of_bytes; s1++) {
		HAL_GPIO_WritePin(SPI_CS_n_GPIO_Port, SPI_CS_n_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi1, &tx_data[s1], &rx_data[s1], 1, 10);
	// Have to wait  @@@note  ???
		HAL_GPIO_WritePin(SPI_CS_n_GPIO_Port, SPI_CS_n_Pin, GPIO_PIN_SET);
	}
	HAL_Delay(5);
#endif

}

#if 0
	static volatile unsigned short wait_for_motor_spi_callback = 0;
	volatile unsigned short vol0 = 0;
	extern volatile unsigned short motor_spi_callback;
#endif
