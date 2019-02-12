//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_letimer.h"
#include "em_cmu.h"
#include "gpio.h"
#include "sleep_routines.h"

//***********************************************************************************
// defined files
//***********************************************************************************
#define Letimer0_Std_Ticks		32768	// Number of standard ticks per second
#define Letimer0_16bit_Ticks	65536	// Number of ticks in a 16-bit counter

#define Letimer0_EM3_Ticks		1000	// Number of ticks per second of ULFRCO in EM3

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void letimer0_init(void);
void LETIMER0_IRQHandler(void);
