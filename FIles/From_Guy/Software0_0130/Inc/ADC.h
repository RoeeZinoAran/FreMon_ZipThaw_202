
#include "stm32l0xx_hal.h"

#define HEATER0_CURRENT_ADC_CHANNEL 	0
#define HEATER1_CURRENT_ADC_CHANNEL   1
#define NTC_IN_ADC_CHANNEL          	2
#define NTC_OUT_ADC_CHANNEL         	3
#define WEIGHT_ADC_CHANNEL          	4
#define P3V3_ADC_CHANNEL          		5
#define P24V0_ADC_CHANNEL          		6


#define NTC_EXCITATION_VOLTAGE_mV        3000
#define PRESSURE_EXCITATION_VOLTAGE_mV   3000

#define NTCS_HISTORY_DEPTH  8

#define ERRONEOUS_TEMPERATURE_DEVIATION  3000

void ADC_go(void);
signed int ntc_resistance_to_temp(unsigned int ntc);
unsigned int resistance_to_weight(unsigned int resistance);
void translate_adc(void);
void calib_adc(void);
void filter_ntcs(void);


#define NUM_OF_ADC_CHANNELS   	8
//#define NUM_OF_ADC_CHANNELS   20
// 8 because we have channels 0 to 6 + channel 17 which is internal reference


// ----------------------------------------------------------------------------
//#define VREFINT_CAL_LSB_ADDR    0x1FF80078 
//#define VREFINT_CAL_MSB_ADDR    0x1FF80079 
// This address and the following one are the 16 bits address to 
// read VREFINT_CAL  for calibrating the ADC.
// --------------------------------------------------------------------------
