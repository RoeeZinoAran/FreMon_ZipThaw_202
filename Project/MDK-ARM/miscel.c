
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "miscel.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: C_MISCEL_INITAL_VALUE
  Unit: [N/A]
  Define Value: 0x96
  Description:  CRC-8 inital value.
*/
/*--------------------------------------------------------------------------------------------------*/
#define C_MISCEL_INITAL_VALUE 		(0x96)

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_MISCEL_debug9834475
  Variable Type: int [50]
  Unit: [N/A]
  Default value: N/A
  Description: 
*/
/*--------------------------------------------------------------------------------------------------*/
volatile int g_MISCEL_debug9834475[50];

/*$GLOBAL VARIABLES$--------------------------------------------------------------------------------*/
/*! 
  Variable Name: g_MISCEL_crc8_table
  Variable Type: uint8_t
  Unit: [N/A]
  Default value: N/A
  Description: CRC-8 calculation look up table.
*/
/*--------------------------------------------------------------------------------------------------*/
const uint8_t g_MISCEL_crc8_table[] =
{
	0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
	157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
	35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
	190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
	70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
	219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
	101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
	248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
	140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
	17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
	175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
	50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
	202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
	87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
	233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
	116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall translating a signed integer number to a string.
 	length of string has to be at least the length of 'len' + 1. The +1 is for
 	the terminating 0.
 	'after_dec' tells how many digits are to be put at right of decimal point. For
   	example: if din=12345  adn after_Dec = 2 then the string will be: 123.45.
 	'len' includes also the sign character ('-' or space) and the decimal point if
 	decimal point exists i.e. if 'after_dec' is Not 0.
\return Void
\DerivedDesc N/A.
\param Void
*/
/*--------------------------------------------------------------------------------------------------*/
void p_MISCEL_int_to_str(signed int din, char* str, unsigned short len, unsigned short after_dec)
{
	unsigned short 	s1;
    signed short 	index;
	unsigned int   	dd, sign;
	unsigned short 	mantisa_cnt;
	char  			str1[20];
	
	/* Find sign and deal with absolute value: */
	if (din < 0) 
	{
		sign = 1;
		dd = (-1) * din;
	}
	else 
	{
		dd = din;
		sign = 0;
	}
	
	for(s1 = 0; s1 < len; s1++)
	{
	  str1[s1] = ' ';
	}
	str1[len] = 0;
	str[len]  = 0;
	
	index = len - 1;
	mantisa_cnt = after_dec;
	
	while(mantisa_cnt) 
	{
		str1[index] = (dd % 10) + '0';
		dd /= 10;
		mantisa_cnt--;
		index--;
	}
	if (after_dec) 
	{
		str1[index] = '.';
		index--;
	}
	
	/* If final value to print is less then 1 */
	if (dd == 0) 
	{
		str1[index] = '0';
		index--;
		str1[index] = sign ? '-' : ' ';
		for(s1 = 0; s1 < len; s1++)
		{
			str[s1] = str1[s1];
		}
		str[len] = 0;
		return;
	}
	
	/* Now we are surely in the integer (not mantissa) part, and the integer to print is Not 0 */
	while(index >= 0) 
	{
		str1[index] = (dd % 10) + '0';
		index--;
		dd /= 10;
		if (dd == 0) {
			str1[index] = sign ? '-' : ' ';
			for(s1 = 0; s1 < len; s1++)
			{
				str[s1] = str1[s1];
			}
			str[len] = 0;
			return;
		}
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! 
\brief Interface function:
	The function shall calculate CRC-8 for message array.
\return Void
\DerivedDesc N/A.
\param uint8_t* msg_array - Income message array
	   Range: Full range.
	   uint8_t msg_size - Income message size.
	   Range: Full range.
*/
/*--------------------------------------------------------------------------------------------------*/
uint8_t p_MISCEL_calculate_crc8(uint8_t* msg_array, uint8_t msg_size)
{
	uint8_t crc = C_MISCEL_INITAL_VALUE;
	for (uint8_t idx = 0; idx < msg_size; idx++) 
	{                           
		crc = g_MISCEL_crc8_table[crc ^ (uint8_t)msg_array[idx]];
			g_MISCEL_debug9834475[idx] = crc;
	}
	return (crc);
}


