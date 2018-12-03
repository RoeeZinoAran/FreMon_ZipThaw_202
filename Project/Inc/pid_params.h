
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: HIGHEST_TEMP_THAT_PWM_CAN_OPERATE
  Unit: [N/A]
  Define Value: 37000
  Description:  The highest temperature (in mC) in which heater PWM may still operate.
				When reaching this (or higher) temperature then PWM duty cycle is forced
				to be 0 (no heating).
*/
/*--------------------------------------------------------------------------------------------------*/
#define HIGHEST_TEMP_THAT_PWM_CAN_OPERATE  			(37000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: THAWING_TARGET_TEMPERATURE
  Unit: [N/A]
  Define Value: 33000
  Description:  The temperature (in mC) that if was read by sensor then thawing compleeted.
*/
/*--------------------------------------------------------------------------------------------------*/
#define THAWING_TARGET_TEMPERATURE  				(33000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: LOWEST_PLASMA_TEMPERATURE
  Unit: [N/A]
  Define Value: -20000
  Description:  The lowest expected temperature (in mC) of the plasma bag.
*/
/*--------------------------------------------------------------------------------------------------*/
#define LOWEST_PLASMA_TEMPERATURE  					(-20000)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: IR_I2C_7BITS_ADD
  Unit: [N/A]
  Define Value: 0x5A
  Description: 10,000 is the value that if sent to PWM register gives 100% duty cycle.
 			   However - we can limit here PWM duty cycle to lower value.
*/
/*--------------------------------------------------------------------------------------------------*/
#define PWM_MAX_VALUE   							(9999)

/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: PID_INTEGRAL_DEPTH
  Unit: [N/A]
  Define Value: 20
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define PID_INTEGRAL_DEPTH							(20)


/*$DEFINES$-----------------------------------------------------------------------------------------*/
/*
  Define Name: IR_I2C_7BITS_ADD
  Unit: [N/A]
  Define Value: 0x5A
  Description:  
*/
/*--------------------------------------------------------------------------------------------------*/
#define NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID	(50)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TYPES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

static unsigned int   pid_p_contst[2] = {17725, 17725}; // 256 stands for K=1
static unsigned int   pid_i_contst[2] = { 240, 240}; // 256 stands for K=1 
static unsigned int   pid_d_contst[2] = {   0,   0}; // 256 stands for K=1



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UNUSED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/* --------------------------------------------------------------------------
#define PID_INTEGRAL_DEPTH                            20
// must not be bigger then 2000
#define NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID    50
static unsigned int   pid_p_contst[2] = { 128,  128}; // 256 stands for K=1
static unsigned int   pid_i_contst[2] = {  64,   64}; // 256 stands for K=1
static unsigned int   pid_d_contst[2] = {   0,    0}; // 256 stands for K=1
-------------------------------------------------------------------------- */


/*
// --------------------------------------------------------------------------
#define PID_INTEGRAL_DEPTH                            20
// must not be bigger then 2000
#define NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID    50
static unsigned int   pid_p_contst[2] = {8192, 8192}; // 256 stands for K=1
static unsigned int   pid_i_contst[2] = {   0,    0}; // 256 stands for K=1
static unsigned int   pid_d_contst[2] = {   0,    0}; // 256 stands for K=1
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
#define PID_INTEGRAL_DEPTH                            20
// must not be bigger then 2000
#define NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID    50
static unsigned int   pid_p_contst[2] = {14725, 14725}; // 256 stands for K=1
static unsigned int   pid_i_contst[2] = { 240, 240}; // 256 stands for K=1
static unsigned int   pid_d_contst[2] = {   0,   0}; // 256 stands for K=1
// --------------------------------------------------------------------------
*/ 
