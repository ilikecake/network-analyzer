/*
 * hardware.c
 *
 *  Created on: Dec 13, 2013
 *      Author: pat
 */

#include "main.h"

#define BUZZ_TIMER						LPC_TIMER16_1
#define BUZZ_TIMER_IRQ_HANDLER			TIMER16_1_IRQHandler
#define BUZZ_TIMER_NVIC_NAME			TIMER_16_1_IRQn

#define DEBOUNCE_TIMER					LPC_TIMER16_0
#define DEBOUNCE_TIMER_IRQ_HANDLER		TIMER16_0_IRQHandler
#define DEBOUNCE_TIMER_NVIC_NAME		TIMER_16_0_IRQn

//Setup for the button pins
//Note: if the GPIO function for the pin is not function 0, the init function will have to be modified
#define BUTTON_1_PORT				0
#define BUTTON_1_PIN				18
#define BUTTON_1_PININT_INDEX		1
#define BUTTON_1_IRQ_HANDLER		FLEX_INT1_IRQHandler	/* IRQ function name */
#define BUTTON_1_NVIC_NAME			PIN_INT1_IRQn			/* NVIC interrupt name */
#define BUTTON_1_DEBOUNCE_TIME		40

#define BUTTON_2_PORT				0
#define BUTTON_2_PIN				19
#define BUTTON_2_PININT_INDEX		2
#define BUTTON_2_IRQ_HANDLER		FLEX_INT2_IRQHandler	/* IRQ function name */
#define BUTTON_2_NVIC_NAME			PIN_INT2_IRQn			/* NVIC interrupt name */
#define BUTTON_2_DEBOUNCE_TIME		40

#define BUTTON_3_PORT				1
#define BUTTON_3_PIN				16
#define BUTTON_3_PININT_INDEX		3
#define BUTTON_3_IRQ_HANDLER		FLEX_INT3_IRQHandler	/* IRQ function name */
#define BUTTON_3_NVIC_NAME			PIN_INT3_IRQn			/* NVIC interrupt name */
#define BUTTON_3_DEBOUNCE_TIME		40

#define BUTTON_4_PORT				1
#define BUTTON_4_PIN				19
#define BUTTON_4_PININT_INDEX		4
#define BUTTON_4_IRQ_HANDLER		FLEX_INT4_IRQHandler	/* IRQ function name */
#define BUTTON_4_NVIC_NAME			PIN_INT4_IRQn			/* NVIC interrupt name */
#define BUTTON_4_DEBOUNCE_TIME		75

#define BUTTON_5_PORT				1
#define BUTTON_5_PIN				25
#define BUTTON_5_PININT_INDEX		5
#define BUTTON_5_IRQ_HANDLER		FLEX_INT5_IRQHandler	/* IRQ function name */
#define BUTTON_5_NVIC_NAME			PIN_INT5_IRQn			/* NVIC interrupt name */
#define BUTTON_5_DEBOUNCE_TIME		40


/** This pin connects the EEPROM to the controller's I2C lines. Setting this line low connects the EEPROM to the I2C bus, A 10K resistor pulls this line high when inactive. */
#define EEPROM_CONNECT_PORT		0
#define EEPROM_CONNECT_PIN		2

/** This pin connects the reset line on the USB hub. Setting this line low resets the USB hub, A 10K resistor pulls this line high when inactive. */
#define USB_HUB_RESET_PORT		0
#define USB_HUB_RESET_PIN		21

/** This pin enables the ADG804 analog multiplexer. Setting this line high enables the mux, setting this line low disables the mux */
#define MUX_ENABLE_PORT			1
#define MUX_ENABLE_PIN			16

/** The first (lower) bit of the ADG804 analog multiplexer address. */
#define MUX_A0_PORT				1
#define MUX_A0_PIN				19

/** Fault input from the STMPS2151STR power switch that enables the analog power. The switch pulls this line low to indicate a fault in the switch. This line should be pulled high by the microcontroller. */
#define ANALOG_PWR_FLT_PORT		1
#define ANALOG_PWR_FLT_PIN		20

/** This pin connects to the write protect pin on the USB Hub EEPROM. Setting this line low disables the write protection, A 10K resistor pulls this line high when inactive. */
#define EEPROM_WP_PORT			1
#define EEPROM_WP_PIN			23

/** The second (upper) bit of the ADG804 analog multiplexer address. */
#define MUX_A1_PORT				1
#define MUX_A1_PIN				25

/** */
#define MCLK_PORT				1
#define MCLK_PIN				26

/** Switches the STMPS2151STR power switch on or off.  Set this line high to enable power to the analog system. */
#define ANALOG_PWR_EN_PORT		1
#define ANALOG_PWR_EN_PIN		27


//Initialize the pins
void App_HWInit(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, EEPROM_CONNECT_PORT, EEPROM_CONNECT_PIN, IOCON_FUNC0);			//Setup pin 0.2 for GPIO as EEPROM connect



	Chip_IOCON_PinMuxSet(LPC_IOCON, EEPROM_WP_PORT, EEPROM_WP_PIN, IOCON_FUNC0);					//Set pin 1.23 for GPIO as EEPROM Write Protect



	Chip_IOCON_PinMuxSet(LPC_IOCON, USB_HUB_RESET_PORT, USB_HUB_RESET_PIN, IOCON_FUNC0);			//Set pin 0.21 for GPIO as USB hub reset



	//Set up pins for the STMPS2151STR power switch
	//Enable pin, initalize to disabled
	Chip_IOCON_PinMuxSet(LPC_IOCON, ANALOG_PWR_EN_PORT, ANALOG_PWR_EN_PIN, IOCON_FUNC0);			//Set pin as GPIO
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, ANALOG_PWR_EN_PORT, ANALOG_PWR_EN_PIN);						//Set pin as output
	Chip_GPIO_SetPinOutLow(LPC_GPIO, ANALOG_PWR_EN_PORT, ANALOG_PWR_EN_PIN);						//Initialize pin low

	//Fault input
	Chip_IOCON_PinMuxSet(LPC_IOCON, ANALOG_PWR_FLT_PORT, ANALOG_PWR_FLT_PIN, (IOCON_FUNC0 | IOCON_MODE_PULLUP) );			//Set pin as GPIO, pulled up
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, ANALOG_PWR_FLT_PORT, ANALOG_PWR_FLT_PIN);											//Set pin as input
	//TODO: put this into the NVIC

	//Set up pins for the ADG804 analog multiplexer
	//Enable pin, initialize to disabled
	Chip_IOCON_PinMuxSet(LPC_IOCON, MUX_ENABLE_PORT, MUX_ENABLE_PIN, IOCON_FUNC0);					//Set pin as GPIO
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, MUX_ENABLE_PORT, MUX_ENABLE_PIN);							//Set pin as output
	Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_ENABLE_PORT, MUX_ENABLE_PIN);								//Initialize pin low

	//Address pins, initialize to 00
	Chip_IOCON_PinMuxSet(LPC_IOCON, MUX_A0_PORT, MUX_A0_PIN, IOCON_FUNC0);							//Set pin as GPIO
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, MUX_A0_PORT, MUX_A0_PIN);									//Set pin as output
	Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_A0_PORT, MUX_A0_PIN);										//Initialize pin low

	Chip_IOCON_PinMuxSet(LPC_IOCON, MUX_A1_PORT, MUX_A1_PIN, IOCON_FUNC0);							//Set pin as GPIO
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, MUX_A1_PORT, MUX_A1_PIN);									//Set pin as output
	Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_A1_PORT, MUX_A1_PIN);										//Initialize pin low

	return;
}


uint8_t ProgramStatus;
//uint8_t ButtonWaiting;

/*void App_SSP_Init (void)
{
	//Setup pin muxing for SPI pins.
	//Note: CS pins are not set up here
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, (IOCON_FUNC2 | IOCON_MODE_PULLUP));	/* SCK0 */
	//Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 8, (IOCON_FUNC1 | IOCON_MODE_PULLUP));	/* MISO0 */
	//Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 9, (IOCON_FUNC1 | IOCON_MODE_PULLUP));	/* MOSI0 */

	/*Chip_SSP_Init(LPC_SSP0);
	Chip_SSP_SetFormat(LPC_SSP0, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA1_CPOL1);
	Chip_SSP_SetMaster(LPC_SSP0, 1);		//Set to master mode
	Chip_SSP_SetBitRate(LPC_SSP0, 8000000);	//Set SSP0 frequency to 8MHz
	Chip_SSP_Enable(LPC_SSP0);

	return;
}*/

void App_Analog_Power_Enable(uint8_t toEnable)
{
	if(toEnable == 1)
	{
		Chip_GPIO_SetPinOutHigh(LPC_GPIO, ANALOG_PWR_EN_PORT, ANALOG_PWR_EN_PIN);
	}
	else
	{
		Chip_GPIO_SetPinOutLow(LPC_GPIO, ANALOG_PWR_EN_PORT, ANALOG_PWR_EN_PIN);
	}
	return;
}

void App_ADG804_Enable(uint8_t toEnable)
{
	if(toEnable == 1)
	{
		Chip_GPIO_SetPinOutHigh(LPC_GPIO, MUX_ENABLE_PORT, MUX_ENABLE_PIN);
	}
	else
	{
		Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_ENABLE_PORT, MUX_ENABLE_PIN);
	}
	return;
}

void App_ADG804_SetAddress(uint8_t AddressToSet)
{
	switch (AddressToSet)
	{
		case 0:
			Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_A0_PORT, MUX_A0_PIN);
			Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_A1_PORT, MUX_A1_PIN);
			break;

		case 1:
			Chip_GPIO_SetPinOutHigh(LPC_GPIO, MUX_A0_PORT, MUX_A0_PIN);
			Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_A1_PORT, MUX_A1_PIN);
			break;

		case 2:
			Chip_GPIO_SetPinOutLow(LPC_GPIO, MUX_A0_PORT, MUX_A0_PIN);
			Chip_GPIO_SetPinOutHigh(LPC_GPIO, MUX_A1_PORT, MUX_A1_PIN);
			break;

		case 3:
			Chip_GPIO_SetPinOutHigh(LPC_GPIO, MUX_A0_PORT, MUX_A0_PIN);
			Chip_GPIO_SetPinOutHigh(LPC_GPIO, MUX_A1_PORT, MUX_A1_PIN);
			break;
	}
	return;
}


void App_USBEEPROM_Connect(uint8_t toConnect)
{

	if(toConnect == 1)
	{
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 2);
		Chip_GPIO_SetPinOutLow(LPC_GPIO, 0, 2);
	}
	else
	{
		//An external pullup resistor pulls the line high to disconnect the USB EEPROM bus from the CPU EEPROM bus
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 2);
	}
	return;
}

void App_USBEEPROM_WP(uint8_t WPState)
{

	if(WPState == 0)
	{
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 23);
		Chip_GPIO_SetPinOutLow(LPC_GPIO, 1, 23);
	}
	else
	{
		//An external pullup resistor pulls the line high to write protect the USB EEPROM
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 23);
	}
	return;
}

void App_USBHUB_Reset(uint8_t ToReset)
{
	if(ToReset == 1)
	{
		//Low to reset the USB hub
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 21);
		Chip_GPIO_SetPinOutLow(LPC_GPIO, 0, 21);
	}
	else
	{
		//An external pullup resistor pulls the line high to unreset the hub
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 21);
	}
	return;
}

int16_t App_USBEEPROM_Init_Program(void)
{
	//Program the initial values to the USB EEPROM
	//This should only need to be called once

	uint8_t j;
	int16_t i;
	uint8_t ReadByte;
	uint8_t USB_EEPROM_InitData[17] = {0x24, 0x04, 0x12, 0x25, 0xB3, 0x0B, 0x1B, 0x20, 0x02, 0x00, 0x00, 0x00, 0x01, 0x32, 0x01, 0x32, 0x32};
									//  {0x24, 0x04, 0x12, 0x25, 0xB3, 0x0B, 0x1B, 0x20, 0x02, 0x00, 0x00, 0x00, 0x01, 0x32, 0x01, 0x32, 0x32};
	//uint8_t USB_EEPROM_InitData[17] =   {0x24, 0x04, 0x12, 0x25, 0xB3, 0x0B, 0x13, 0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x32, 0x01, 0x32, 0x32};


	for(j=0;j<4;j++)
	{

		App_USBHUB_Reset(1);		//Reset the hub to free up the I2C interface
		App_USBEEPROM_WP(0);		//Turn off the write protect on the EEPROM chip
		App_USBEEPROM_Connect(1);	//Connect the chip I2C bus to the EEPROM chip

		//Initialize the entire EEPROM bank to zero
		for(i=0; i<0xFF; i++)
		{
			EEPROM_24LV08B_WriteByte(j, i, 0x00);
			EEPROM_24LV08B_WaitReady();
		}

		for(i=0; i<0xFF; i++)
		{
			EEPROM_24LV08B_ReadByte(j, i, &ReadByte);
			if(ReadByte != 0x00)
			{
				//Error when clearing EEPROM
				printf("Error clearing EEPROM");
				return i;
			}
		}
		//printf("EEPROM Cleared");

		//Write initial values to the relevant addresses
		for(i=0;i<17;i++)
		{
			//printf("Write 0x%02 to address 0x%02X\r\n", );
			EEPROM_24LV08B_WriteByte(j, i, (USB_EEPROM_InitData[i]));
			EEPROM_24LV08B_WaitReady();
		}

		//Verify the initial values in EEPROM
		for(i=0;i<17;i++)
		{
			EEPROM_24LV08B_ReadByte(j, i, &ReadByte);
			if(ReadByte != USB_EEPROM_InitData[i]) //((~USB_EEPROM_InitData[i])& 0xFF) )
			{
				//Error writing initial values to EEPROM
				printf("Incorrect data at address 0x%02X, read 0x%02X, expected 0x%02X\r\n", i, ReadByte, ((~USB_EEPROM_InitData[i])& 0xFF));
				return i;
			}
		}

		//Write other values
		EEPROM_24LV08B_WriteByte(j, 0xFA, (0x06));
		EEPROM_24LV08B_WaitReady();
		EEPROM_24LV08B_ReadByte(j, 0xFA, &ReadByte);
		if(ReadByte != (0x06))
		{
			//Error writing inital values to EEPROM
			printf("Incorrect data at address 0xFA, read 0x%02X, expected 0x%02X\r\n", ReadByte, (0x06));
			return 0xFA;
		}

		App_USBEEPROM_Connect(0);
		App_USBEEPROM_WP(1);
		App_USBHUB_Reset(0);

	}

	return -1;
}

uint8_t App_USBEEPROM_ProgramByte(uint8_t AddressToProgram, uint8_t ByteToWrite)
{
	uint8_t ReadByte;

	App_USBHUB_Reset(1);		//Reset the hub to free up the I2C interface
	App_USBEEPROM_WP(0);		//Turn off the write protect on the EEPROM chip
	App_USBEEPROM_Connect(1);	//Connect the chip I2C bus to the EEPROM chip

	//Write the byte to the EEPROM
	EEPROM_24LV08B_WriteByte(0, AddressToProgram, ByteToWrite);
	EEPROM_24LV08B_WaitReady();

	//Verify the byte has been written
	EEPROM_24LV08B_ReadByte(0, AddressToProgram, &ReadByte);
	if(ReadByte != ByteToWrite)
	{
		return 0xFF;	//Error writing to the EEPROM (TODO: make it retry?)
	}

	//Disconnect from the EEPROM and renable the USB hub
	App_USBEEPROM_Connect(0);
	App_USBEEPROM_WP(1);
	App_USBHUB_Reset(0);

	return 0x00;
}

uint8_t App_USBEEPROM_ReadByte(uint8_t AddressToRead)
{
	uint8_t ReadByte;

	App_USBHUB_Reset(1);		//Reset the hub to free up the I2C interface
	App_USBEEPROM_WP(0);		//Turn off the write protect on the EEPROM chip
	App_USBEEPROM_Connect(1);	//Connect the chip I2C bus to the EEPROM chip

	EEPROM_24LV08B_ReadByte(0, AddressToRead, &ReadByte);

	App_USBEEPROM_Connect(0);
	App_USBEEPROM_WP(1);
	App_USBHUB_Reset(0);

	return ReadByte;
}

uint8_t App_USBEEPROM_Program(uint8_t StartAddress, uint8_t BytesToWrite, uint8_t *DataArray)
{
	uint8_t i;
	uint8_t ReadByte;

	App_USBHUB_Reset(1);		//Reset the hub to free up the I2C interface
	App_USBEEPROM_WP(0);		//Turn off the write protect on the EEPROM chip
	App_USBEEPROM_Connect(1);	//Connect the chip I2C bus to the EEPROM chip

	for(i=0; i<BytesToWrite; i++)
	{
		EEPROM_24LV08B_WriteByte(0, i, DataArray[i]);
		EEPROM_24LV08B_WaitReady();
	}

	for(i=0; i<BytesToWrite; i++)
	{
		EEPROM_24LV08B_ReadByte(0, i, &ReadByte);
		if(ReadByte != DataArray[i])
		{
			return i;
		}
	}

	App_USBEEPROM_Connect(0);
	App_USBEEPROM_WP(1);
	App_USBHUB_Reset(0);

	//Return 0xFF if it worked
	return 0xFF;
}

void App_Button_Init(void)
{
	//ButtonWaiting = 0;

	//Initialize the timer for the debouncing, but don't start it
	/*Chip_TIMER_Init(DEBOUNCE_TIMER);
	Chip_TIMER_Reset(DEBOUNCE_TIMER);

	DEBOUNCE_TIMER->PR = 100;
	DEBOUNCE_TIMER->MCR = (1<<1)|(1<<0);		//Enable MR0 match interrupt, Reset TC on MR0 match
	DEBOUNCE_TIMER->MR[0]= 0xFFFF;				//MR0 match value

	//Enable the IRQ for the timer
	NVIC_EnableIRQ(DEBOUNCE_TIMER_NVIC_NAME);

	//Set all button pins to GPIO input with pullup
	Chip_IOCON_PinMuxSet(LPC_IOCON, BUTTON_1_PORT, BUTTON_1_PIN, (IOCON_FUNC0 | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, BUTTON_2_PORT, BUTTON_2_PIN, (IOCON_FUNC0 | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, BUTTON_3_PORT, BUTTON_3_PIN, (IOCON_FUNC0 | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, BUTTON_4_PORT, BUTTON_4_PIN, (IOCON_FUNC0 | IOCON_MODE_PULLUP));
	Chip_IOCON_PinMuxSet(LPC_IOCON, BUTTON_5_PORT, BUTTON_5_PIN, (IOCON_FUNC0 | IOCON_MODE_PULLUP));

	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BUTTON_1_PORT, BUTTON_1_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BUTTON_2_PORT, BUTTON_2_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BUTTON_3_PORT, BUTTON_3_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BUTTON_4_PORT, BUTTON_4_PIN);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BUTTON_5_PORT, BUTTON_5_PIN);
*/

	//TODO: Probably put this in the main initalization...
	//Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PINT);

	//Setup GPIO interrupts for each button

	/* Configure interrupt channel for the GPIO pin in SysCon block */
	/*Chip_SYSCTL_SetPinInterrupt(BUTTON_1_PININT_INDEX, BUTTON_1_PORT, BUTTON_1_PIN);
	Chip_SYSCTL_SetPinInterrupt(BUTTON_2_PININT_INDEX, BUTTON_2_PORT, BUTTON_2_PIN);
	Chip_SYSCTL_SetPinInterrupt(BUTTON_3_PININT_INDEX, BUTTON_3_PORT, BUTTON_3_PIN);
	Chip_SYSCTL_SetPinInterrupt(BUTTON_4_PININT_INDEX, BUTTON_4_PORT, BUTTON_4_PIN);
	Chip_SYSCTL_SetPinInterrupt(BUTTON_5_PININT_INDEX, BUTTON_5_PORT, BUTTON_5_PIN);
*/
	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	/*Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(BUTTON_1_PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(BUTTON_2_PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(BUTTON_3_PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(BUTTON_4_PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(BUTTON_5_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(BUTTON_1_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(BUTTON_2_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(BUTTON_3_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(BUTTON_4_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(BUTTON_5_PININT_INDEX));

	Chip_PININT_ClearIntStatus(LPC_PININT, ((1 << BUTTON_1_PININT_INDEX)|(1<<BUTTON_2_PININT_INDEX)|(1<<BUTTON_3_PININT_INDEX)|(1<<BUTTON_4_PININT_INDEX)|(1<<BUTTON_5_PININT_INDEX)) );

	App_EnableButtons();*/

	return;
}

void App_EnableButtons(void)
{
	/*Chip_PININT_ClearIntStatus(LPC_PININT, ((1 << BUTTON_1_PININT_INDEX)|(1<<BUTTON_2_PININT_INDEX)|(1<<BUTTON_3_PININT_INDEX)|(1<<BUTTON_4_PININT_INDEX)|(1<<BUTTON_5_PININT_INDEX)) );

	NVIC_ClearPendingIRQ(BUTTON_1_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_2_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_3_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_4_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_5_NVIC_NAME);

	NVIC_EnableIRQ(BUTTON_1_NVIC_NAME);
	NVIC_EnableIRQ(BUTTON_2_NVIC_NAME);
	NVIC_EnableIRQ(BUTTON_3_NVIC_NAME);
	NVIC_EnableIRQ(BUTTON_4_NVIC_NAME);
	NVIC_EnableIRQ(BUTTON_5_NVIC_NAME);*/
	return;
}

void App_DisableButtons(void)
{
	/*NVIC_DisableIRQ(BUTTON_1_NVIC_NAME);
	NVIC_DisableIRQ(BUTTON_2_NVIC_NAME);
	NVIC_DisableIRQ(BUTTON_3_NVIC_NAME);
	NVIC_DisableIRQ(BUTTON_4_NVIC_NAME);
	NVIC_DisableIRQ(BUTTON_5_NVIC_NAME);

	Chip_PININT_ClearIntStatus(LPC_PININT, ((1 << BUTTON_1_PININT_INDEX)|(1<<BUTTON_2_PININT_INDEX)|(1<<BUTTON_3_PININT_INDEX)|(1<<BUTTON_4_PININT_INDEX)|(1<<BUTTON_5_PININT_INDEX)) );

	NVIC_ClearPendingIRQ(BUTTON_1_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_2_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_3_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_4_NVIC_NAME);
	NVIC_ClearPendingIRQ(BUTTON_5_NVIC_NAME);*/

	return;
}

/**Called in the interrupt to handle a button press
 *   To debounce the buttons, the button interrupt is set to trigger on a high-to-low interrupt. When the interrupt triggers, the button number
 *   that triggered the interrupt is saved into the global 'ButtonWaiting' variable. The button interrupts are disabled and the debounce timer
 *   is started. When the debounce timer interrupts (based on the DEBOUNCE_TIME global define) the state of the button is polled again. If the
 *   button is still low, the device treats the button press as legitimate and passes the button press to the RTOS. If the button interrupts
 *   are then reenabled and the device starts to wait for another button press.
 */
void App_HandleButtonPress(void)
{
	/*if(App_GetStatus() != APP_STATUS_INIT)
	{
		DisplayCommand CommandToSend;

		if( ((ButtonWaiting == BUTTON_1_PININT_INDEX) &&  (Chip_GPIO_GetPinState(LPC_GPIO, BUTTON_1_PORT, BUTTON_1_PIN) == false)) ||
			((ButtonWaiting == BUTTON_2_PININT_INDEX) &&  (Chip_GPIO_GetPinState(LPC_GPIO, BUTTON_2_PORT, BUTTON_2_PIN) == false)) ||
			((ButtonWaiting == BUTTON_3_PININT_INDEX) &&  (Chip_GPIO_GetPinState(LPC_GPIO, BUTTON_3_PORT, BUTTON_3_PIN) == false)) ||
			((ButtonWaiting == BUTTON_4_PININT_INDEX) &&  (Chip_GPIO_GetPinState(LPC_GPIO, BUTTON_4_PORT, BUTTON_4_PIN) == false)) ||
			((ButtonWaiting == BUTTON_5_PININT_INDEX) &&  (Chip_GPIO_GetPinState(LPC_GPIO, BUTTON_5_PORT, BUTTON_5_PIN) == false)))
		{
			CommandToSend.CommandName = OLED_CMD_BUTTON_IN;
			CommandToSend.CommandData[0] = ButtonWaiting;
			xQueueSendFromISR(xDisplayCommands, (void *)&CommandToSend, NULL);
		}
	}
	else
	{
		App_EnableButtons();
	}*/
	return;
}

void App_SetStatus(uint8_t Status)
{
	ProgramStatus = Status;

	/*if( EEPROM_Write(EEPROM_ADDRESS_HW_STATUS, &Status, 1 ) !=0)
	{	//Failed to write to EEPROM
		App_Die(8);
	}*/

	return;
}

uint8_t App_GetStatus(void)
{
	return ProgramStatus;
}

//#if defined(DEBUG_UART)
void UART_IRQHandler(void)
{
	uint32_t UART_IntStatus;
	uint8_t UART_RecieveByteVal;

	Board_LED_Set(2, true);
	Board_LED_Set(1, false);

	UART_IntStatus = Chip_UART_ReadIntIDReg(LPC_USART);
	if( (UART_IntStatus & 0x01) == 0x00)
	{
		if((UART_IntStatus & UART_IIR_INTID_MASK) == UART_IIR_INTID_RDA)
		{
			//TODO: Rename this queue later
			UART_RecieveByteVal = Chip_UART_ReadByte(LPC_USART);
			xQueueSendFromISR(xUSBCharReceived, &UART_RecieveByteVal, NULL);
		}
	}
}
//#endif
