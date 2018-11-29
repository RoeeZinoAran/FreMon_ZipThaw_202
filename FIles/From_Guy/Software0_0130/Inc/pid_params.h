// --------------------------------------------------------------------------
#define HIGHEST_TEMP_THAT_PWM_CAN_OPERATE  37000
// The highest temperature (in mC) in which heater PWM may still operate.
// When reaching this (or higher) temperature then PWM duty cycle is forced
// to be 0 (no heating).
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
#define THAWING_TARGET_TEMPERATURE  33000
// The The temperature (in mC) that if was read by sensor then thawing compleeted.
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
#define LOWEST_PLASMA_TEMPERATURE  (-20000)
// The lowest expected temperature (in mC) of the plasma bag.
// --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 10,000 is the value that if sent to PWM register gives 100% duty cycle.
 However - we can limit here PWM duty cycle to lower value.
-------------------------------------------------------------------------- */
#define PWM_MAX_VALUE   9999
// --------------------------------------------------------------------------


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


// -------------------------------------------------------------------------- 
#define PID_INTEGRAL_DEPTH                            20
// must not be bigger then 2000
#define NUM_OF_LOOPS_FOR_CALCULATING_D_PART_OF_PID    50
static unsigned int   pid_p_contst[2] = {17725, 17725}; // 256 stands for K=1
static unsigned int   pid_i_contst[2] = { 240, 240}; // 256 stands for K=1 
static unsigned int   pid_d_contst[2] = {   0,   0}; // 256 stands for K=1
// ----------------------------------------------------------------

