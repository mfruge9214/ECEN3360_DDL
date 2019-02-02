// Includes



#include "letimer.h"



// Global Vars



// Functions
void letimer0_init()
{
	//uint32_t vcomp0= LFXO_PRESCALAR*LETIMER0_PERIOD;
	//uint32_t vcomp1=vcomp0-(LFXO_PRESCALAR*LED_ON_TIME);
	uint32_t vcomp0, vcomp1, eff_freq, prescalar;
	vcomp0= LFXO_FREQ*LETIMER0_PERIOD;
	eff_freq=LFXO_FREQ;
	prescalar=1;
	while(vcomp0/MAXCOUNT)
	{
		prescalar=prescalar<<1;
		eff_freq=eff_freq>>1;
		vcomp0=eff_freq*LETIMER0_PERIOD;
	}
	if(prescalar!=1)
	{
		CMU_ClockPrescSet(cmuClock_LETIMER0, prescalar);
	}
	vcomp1=vcomp0-(eff_freq*LED_ON_TIME);



	// Initialize values for LETIMER type Def
	LETIMER_Init_TypeDef init;

	init.enable=false;			/* Enable timer when initialization completes. */
	init.debugRun=false;		/* Stop counter during debug halt. */
	init.comp0Top=true;			/* load COMP0 into CNT on underflow. */
	init.bufTop=false;         /**< Load COMP1 into COMP0 when REP0 reaches 0. */
	init.out0Pol=false;        /**< Idle value for output 0. */
	init.out1Pol=false;        /**< Idle value for output 1. */
	init.ufoa0=letimerUFOANone;          /**< Underflow output 0 action. */
	init.ufoa1=letimerUFOANone;          /**< Underflow output 1 action. */
	init.repMode=letimerRepeatFree;        /**< Repeat mode. */
	init.topValue=0;       /**< Top value. Counter wraps when top value matches counter value is reached. */

	// Call the init function
	LETIMER_Init(LETIMER0, &init);

	// Calculate and load the values of seconds_ticks and on_ticks into comp0 and comp1
	LETIMER_CompareSet(LETIMER0, 0, vcomp0);
	LETIMER_CompareSet(LETIMER0, 1, vcomp1);

	// Clear and enable the interrupts

	LETIMER0->IFC =LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1;
	LETIMER0->IEN |=LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1;

	//blockSleepMode(LETIMER0_EM_MIN);

	NVIC_EnableIRQ(LETIMER0_IRQn);
}

void LETIMER0_IRQHandler(void){
	uint32_t int_flag;
	int_flag=LETIMER0->IF;
	LETIMER0->IFC=int_flag;
	if(int_flag & LETIMER_IF_COMP0)
	{
		GPIO_PinOutSet(LED0_port, LED0_pin);
	}

	if(int_flag & LETIMER_IF_COMP1)
	{
		GPIO_PinOutClear(LED0_port, LED0_pin);
	}
}


