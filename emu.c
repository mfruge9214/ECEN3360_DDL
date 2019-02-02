/*
 * #define MAX_EM_ELEMETS	5
 * uint32_t sleep_block_counter[MAX_EM_ELEMENTS];
 * enum={EM0, EM1, EM2, EM3, EM4};
 */


// Include Files
#include "emu.h"

#define	MAX_EM_ELEMENTS		5
// Global Variables

volatile uint32_t sleep_block_counter[MAX_EM_ELEMENTS];

// Functions
void sleep(void){
	if (sleep_block_counter[0] > 0) {
	return; // stay in EM0
	} else if (sleep_block_counter[1] > 0) {
	return; // EM1 is blocked, so go into EM0
	} else if (sleep_block_counter[2] > 0) {
	EMU_EnterEM1(); // EM2 is blocked, so go into EM1
	} else if (sleep_block_counter[3] > 0) {
	EMU_EnterEM2(true); // EM3 is blocked, so go into EM2
	} else{
	EMU_EnterEM3(true); // Don‘t go into EM4
	}
	return;
}

/** Unblock the microcontroller from sleeping below a certain mode
*
* This will unblock sleep() from entering an energy mode below the one given.
* -- To be called by peripheral HAL's --
*
* This should be called after all transactions on a peripheral are done.
*/
void unblockSleepMode(uint8_t minimumMode)
{
	CORE_ATOMIC_IRQ_DISABLE();
	if(sleep_block_counter[minimumMode] > 0) {
		sleep_block_counter[minimumMode]--;
	}
	CORE_ATOMIC_IRQ_ENABLE();
}

/** Block the microcontroller from sleeping below a certain mode
*
* This will block sleep() from entering an energy mode below the one given.
* -- To be called by peripheral HAL's --
*
* After the peripheral is finished with the operation, it should call unblock with the same state
*
*/
void blockSleepMode(uint8_t minimumMode)
{
CORE_ATOMIC_IRQ_DISABLE();
sleep_block_counter[minimumMode]++;
CORE_ATOMIC_IRQ_ENABLE();
}
