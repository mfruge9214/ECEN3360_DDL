// Include Files

#include "i2c.h"
#include "main.h"
#include "gpio.h"

void i2c0_init(void)
{
	// Initialize I2C struct
	I2C_Init_TypeDef init;
	init.enable=false;
	init.master=true;
	init.refFreq=0;
	init.freq=I2C_FREQ_FAST_MAX;
	init.clhr= _I2C_CTRL_CLHR_FAST;

	I2C_Init(I2C0, &init);

	// Route the pins
	I2C0 ->ROUTELOC0= I2C_ROUTELOC0_SCLLOC_LOC15 | I2C_ROUTELOC0_SDALOC_LOC15;
	I2C0->ROUTEPEN= I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN;

	// Enable CLTO
	I2C0->CTRL |= I2C_CTRL_CLTO_1024PCC;
	I2C0->CTRL |= I2C_CTRL_BITO_160PCC; // Says 1024PCC in the lab

	// Enable interrupts
	// These first 4 lines are for debugging the I2C bus, not necessary
	I2C0->IFC |= I2C_IFC_CLTO;
	I2C0->IEN |= I2C_IEN_CLTO;
	I2C0->IFC |= I2C_IFC_BITO;
	I2C0->IEN |= I2C_IEN_BITO;

	I2C0->IFC |= I2C_IFC_ACK;
	I2C0->IEN |= I2C_IEN_ACK;
	I2C0->IEN |= I2C_IEN_RXDATAV;

	//	Reset bus if busy
	//	Toggle I2C 9 times to reset any slaves that need it
	for (int i=0; i<9; i++)
	{
		GPIO_PinOutClear(I2C_SCL_PORT, I2C_SCL_PIN);
		GPIO_PinOutSet(I2C_SCL_PORT, I2C_SCL_PIN);
	}
	reset_I2C_Bus();
//	Enable the interrupts
//	NVIC_EnableIRQ(I2C0_IRQn);
}


void read_I2C(void)
{
//	 Follow flowchart
//	 Start and slave Address and Write
	start_I2C(I2C_WRITE);
//	Response
	wait_for_ACK();

	I2C0->TXDATA=READ_COMMAND;		// Read reg cmd
//	Response
	wait_for_ACK();
//	Restart
	start_I2C(I2C_READ);

	wait_for_ACK();
	// Read data
	while(!(I2C0->IF & I2C_IF_RXDATAV));
//	Automatically clears the flag
//	 58 when it is working
	uint8_t data=I2C0->RXDATA;  // in_data does not show up in the variables window during debug because it is global...
//	... Using local var for now
	in_data=data;
	stop_I2C();
}

void start_I2C(bool read_or_write)		// Start I2C with a read or a write bit
{
	I2C0->CMD =I2C_CMD_START;
	I2C0->TXDATA=(SENSOR_ADDRESS<<1)| read_or_write;
}
void write_I2C(uint8_t data)
{
	start_I2C(I2C_WRITE);
	wait_for_ACK();
	I2C0->TXDATA=WRITE_COMMAND;
	wait_for_ACK();
	I2C0->TXDATA=data;
//	Wait for the data to be sent before sending ACK????????????????
//	Send ACK
	I2C0->CMD=I2C_CMD_ACK;
//	Send Stop
	I2C0->CMD=I2C_CMD_STOP;
}

void wait_for_ACK(void)
{
	while(!(I2C0->IF & I2C_IF_ACK)); // Wait for the ACK bit to be sent
	I2C0->IFC= I2C_IFC_ACK;		// Clear the flag for the ACK bit
}

void stop_I2C(void)
{
	I2C0->CMD=I2C_CMD_NACK;
	I2C0->CMD=I2C_CMD_STOP;
}

void reset_I2C_Bus(void)
{
	if(I2C0->STATE & I2C_STATE_BUSY)
	{
		I2C0->CMD=I2C_CMD_ABORT;
	}
}

void I2C0_IRQHandler(void)
{
	/*
	 * For the Extra credit, handle all the interrupts within this IRQ handler
	 * If int_flag== RXDATAV, then read the data
	 * if int_flag== ACK, clear the ack
	 */
	unsigned int int_flag;

	CORE_ATOMIC_IRQ_DISABLE();		// prevent nesting of interrupts for initial interrupt handling
	int_flag=I2C0->IF;
	I2C0->IFC=int_flag;

//	Set LED's based on the CLTO and the BITO data

	CORE_ATOMIC_IRQ_DISABLE();
}
