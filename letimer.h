//***********************************************************************************
// Include files
//***********************************************************************************
#include <em_letimer.h>
#include <em_cmu.h>
#include "cmu.h"
#include "gpio.h"
#include "emu.h"
#include <stdint.h>
#include <stdbool.h>


#define LFXO_FREQ 		 	32768		// Becomes a variable in the EC because you need to change the clk freq from 32768 to 1000
#define LETIMER0_PERIOD 	1.75
#define LED_ON_TIME			.4
#define ULFRCO_FREQ			1000
#define LFXO_PRESCALAR		1024
#define LETIMER0_EM_MIN		EM3
#define MAXCOUNT			65535

enum {EM0, EM1, EM2, EM3, EM4};


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void letimer0_init();
void LETIMER0_IRQHandler(void);


