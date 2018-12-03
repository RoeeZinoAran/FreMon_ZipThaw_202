
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"
#include "string.h"
#include "rfid.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: RFID_STX
  Unit: [N/A]
  Define Value: 0x02u
  Description: UART RF TX start frame byte.
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_STX    (0x02u)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! 
  Define Name: RFID_EXT
  Unit: [N/A]
  Define Value: 0x03u
  Description: UART RF TX end frame byte.  
*/
/*--------------------------------------------------------------------------------------------------*/
#define RFID_EXT    (0x03u)

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: dummy
  Variable Type: unsigned short
  Unit: [N/A]
  Default value: 0
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile unsigned short dummy = 0;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: huart4
  Variable Type: UART_HandleTypeDef
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart4;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: uart_tx_callback
  Variable Type: unsigned short[5]
  Unit: [N/A]
  Default value: 
  Description: Callbacks for UART TX interfaces.
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short uart_tx_callback[5]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: 
  Variable Type: 
  Unit: [N/A]
  Default value: 
  Description: Callbacks for UART RX interfaces.
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short uart_rx_callback[5]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: from_rfid_string
  Variable Type: uint8_t [RFID_STRING_ARRAY_SIZE]
  Unit: [N/A]
  Default value: N/A.
  Description: RF - ID string handler.
*/
/*--------------------------------------------------------------------------------------------------*/
extern uint8_t from_rfid_string[RFID_STRING_ARRAY_SIZE]; 

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: rfid_task
  Variable Type: unsigned short
  Unit: [N/A]
  Default value: N/A.
  Description: RF-ID task number.
*/
/*--------------------------------------------------------------------------------------------------*/
unsigned short rfid_task;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup
\brief Interface function:
	The function shall read RF ID temreture 
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void get_rfid_temperature(uint8_t *str)
{
#if 0
	//   [STX]REARP00[ETX]U
#endif

	uint8_t rfid_read_temperature_string[] = {RFID_STX, 'R', 'E', 'A', 'G', 'T', RFID_EXT, 'D'};

	uart_tx_callback[RFID_COMM_UART_NUM] = 0;
	uart_rx_callback[RFID_COMM_UART_NUM] = 0;
	
	/* Prepare receiving data */
	str[0] = 0; /* first returned byte is ACK. We reset it in order not to preserve ACK from previous message (for case of failure). */
	HAL_UART_Receive_IT(&huart4, str, 18);
	
	/* Transmit and wait until transmission end */
	HAL_UART_Transmit(&huart4, rfid_read_temperature_string, 8, 10);
	
	rfid_task = READ_TEMPERATURE_RFID_TASK;
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup
\brief Interface function:
	The function shall get RF-ID counter.
\return Void
\DerivedDesc N/A.
\param uint8_t* str - Outcome string from RF-ID device.
	   Range: Not NULL.
*/
/*--------------------------------------------------------------------------------------------------*/
void get_rfid_counter(uint8_t* str)
{

	uint8_t rfid_counter_tx_string[] = {RFID_STX, 'R', 'E', 'A', 'G', 'C', RFID_EXT, 'S'}; // command is "REAGC" (ASCII string). 'S' at end is checksum

	uart_tx_callback[RFID_COMM_UART_NUM] = 0;
	uart_rx_callback[RFID_COMM_UART_NUM] = 0;
	
	/* Prepare receiving data */
	HAL_UART_Receive_IT(&huart4, str, 36);
	
	/* Transmit and wait until transmission end */
	HAL_UART_Transmit(&huart4, rfid_counter_tx_string, 8, 10);
	
	rfid_task = READ_COUNTER_RFID_TASK;
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup
\brief Interface function:
	The function shall increment counter of RF-ID device.
\return Void
\DerivedDesc N/A.
\param uint8_t* str - Outcome string from RF-ID device.
	   Range: Not NULL.
*/
/*--------------------------------------------------------------------------------------------------*/
void increment_rfid_counter(uint8_t* str)
{
//	unsigned short s1;
	uint8_t rfid_counter_tx_string[] = {RFID_STX, 'R', 'E', 'A', 'I', 'N', 'C', 'C', RFID_EXT, 'P'}; // command is "REAGC" (ASCII string). 'P' at end is checksum

	uart_tx_callback[RFID_COMM_UART_NUM] = 0;
	uart_rx_callback[RFID_COMM_UART_NUM] = 0;
	
	/* Prepare receiving data */
	HAL_UART_Receive_IT(&huart4, str, 11);
	
	/* Transmit and wait until transmission end */
	HAL_UART_Transmit(&huart4, rfid_counter_tx_string, 10, 10);
	
	rfid_task = INCREMENT_COUNTER_RFID_TASK;
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup
\brief Interface function:
	The function shall reset RF-ID device.
\return Void
\DerivedDesc N/A.
\param uint8_t* str - Outcome string from RF-ID device.
	   Range: Not NULL.
*/
/*--------------------------------------------------------------------------------------------------*/
void reset_rfid_counter(uint8_t* str)
{
//	unsigned short s1;

	uint8_t rfid_counter_tx_string[14]; 
	
	uart_tx_callback[RFID_COMM_UART_NUM] = 0;
	uart_rx_callback[RFID_COMM_UART_NUM] = 0;
	
	/* command is "REAGC" (ASCII string). 'A' at end is checksum */
	rfid_counter_tx_string[0] = RFID_STX;  
	strcat((char *)(&rfid_counter_tx_string[1]), "REARESETC");
	rfid_counter_tx_string[10] = RFID_EXT;
	rfid_counter_tx_string[11] = 'A';
	rfid_counter_tx_string[12] = 0;
	
	/* Prepare receiving data */
	HAL_UART_Receive_IT(&huart4, str, 17);
	
	/* Transmit and wait until transmission endif */
	HAL_UART_Transmit(&huart4, rfid_counter_tx_string, 13, 10);
	
	rfid_task = RESET_COUNTER_RFID_TASK;
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup
\brief Interface function:
	The function shall translatig the ASCII string retuned by RFID to integer in mC units.
 	Incase communication with RFID failed (bad ACK) then 99.9 C (99900 mC)
 	is returned.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
signed int rfid_temp_ascii_to_int(void)
{
	signed int i1;
	
	if (from_rfid_string[0] != 0x06) // if bad ACK from RFID
		return RFID_COMM_ERROR_RETURNED_TEMPERATURE; // return 99.9C  (99900 mC).
	
	i1  = (((int)((from_rfid_string[ 9] - '0'))) * 100000);
	i1 += (((int)((from_rfid_string[10] - '0'))) * 10000);
	i1 += (((int)((from_rfid_string[11] - '0'))) * 1000);
	i1 += (((int)((from_rfid_string[13] - '0'))) * 100); // index 13 because index 12 is decimal point
	
	if (from_rfid_string[8] == '-')
		i1 *= (-1);
	
	return i1;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UNUSED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#if 0

void get_rfid_page(uint8_t add, uint8_t *str)
{
	//   [STX]REARP00[ETX]U

	unsigned short s1;
	uint8_t rfid_read_page_string[] = {RFID_STX, 'R', 'E', 'A', 'R', 'P', 0, 0, RFID_EXT, 0};
	uint8_t crc = 0;

	rfid_read_page_string[6] = '0' + (add / 16); // address MSNibble in ASCII
	rfid_read_page_string[7] = '0' + (add % 16); // address LSNibble in ASCII
	
	// calculating CRC:
	// ---------------
	for(s1 = 0; s1 < 9; s1++) 
		crc ^= rfid_read_page_string[s1];
	rfid_read_page_string[9] = crc;
	
	
	// Prepare receiving data:
	// ----------------------
	HAL_UART_Receive_IT(&huart4, str, 18);
	
	// Transmit and wait until transmission end:
	// ----------------------------------------
	HAL_UART_Transmit(&huart4, rfid_read_page_string, 10, 10);

	
	// Wait until receiving end:
	// ------------------------
	s1 = 0;
//	while((uart_rx_callback[RFID_COMM_UART_NUM] == 0) && (s1 < 50000))
//		s1++;
//
//	uart_rx_callback[RFID_COMM_UART_NUM] = 0;
}
#endif

#if 0

void get_rfid_version(uint8_t *str)
{
	unsigned short s1;
	uint8_t rfid_version_tx_string[] = {RFID_STX, '1', '0', '0', '1', RFID_EXT, 0x01}; // command is "1001" (ASCII string). 0x01 at end is checksum

	// Prepare receiving data:
	// ----------------------
	HAL_UART_Receive_IT(&huart4, str, 17);
	
	// Transmit and wait until transmission end:
	// ----------------------------------------
	HAL_UART_Transmit(&huart4, rfid_version_tx_string, 7, 10);

	
	// Wait until receiving end:
	// ------------------------
	s1 = 0;
	while((uart_rx_callback[RFID_COMM_UART_NUM] == 0) && (s1 < 50000))
		s1++;

	uart_rx_callback[RFID_COMM_UART_NUM] = 0;
}

#endif
