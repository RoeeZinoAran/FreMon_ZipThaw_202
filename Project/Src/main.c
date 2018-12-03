
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "main.h"
#include "stm32l0xx_hal.h"

/* USER CODE BEGIN Includes */

#include "host_from_comm.h"
#include "host_to_comm.h"
#include "ADC.h"
#include "heater.h"
#include "motor.h"
#include "rfid.h"
#include "board_temp.h"
#include "ir_temp.h"
#include "error.h"
#include "discreets.h"
#include "init.h"
#include "technician.h"
#include "pid.h"
#include "progress.h"
#include "main.h"
#include "structs.h"



#if 0
  #define SEND_TO_TERMINAL 1
#endif

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;


TIM_OC_InitTypeDef PWM_timer2_OC_params; // structure for controlling PWMs' duty cycles
TIM_OC_InitTypeDef PWM_timer3_OC_params; // structure for controlling PWMs' duty cycles

volatile unsigned short debug_var0;
volatile unsigned short uart_tx_callback[5] = {0, 0, 0, 0, 0}; // {0xffff, 0xffff, 0xffff, 0xffff, 0xffff}; // 5 indexes for UART0 to UART4. 0xffff indicates after power on
volatile unsigned short uart_rx_callback[5] = {0, 0, 0, 0, 0}; // {0xffff, 0xffff, 0xffff, 0xffff, 0xffff}; // 5 indexes for UART0 to UART4

// @@@note: See at main.h defines of comm numbers. Ensure that they fit numbers as deined by CUBE !.

volatile unsigned short adc_conversion_callback   = 0xffff; // 0xffff for indicating after power on
volatile unsigned short ir_conversion_callback    = 0xffff; // 0xffff for indicating after power on
volatile unsigned short pcb_temperature_callback  = 0xffff; // 0xffff for indicating after power on

//volatile unsigned short motor_spi_callback = 0xffff;

volatile unsigned short after_thawing_time = 0;
// counting the time in seconds from end of thawing until door is open.

volatile unsigned int   main_loop_tasks = 0;
// Every bits of this variable when set indicates that a task has to be
// performed in next main loop.  See defines in main.h
// ----------------------------------------------------------------

unsigned short safety_error = 0;
unsigned short rfid_count = 0;

extern uint32_t adc_raw_results[NUM_OF_ADC_CHANNELS];
extern uint8_t som_tx_buffer[MAX_MESSAGE_LENGTH_TO_SOM + 5];

extern volatile unsigned short vol0;

extern unsigned short rfid_task;
uint8_t from_rfid_string[RFID_STRING_ARRAY_SIZE] = {9, 9, 9}; 
// contain the entire string returned by the RFID device when it answers a temperature 
// measurement request.

uint8_t pcb_temperature_string[2];
// contain the entire string returned by the board (PCB) temperature measurement device.

extern uint8_t som_rx_buffer[MAX_MESSAGE_LENGTH];

volatile unsigned short no_somm_message_timer = 0;
volatile unsigned short motor_index_timer = 0;
volatile unsigned short delay_var0 = 0;


volatile unsigned short timer_20mS_period  = 0; // incremented every   1 mSec, and reset every  20 mSec.
volatile unsigned short timer_100mS_period = 0; // incremented every  20 mSec, and reset every 100 mSec.
volatile unsigned short timer_1S_period    = 0; // incremented every 100 mSec, and reset every   1  Sec.

//volatile unsigned short one_second_period_timer = 0;
// incremented once every 100mSec, reset every 1 second.

volatile unsigned short one_second_passed_bits = 0; 
// made 0xffff every second. Every task that reads this variable can reset one bit of this variable after task run


// --------------------------------------------------------------------------
//volatile unsigned short ten_mili_second_period_timer = 0;
// free running, (incremented at every systick), reset every 10 mili second.

//volatile unsigned short ten_mili_seconds_counter = 0;
// Count modulo 10 (0 to 9), changed every 10 mSecond so overrun every 100 mSec.

//volatile unsigned short ten_mili_second_passed_bits = 0;
// One of the 10 LSBits is set every 10 mili second:
// Bit 0 is set after 10 mSec, bit 1 after 20 mSec... bit 9 after 100mSec.
// Every task that reads this variable can reset one bit of this variable after
// that task run.
// Used to divide a 100mSec cycle to 10 time periods of 10mSec, so each task
// can run every 100mSec but at it's specific 10mSec time slot.
// For example: ADC can run at 1st. 10mSec,  RFID measure at next 10mSec etc...
// In this way, since not all tasks run continously at once, the main loop
// last much shorter time because that at every main loop on;y one (or few)
// tasks run and not all the tasks.
// --------------------------------------------------------------------------

//volatile unsigned short tenth_second_passed = 0; // Made 1 by systick every 100 mSec.

/* USER CODE END PV */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOCAL DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART4_UART_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

void HAL_SYSTICK_Callback(void)
{
	static unsigned short s1 = 0;
	
	if (no_somm_message_timer < 0xfff0)
		no_somm_message_timer++;
	
	if (motor_index_timer < 0xfff0)
		motor_index_timer++;

	timer_20mS_period++;
	if (timer_20mS_period >= 20) {
		timer_20mS_period = 0;
		timer_100mS_period++;
		if (timer_100mS_period == 5) {
			timer_100mS_period = 0;
			timer_1S_period++;
			if (timer_1S_period == 10) {
				timer_1S_period = 0;
				one_second_passed_bits = 0xffff;
				s1 = 1 - s1; // LED blinking
				if (s1)
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			}
		}
	}
	
	
	// Tasks allocation:
	// ----------------
	if (timer_20mS_period == 0) { // at start of every 20 mSec
		main_loop_tasks |= ADC_MAIN_LOOP_TASK_BIT;
	}
	else if (timer_20mS_period == 4) { // at 5th. mS of every 20 mSec
		main_loop_tasks |= STAGE_UPDATE_MAIN_LOOP_TASK_BIT;
	}
	else if (timer_20mS_period == 8) { // at 9th. mS of every 20 mSec
		main_loop_tasks |= SOM_SERVICE_MAIN_LOOP_TASK_BIT;
	}
	else if (timer_20mS_period ==12) { // at 13th. mS of every 20 mSec
		if (timer_100mS_period == 0) {
			main_loop_tasks |= IR_TEMP_MAIN_LOOP_TASK_BIT;
		}
		else if (timer_100mS_period == 1) {
			main_loop_tasks |= DOOR_CHECK_MAIN_LOOP_TASK_BIT;
			main_loop_tasks |= SOM_ADD_UPDATE_MAIN_LOOP_TASK_BIT;
		}
		else if (timer_100mS_period == 2) {
			main_loop_tasks |= MOTOR_MAIN_LOOP_TASK_BIT;
		}
		else if (timer_100mS_period == 3) {
			if (timer_1S_period == 0) {
				main_loop_tasks |= PID_CALC_MAIN_LOOP_TASK_BIT;
			}
			else if (timer_1S_period == 4) {
				main_loop_tasks |= HEATER_UPDATE_MAIN_LOOP_TASK_BIT;
			}
			else if (timer_1S_period == 6) {
				main_loop_tasks |= PCB_TEMP_MAIN_LOOP_TASK_BIT;
			}
			else if (timer_1S_period == 7) {
				main_loop_tasks |= DEBUG_TERMINAL_MAIN_LOOP_TASK_BIT;
			}
			else if (timer_1S_period == 8) { // values of this 100mS timer are 0 to 4. Only 5 values as there are 5 slots of 20mS in 100mS
				main_loop_tasks |= RFID_TEMP_MAIN_LOOP_TASK_BIT;
			}
		}
	}
}
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1)
		adc_conversion_callback = 1;
}
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
		if (hi2c->Instance == I2C2) // PCB Temperature
			pcb_temperature_callback = 1;
}
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
/*
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi1)
{
	if (hspi1->Instance == SPI1)
		motor_spi_callback = 1;
		HAL_GPIO_WritePin(SPI_CS_n_GPIO_Port, SPI_CS_n_Pin, GPIO_PIN_SET); 
}
*/
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
		if (hi2c->Instance == I2C1) // IR
			ir_conversion_callback = 1;
}
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) // debug channel
		uart_tx_callback[1] = 1;
	
	if (huart->Instance == USART2) { // If Transmission to SOM ended
		uart_tx_callback[2] = 1;
		debug_var0 = 10;
	}
	
	if (huart->Instance == USART4) // RFID
		uart_tx_callback[4] = 1;
}	
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
		uart_rx_callback[1] = 1;
	
	
	// If data (header or body) was received from SOM:
	// ----------------------------------------------
	if (huart->Instance == USART2) { // Received message from SOM
		debug_var0 = 11;
		
		// if just received message header:
		// -------------------------------
		if (system_state.som_comm_state == RECEIVING_MESSAGE_HEADER) { 
			debug_var0 = 12;
			system_state.som_comm_state = RECEIVING_MESSAGE_BODY;
			from_som_message_state.message_length = ((uint16_t)som_rx_buffer[NUM_OF_HEADER_BYTES - 1]);
			if (from_som_message_state.message_length >= (MAX_MESSAGE_LENGTH - 2))
				from_som_message_state.message_length = NUM_OF_HEADER_BYTES + 5;
//			if (from_som_message_state.message_length == 128)
//				ia11111++; // for debug only
//			if (from_som_message_state.message_length == 31)
//				ia22222++; // for debug only
			
			// Checking that header is OK:
			// --------------------------
			if ((som_rx_buffer[0] != '$') || (som_rx_buffer[1] != '$')) { // If error since header does not start with "$$"
				init_receiving_from_som();
				return;
			}
			
			// activate DMA to receive message body:
			// ------------------------------------
			debug_var0 = 13;
			HAL_UART_Receive_DMA(&huart2, &som_rx_buffer[NUM_OF_HEADER_BYTES], from_som_message_state.message_length - NUM_OF_HEADER_BYTES); 
			debug_var0 = 14;
		}
		
		// if just received message body:
		// -----------------------------
		else if (system_state.som_comm_state == RECEIVING_MESSAGE_BODY) { 
			debug_var0 = 15;
			system_state.som_comm_state = COMPLETE_MESSAGE_RECEIVED;
		}
		else {
			debug_var0 = 16;
			system_state.som_comm_state = FAIL;
		}
	}
	// ----------------------------------------------
	
	if (huart->Instance == USART4) {
		uart_rx_callback[RFID_COMM_UART_NUM] = 1;
	}
}	
// --------------------------------------------------------------------------



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART4_UART_Init();
  /* USER CODE BEGIN 2 */


	init();

/* --------------------------------------------------------------------------
 Initializing stepper motor driver
-------------------------------------------------------------------------- */
init_L6470();

	// for debug only:
  // --------------
/*
	read_agitation_motor_register(0x07, str11111);
	read_agitation_motor_register(0x15, str11111);
	read_agitation_motor_register(0x06, str11111);
*/
  // --------------
	
// --------------------------------------------------------------------------


	// Preparing receiving header of the first message after reset:
	// -----------------------------------------------------------
	no_somm_message_timer = 0;
	HAL_UART_Receive_DMA(&huart2, som_rx_buffer, NUM_OF_HEADER_BYTES);
	system_state.som_comm_state = RECEIVING_MESSAGE_HEADER;
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
				
		// if just ended transmission to SOM:
		// ---------------------------------
		if (uart_tx_callback[SOM_COMM_UART_NUM]) { 
			uart_tx_callback[SOM_COMM_UART_NUM] = 0;
			init_receiving_from_som(); // Prepare for receiving new message from SOM
		}

		
		get_motor_index();
					
			
		// If no message from SOM for too long time (time out):
		// ---------------------------------------------------
		if (no_somm_message_timer >= NO_SOM_MESSAGE_TIMO) {
			debug_var0 = 18;
//			MX_USART2_UART_Init();
			system_state.som_comm_state = FAIL;
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
//		HAL_UART_Receive(&huart2,som_rx_buffer, MAX_MESSAGE_LENGTH_FROM_SOM, 5); // Blocking function for empty UART buffer
			init_receiving_from_som();
		}
		
		
//		get_rfid_version(str11111);

			
		// Getting my SOM comm. address: 
		// ----------------------------
		if (main_loop_tasks & SOM_ADD_UPDATE_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 19;
			main_loop_tasks &= (~SOM_ADD_UPDATE_MAIN_LOOP_TASK_BIT);
			debug_var0 = 22;
			system_state.my_som_comm_add = get_som_comm_address();
		}
			
		// ADCs:
		// ----
		if (main_loop_tasks & ADC_MAIN_LOOP_TASK_BIT) {
			main_loop_tasks &= (~ADC_MAIN_LOOP_TASK_BIT);
			
			// If NTC seems disconnected:
			// -------------------------
//		if ((adc_raw_results[NTC_IN_ADC_CHANNEL] < 340) || (adc_raw_results[NTC_OUT_ADC_CHANNEL] < 340)) { 
			if ((system_state.ntc_temp[CUSHION0] < (-30)) || (system_state.ntc_temp[CUSHION1] < (-30))) { 
				translate_adc(); // for displaying the errorenous values
				if (safety_error < 1000) {
					safety_error += 5;
				}
				else { // safety_error >= 1000
					debug_var0 = 60;
					update_heaters_power();
					debug_var0 = 61;
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
				}
			}
			
			// if ADC  reading does Not indicate disconnected sensor:
			// -----------------------------------------------------
			else { 
				if ((safety_error >= 2) && (safety_error < 1000)){
					safety_error -= 2;
					debug_var0 = 62;
				}
			
				debug_var0 = 20;
			
				if (adc_conversion_callback) {
					adc_conversion_callback = 0;
					debug_var0 = 63;
				} // @@@wr error code if no call back
				
			}
			debug_var0 = 21; 
			translate_adc();
			debug_var0 = 74;
			ADC_go();
		}
		
			
		// PCB temperature:
		// ---------------
		if (main_loop_tasks & PCB_TEMP_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 23;
			main_loop_tasks &= (~PCB_TEMP_MAIN_LOOP_TASK_BIT);
			if (pcb_temperature_callback) {
				pcb_temperature_callback = 0;
				debug_var0 = 24;
			 // @@@wr error code
				system_state.pcb_temp = interpret_pcb_temperature();
				debug_var0 = 25;
				get_board_temperature(pcb_temperature_string);
				debug_var0 = 70;
			}
		}
		
		
			
		// IR temperature sensor:
		// ---------------------
		if (main_loop_tasks & IR_TEMP_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 26;
			main_loop_tasks &= (~IR_TEMP_MAIN_LOOP_TASK_BIT);
			if (ir_conversion_callback) {
				ir_conversion_callback = 0;
				debug_var0 = 27;
				system_state.ir_temp = interpret_ir_temperature();
				debug_var0 = 28;
				get_ir_temperature();
			}
		}
		
		
			
		// RFID temperature sensor:
		// -----------------------
		if (main_loop_tasks & RFID_TEMP_MAIN_LOOP_TASK_BIT) {
			main_loop_tasks &= (~RFID_TEMP_MAIN_LOOP_TASK_BIT);
			rfid_count++;
			if (rfid_count >= 3) {
				rfid_count = 0;
			
				debug_var0 = 29;
				if (uart_rx_callback[RFID_COMM_UART_NUM]) { // If was answer (any answer) from RFID
					uart_rx_callback[RFID_COMM_UART_NUM] = 0;
					debug_var0 = 31;
					switch(rfid_task) {
						case READ_TEMPERATURE_RFID_TASK:
							debug_var0 = 32;
							system_state.rfid_temp = rfid_temp_ascii_to_int();
							debug_var0 = 33;
							get_rfid_temperature(from_rfid_string);
							debug_var0 = 34;
							break;
						case READ_COUNTER_RFID_TASK:
							debug_var0 = 35;
							get_rfid_temperature(from_rfid_string); // @@@note changed to temperature reading
							debug_var0 = 36;
							break;
						default:
							debug_var0 = 81;
							get_rfid_temperature(from_rfid_string); // @@@note changed to temperature reading
							debug_var0 = 82;
							break;
					}
				}
				else { // If was no CallBack
					debug_var0 = 81;
					system_state.rfid_temp = RFID_NO_ANSWER_RETURNED_TEMPERATURE; // 88.8 C indicate no answer from RFID
					get_rfid_temperature(from_rfid_string);
					debug_var0 = 82;
				}
			}
		}
			

		// Check the door state:
		// --------------------
		if (main_loop_tasks & DOOR_CHECK_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 37;
			main_loop_tasks &= (~DOOR_CHECK_MAIN_LOOP_TASK_BIT);
			get_door_state();
			debug_var0 = 38;
		}



#ifdef SEND_TO_TERMINAL
		// If data transmision to debug UART is compleeted:
		// -----------------------------------------------
		if (main_loop_tasks & DEBUG_TERMINAL_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 39;
			main_loop_tasks &= (~DEBUG_TERMINAL_MAIN_LOOP_TASK_BIT);
			uart_tx_callback[DEBUG_COMM_UART_NUM] = 0;
				send_debug_values();
			debug_var0 = 40;
		}
#endif
			
		// Stage and Progress:
		// ------------------
		if (main_loop_tasks & STAGE_UPDATE_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 41;
			main_loop_tasks &= (~STAGE_UPDATE_MAIN_LOOP_TASK_BIT);
			stage_and_rpogress();
			debug_var0 = 42;
		}
		
		
		// Calculate PID:
		// -------------
		if (main_loop_tasks & PID_CALC_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 43;
			main_loop_tasks &= (~PID_CALC_MAIN_LOOP_TASK_BIT);
			calc_pid();
			debug_var0 = 44;
		}
		
		
		// Update PWMs of heating elements:
		// -------------------------------
		if (main_loop_tasks & HEATER_UPDATE_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 45;
			main_loop_tasks &= (~HEATER_UPDATE_MAIN_LOOP_TASK_BIT);
			update_heaters_power();
			debug_var0 = 46;
		}
		
		
		// Update agitation motor speed and check ADC calibration:
		// ------------------------------------------------------
		if (main_loop_tasks & MOTOR_MAIN_LOOP_TASK_BIT) {
			debug_var0 = 47;
			main_loop_tasks &= (~MOTOR_MAIN_LOOP_TASK_BIT);
			update_agitation_motor_speed();
			debug_var0 = 48;
			
			
			calib_adc();
			debug_var0 = 49;
		}
		
		
		// Serve communication with host for the case a message was received:
		// -----------------------------------------------------------------
//		if (main_loop_tasks & SOM_SERVICE_MAIN_LOOP_TASK_BIT) {
//			debug_var0 = 50;
//			main_loop_tasks &= (~SOM_SERVICE_MAIN_LOOP_TASK_BIT);
			serve_host_comm();
//			debug_var0 = 51;
//		}
		
		
		// Check and treat errors:
		// ----------------------
		errors_top();
		debug_var0 = 52;
		
		
		// Watchdog triggerring:
		// --------------------
		HAL_GPIO_WritePin(HOW_HOW_GPIO_Port, HOW_HOW_Pin, GPIO_PIN_SET);
		delay_var0++;
		delay_var0++;
		HAL_GPIO_WritePin(HOW_HOW_GPIO_Port, HOW_HOW_Pin, GPIO_PIN_RESET);


  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  } 
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC init function */
static void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = ENABLE;
  hadc.Init.Oversample.Ratio = ADC_OVERSAMPLING_RATIO_16;
  hadc.Init.Oversample.RightBitShift = ADC_RIGHTBITSHIFT_4;
  hadc.Init.Oversample.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_6;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x202029A8;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00707CBB;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 300;
  htim3.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim3.Init.Period = 10000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIMEx_RemapConfig(&htim3, TIM3_TI1_GPIO) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart2, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART4 init function */
static void MX_USART4_UART_Init(void)
{

  huart4.Instance = USART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  /* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HeaterEnChamber_GPIO_Port, HeaterEnChamber_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6|LED_B_Pin|LED_G_Pin|SPI_CS_n_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HOW_HOW_GPIO_Port, HOW_HOW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : HeaterEnChamber_Pin PE6 LED_B_Pin LED_G_Pin 
                           SPI_CS_n_Pin */
  GPIO_InitStruct.Pin = HeaterEnChamber_Pin|GPIO_PIN_6|LED_B_Pin|LED_G_Pin 
                          |SPI_CS_n_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : HOW_HOW_Pin */
  GPIO_InitStruct.Pin = HOW_HOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HOW_HOW_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MOTOR_INDEX_Pin PB13 PB14 */
  GPIO_InitStruct.Pin = MOTOR_INDEX_Pin|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DOOR_STATE_Pin PD12 PD13 */
  GPIO_InitStruct.Pin = DOOR_STATE_Pin|GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : COMM_ADD0_Pin COMM_ADD1_Pin COMM_ADD2_Pin */
  GPIO_InitStruct.Pin = COMM_ADD0_Pin|COMM_ADD1_Pin|COMM_ADD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_R_Pin */
  GPIO_InitStruct.Pin = LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_R_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
