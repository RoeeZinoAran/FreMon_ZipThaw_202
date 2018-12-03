
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "ADC.h"
#include "stdint.h"
#include "stm32l0xx_hal.h"
#include "miscel.h"
#include "extern_structs.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_ADC_FULL_SCALE_MILI_V
  Unit: [mVolts]
  Define Value: 3300
  Description: ADC full scale. 
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_ADC_FULL_SCALE_MILI_V 	(3300)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_ADC_V_REF
  Unit: [mVolts]
  Define Value: 4096
  Description: ADC voltage referance (for 12 bit convertor). 
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_ADC_V_REF					(4096)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_ADC_SERIES_RES_VOLTAGE
  Unit: [mVolts]
  Define Value: 4990
  Description: ADC series voltage. 
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_ADC_SERIES_RES_VOLTAGE 	(4990)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_ADC_TEMP_ERROR_90_DEG
  Unit: [degree]
  Define Value: 90000
  Description: ADC tempture error code. 
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_ADC_TEMP_ERROR_90_DEG 	(90000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_ADC_NTCS_RESISTANCE
  Unit: [homs]
  Define Value: 10000
  Description: NTCS resistance. 
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_ADC_NTCS_RESISTANCE 		(10000)

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */


/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: hadc
  Variable Type: ADC_HandleTypeDef
  Extern module declaration: Main. 
  Unit: [N/A]
  Default value: N/A
  Description: Header ADC handle type. 
*/
/*--------------------------------------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_MAIN_adc_conversion_callback
  Variable Type: unsigned short
  Extern module declaration: Main.
  Unit: [N/A]
  Default value: N/A
  Description: Function callback for ADC converstion.
*/
/*--------------------------------------------------------------------------------------------------*/
extern volatile unsigned short g_MAIN_adc_conversion_callback;

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_ADC_adc_raw_results
  Variable Type: uint32_t [NUM_OF_ADC_CHANNELS]
  Unit: [N/A]
  Default value: N/A
  Description: ADC raw results handler.
*/
/*--------------------------------------------------------------------------------------------------*/
uint32_t g_ADC_adc_raw_results [NUM_OF_ADC_CHANNELS];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_ADC_adc_raw_results
  Variable Type: uint32_t [NUM_OF_ADC_CHANNELS]
  Unit: [N/A]
  Default value: As described below.
  Description: NTC conversion table. 10000 (see 25C) means 10K * 1 = 10K Ohm.
               32791 (see  0C) means 10K * 3.2791 = 32.791K Ohm.
               etc...
*/
/*--------------------------------------------------------------------------------------------------*/
const unsigned int g_ADC_ntc_table[] = 
{
	177969, // -30 C
	167327, // -29 C
	157384, // -28 C
	148091, // -27 C
	139402, // -26 C
	131273, // -25 C
	123666, // -24 C
	116544, // -23 C
	109874, // -22 C
	103624, // -21 C
	 97765, // -20 C
	 92271, // -19 C
	 87118, // -18 C
	 82281, // -17 C
 	 77741, // -16 C
	 73476, // -15 C
	 69470, // -14 C
	 65704, // -13 C
	 62174, // -12 C
	 58834, // -11 C
	 55700, // -10 C
	 52751, //  -9 C
	 49975, //  -8 C
	 47359, //  -7 C
	 44895, //  -6 C
	 42572, //  -5 C
	 40382, //  -4 C
	 38317, //  -3 C
	 36368, //  -2 C
	 34529, //  -1 C
	 32791, //   0 C
	 31165, //   1 C
	 29628, //   2 C
	 28176, //   3 C
	 26802, //   4 C
	 25504, //   5 C
	 24275, //   6 C
	 23111, //   7 C
	 22010, //   8 C
	 20968, //   9 C
	 19980, //  10 C
	 19044, //  11 C
	 18157, //  12 C
	 17315, //  13 C
	 16518, //  14 C
	 15761, //  15 C
	 15043, //  16 C
	 14361, //  17 C
	 13714, //  18 C
	 13099, //  19 C
	 12515, //  20 C
	 11960, //  21 C
	 11432, //  22 C
	 10931, //  23 C
	 10454, //  24 C
	 10000, //  25 C
	  9568, //  26 C
	  9157, //  27 C
	  8766, //  28 C
	  8393, //  29 C
	  8038, //  30 C
	  7700, //  31 C
	  7378, //  32 C
	  7071, //  33 C
	  6778, //  34 C
	  6498, //  35 C
	  6232, //  36 C
	  5978, //  37 C
	  5735, //  38 C
	  5503, //  39 C
	  5282, //  40 C
	  5071, //  41 C
	  4869, //  42 C
	  4677, //  43 C
	  4492, //  44 C
	  4316, //  45 C
	  4148, //  46 C
	  3987, //  47 C
	  3833, //  48 C
	  3686, //  49 C
	  3545, //  50 C
	  3415, //  51 C
	  3291, //  52 C
	  3172, //  53 C
	  3058, //  54 C
	  2948, //  55 C
	  2844, //  56 C
	  2743, //  57 C
	  2646, //  58 C
	  2554, //  59 C
	  2465  //  60 C
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall activating ADC to start sample.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_ADC_start_conv(void)
{
	g_MAIN_adc_conversion_callback = 0;
	
	/* Activating ADC conversions in DMA mode */
	HAL_ADC_Start_DMA(&hadc, g_ADC_adc_raw_results, NUM_OF_ADC_CHANNELS);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall filtering out errors (spikes etc...) on NTC readings.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/

void filter_ntcs(void)
{
	static signed int   	NTCs_temperature_history[2][NTCS_HISTORY_DEPTH];
	static unsigned short 	NTCs_temperature_history_pointer = 0;
	unsigned short 			cushion;
	signed int 				average[2];

	/* listing for each NTC, the history indexes that are far from average. */
	unsigned short failing_results[2][NTCS_HISTORY_DEPTH]; 
	signed int is1;
	unsigned short s1, s2;

	/* Updating NTCs' history */
	NTCs_temperature_history[CUSHION0][NTCs_temperature_history_pointer] = system_state.ntc_temp[CUSHION0];
	NTCs_temperature_history[CUSHION1][NTCs_temperature_history_pointer] = system_state.ntc_temp[CUSHION1];
	NTCs_temperature_history_pointer++;
	if (NTCs_temperature_history_pointer >= NTCS_HISTORY_DEPTH) 
	{
		NTCs_temperature_history_pointer = 0; 
	}


	/* Averaging */
	for(cushion = 0; cushion < 2; cushion++) 
	{
		average[cushion] = 0;
		for(s1 = 0; s1 < NTCS_HISTORY_DEPTH; s1++) 
		{
			average[cushion] += NTCs_temperature_history[cushion][s1];
		}
		average[cushion] /= NTCS_HISTORY_DEPTH;
	}
	
	
	/* Checking the number of failing results for each cushion */
	for(cushion = 0; cushion < 2; cushion++) 
	{
		for(s1 = 0; s1 < NTCS_HISTORY_DEPTH; s1++) 
		{
			/* is1 is the difference between current measurement and average */
			is1 = average[cushion] - NTCs_temperature_history[cushion][s1];
			if (is1 < 0) is1 = (-1);
			
			/* Marking the samples that are wierd */
			failing_results[cushion][s1] =  is1 > ERRONEOUS_TEMPERATURE_DEVIATION ? 1 : 0;
		}
	}

	
	/* Averaging again without the failing indexes */
	for(cushion = 0; cushion < 2; cushion++) 
	{
		s2 = NTCS_HISTORY_DEPTH; /* s1 contains the number of good results in the history */
		average[cushion] = 0;
		for(s1 = 0; s1 < NTCS_HISTORY_DEPTH; s1++) 
		{
			if (failing_results[cushion][s1]) /* if failed history member */
			{
				s2--;
			}
			else
			{
				average[cushion] += NTCs_temperature_history[cushion][s1];
			}
		}
		if (s2) 
		{
			average[cushion] /= s2;
			system_state.ntc_temp[cushion] = average[cushion];
		}
	}
}


/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall Translating raw ADC result to 'human' values. (Cuurents in Amps. temperatures
 	in C etc...
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_ADC_translate_adc(void)
{
	volatile signed int 	is1;
	volatile unsigned int   resistance;
	unsigned short 			s1;
	
	if (g_ADC_adc_raw_results[3] < 1800)
	{
		s1++;
	}
	
	/* Heater curent 0 */
	is1 = g_ADC_adc_raw_results[HEATER0_CURRENT_ADC_CHANNEL] * C_ADC_FULL_SCALE_MILI_V;
	is1 /= C_ADC_V_REF;
	if (is1 < C_ADC_FULL_SCALE_MILI_V) /* 0.1 Vcc which is 0.33V is the offset of the current measurement. */
	{
		is1 = 0;
	}
	else
	{
		is1 -= C_ADC_FULL_SCALE_MILI_V;
	}
	is1 *= 1000; /* for mAmp (and Not Amp) */
	is1 /= 264;  /* The measuring device gives 264mV / Amp */
	system_state.heater_current[CUSHION0] = is1;

	/* Heater curent 1 */
	is1 = g_ADC_adc_raw_results[HEATER1_CURRENT_ADC_CHANNEL] * C_ADC_FULL_SCALE_MILI_V;
	is1 /= C_ADC_V_REF;
	if (is1 < C_ADC_FULL_SCALE_MILI_V) 	/* 0.1 Vcc which is 0.33V is the offset f the current measurement. */
	{
		is1 = 0;
	}
	else
	{
		is1 -= C_ADC_FULL_SCALE_MILI_V;
	}
	is1 *= 1000; 		/* For mAmp (and Not Amp) */
	is1 /= 264;  		/* The measuring device gives 264mV / Amp */
	system_state.heater_current[CUSHION1] = is1;
	
	
  
   	/* NTCs */
	for(s1 = 0; s1 < 2; s1++) /* Loop for 2 NTCs */ 
	{ 
	 	resistance = NTC_EXCITATION_VOLTAGE_mV * C_ADC_V_REF; 
		resistance /= C_ADC_FULL_SCALE_MILI_V;
		resistance *= C_ADC_NTCS_RESISTANCE; 							
		resistance /= g_ADC_adc_raw_results[s1 ? NTC_OUT_ADC_CHANNEL : NTC_IN_ADC_CHANNEL];
		resistance -= C_ADC_NTCS_RESISTANCE; 						
		if (s1)
		{
			system_state.ntc_temp[CUSHION0] = p_ADC_ntc_resistance_to_temp(resistance);
		}
		 else
		 {
			 system_state.ntc_temp[CUSHION1] = p_ADC_ntc_resistance_to_temp(resistance);
		 }
	}
   
	 /* Filtering NTCs */
	 p_ADC_translate_adc();

	/* Pressure (weight) */
	resistance = PRESSURE_EXCITATION_VOLTAGE_mV * C_ADC_V_REF; /* 4096 is the resolution (12 bits) of the ADC */
	resistance /= C_ADC_FULL_SCALE_MILI_V;
	resistance *= C_ADC_SERIES_RES_VOLTAGE; 								/* 4990 is the series resistance*/
	resistance /= g_ADC_adc_raw_results[WEIGHT_ADC_CHANNEL];
	resistance -= C_ADC_SERIES_RES_VOLTAGE; 								/* 4990 is the series resistance */
	system_state.weight = p_ADC_resistance_to_weight(resistance);
	
	 
	/* 3.3V supply */
	is1 = g_ADC_adc_raw_results[P3V3_ADC_CHANNEL] * C_ADC_FULL_SCALE_MILI_V;
	is1 /= C_ADC_V_REF;
	is1 *= 3; 											/* The resistors devide at ADC input passes 2/3 of the voltage */
	is1 /= 2;
	system_state.p3v3 = is1;

	/* 24V supply */
	is1 = g_ADC_adc_raw_results[P24V0_ADC_CHANNEL] * C_ADC_FULL_SCALE_MILI_V;
	is1 /= C_ADC_V_REF;
	is1 *= 449; 										/* The resistors devide at ADC input passes 4.7/44.9 of the voltage */
	is1 /= 47;
	system_state.p24v0 = is1;
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall translating raw ADC result to 'human' values. (Cuurents in Amps. temperatures
 	in C etc...
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_ADC_calib(void)
{

#if 0
	uint16_t ref_int_cal;
	
	ref_int_cal = *((uint16_t*)0x1FF80078);
	
	p_ADC_start_conv();
 	HAL_Delay(100); // to allow ADC conversions
	C_ADC_FULL_SCALE_MILI_V  = 3000 * ((unsigned int)ref_int_cal);
	C_ADC_FULL_SCALE_MILI_V /= ((unsigned int)g_ADC_adc_raw_results[7]);
	
	C_ADC_FULL_SCALE_MILI_V = 3300;
#endif
}


/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall receives NTC resistance and returns temperature in mC units.
\return signed int: temperature in mC units.
\DerivedDesc N/A.
\param unsigned int ntc - Income NTC resistance.
	   Range: Full range.
*/
/*--------------------------------------------------------------------------------------------------*/
signed int p_ADC_ntc_resistance_to_temp(unsigned int ntc)
{
	#define FIRST_TEMP_IN_NTC_TABLE  (-30)
	
	signed int   			index;
	volatile  signed int   	temp0, temp1, is0, is1;
	
	/* finding the index in NTC table that the ntc 
	   resistance is equal to or slightly lower then */
	for(index = 0; index < sizeof(g_ADC_ntc_table); index++)
	{
		if (ntc >= g_ADC_ntc_table[index])
		{
			break;
		}
	}
		
	/* If we are out of table i.e. temperature is too high */
	if (index >= (sizeof(g_ADC_ntc_table) - 1)) /* if resistance is too low (i.e. it is very hot) */
	{
		return(C_ADC_TEMP_ERROR_90_DEG); /* return 90C (90000mC) */
	}
	
	/* If NTC resistance is exactly like one of the values in the table */
	if (ntc == g_ADC_ntc_table[index])
	{
		return(1000 * (index + FIRST_TEMP_IN_NTC_TABLE));
	}
	
	/* If NTC value is between lines of table */
	else 
	{
		temp0 = 1000 * (index + FIRST_TEMP_IN_NTC_TABLE); 	/* temperature at g_ADC_ntc_table[index] in mC */
		is0   = (ntc - g_ADC_ntc_table[index + 0]) * 1000; 		/* 1000 is the temperature difference (in mC) between g_ADC_ntc_table[index + 0] abd g_ADC_ntc_table[index + 1] */
		is0  /= (g_ADC_ntc_table[index + 0] - g_ADC_ntc_table[index + 1]);
		return(temp0 - is0);
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function receives pressure (weight) sensor's resistance in Ohms and returns
 	weight in gram units.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
unsigned int p_ADC_resistance_to_weight(unsigned int resistance)
{
	return 0; // @@@wr
}
