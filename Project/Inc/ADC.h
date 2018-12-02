
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "stm32l0xx_hal.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEATER0_CURRENT_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 0
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEATER0_CURRENT_ADC_CHANNEL 	(0)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: HEATER1_CURRENT_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 1
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define HEATER1_CURRENT_ADC_CHANNEL   	(1)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: NTC_IN_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 2
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define NTC_IN_ADC_CHANNEL          	(2)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: NTC_OUT_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 3
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define NTC_OUT_ADC_CHANNEL         	(3)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: WEIGHT_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 4
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define WEIGHT_ADC_CHANNEL          	(4)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: P3V3_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 5
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define P3V3_ADC_CHANNEL          		(5)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: P24V0_ADC_CHANNEL
  Unit: [N/A]
  Define Value: 6
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define P24V0_ADC_CHANNEL          		(6)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: NTC_EXCITATION_VOLTAGE_mV
  Unit: [N/A]
  Define Value: 3000
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define NTC_EXCITATION_VOLTAGE_mV        (3000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: PRESSURE_EXCITATION_VOLTAGE_mV
  Unit: [N/A]
  Define Value: 3000
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define PRESSURE_EXCITATION_VOLTAGE_mV   (3000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: NTCS_HISTORY_DEPTH
  Unit: [N/A]
  Define Value: 8
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define NTCS_HISTORY_DEPTH  				(8)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: ERRONEOUS_TEMPERATURE_DEVIATION
  Unit: [N/A]
  Define Value: 3000
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define ERRONEOUS_TEMPERATURE_DEVIATION  	(3000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*! \ingroup 
  Define Name: NUM_OF_ADC_CHANNELS
  Unit: [N/A]
  Define Value: 8
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define NUM_OF_ADC_CHANNELS   				(8)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void ADC_go(void);
signed int ntc_resistance_to_temp(unsigned int ntc);
unsigned int resistance_to_weight(unsigned int resistance);
void translate_adc(void);
void calib_adc(void);
void filter_ntcs(void);


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UNUSED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

//#define NUM_OF_ADC_CHANNELS   20
// 8 because we have channels 0 to 6 + channel 17 which is internal reference


// ----------------------------------------------------------------------------
//#define VREFINT_CAL_LSB_ADDR    0x1FF80078 
//#define VREFINT_CAL_MSB_ADDR    0x1FF80079 
// This address and the following one are the 16 bits address to 
// read VREFINT_CAL  for calibrating the ADC.
// --------------------------------------------------------------------------
