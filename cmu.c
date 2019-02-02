//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************




//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void){

		uint32_t	temp_freq;

		CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFXO);
		CMU_ClockEnable(cmuClock_HFPER, true);

		// By default, LFRCO is enabled
		CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);	// using LFXO or ULFRCO

		// Route LF clock to the LF clock tree

		CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false); // Change these to true for EC
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true);		// Disable LFXO (Changed to false for EC)
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);	// routing clock to LFA

		//CMU->LFAPRESC0 = PRESCALAR;


		CMU_ClockEnable(cmuClock_LFA, true);
		CMU_ClockEnable(cmuClock_CORELE, true);

		// Peripheral clocks enabled
		CMU_ClockEnable(cmuClock_GPIO, true);
		CMU_ClockEnable(cmuClock_LETIMER0, true);

}
