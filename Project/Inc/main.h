/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define HeaterEnChamber_Pin GPIO_PIN_2
#define HeaterEnChamber_GPIO_Port GPIOE
#define HOW_HOW_Pin GPIO_PIN_9
#define HOW_HOW_GPIO_Port GPIOH
#define LED_B_Pin GPIO_PIN_10
#define LED_B_GPIO_Port GPIOE
#define LED_G_Pin GPIO_PIN_11
#define LED_G_GPIO_Port GPIOE
#define SPI_CS_n_Pin GPIO_PIN_12
#define SPI_CS_n_GPIO_Port GPIOE
#define MOTOR_INDEX_Pin GPIO_PIN_12
#define MOTOR_INDEX_GPIO_Port GPIOB
#define DOOR_STATE_Pin GPIO_PIN_10
#define DOOR_STATE_GPIO_Port GPIOD
#define COMM_ADD0_Pin GPIO_PIN_6
#define COMM_ADD0_GPIO_Port GPIOC
#define COMM_ADD1_Pin GPIO_PIN_7
#define COMM_ADD1_GPIO_Port GPIOC
#define COMM_ADD2_Pin GPIO_PIN_8
#define COMM_ADD2_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_15
#define LED_R_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#include "stm32l0xx_hal.h"

// --------------------------------------------------------------------------
#define FIRMWARE_VERSION 0x0131			// Increase KVAL_RUN to 0x96
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
#define CUSHION0    0
#define CUSHION1    1
// Cushion 1 is the one closer to door. Cushion 0 is the one inside the machine.
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
#define DEBUG_COMM_UART_NUM   1
#define SOM_COMM_UART_NUM     2
#define RFID_COMM_UART_NUM    4
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Bits of  'main_loop_tasks' variable.
// --------------------------------------------------------------------------
#define ADC_MAIN_LOOP_TASK_BIT							(1 <<  0)
#define PID_CALC_MAIN_LOOP_TASK_BIT     		(1 <<  1)
#define HEATER_UPDATE_MAIN_LOOP_TASK_BIT		(1 <<  2)
#define MOTOR_MAIN_LOOP_TASK_BIT						(1 <<  3)
#define SOM_ADD_UPDATE_MAIN_LOOP_TASK_BIT		(1 <<  4)
#define PCB_TEMP_MAIN_LOOP_TASK_BIT					(1 <<  5)
#define IR_TEMP_MAIN_LOOP_TASK_BIT					(1 <<  6)
#define RFID_TEMP_MAIN_LOOP_TASK_BIT				(1 <<  7)
#define	DOOR_CHECK_MAIN_LOOP_TASK_BIT				(1 <<  8)
#define STAGE_UPDATE_MAIN_LOOP_TASK_BIT			(1 <<  9)
#define DEBUG_TERMINAL_MAIN_LOOP_TASK_BIT		(1 << 10)
#define SOM_SERVICE_MAIN_LOOP_TASK_BIT      (1 << 11)
// --------------------------------------------------------------------------

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
