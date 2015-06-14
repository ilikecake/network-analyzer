/*   This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file
*	\brief		Command interpreter application specific functions
*	\author		Pat Satyshur
*	\version	1.1
*	\date		1/13/2013
*	\copyright	Copyright 2013, Pat Satyshur
*
*	@{
*/

#include "main.h"
//#include "commands.h"
//#include <stdio.h>
//#include "board.h"

//The number of commands
const uint8_t NumCommands = 9;

//Handler function declarations

//LED control function
static int _F1_Handler (void);
const char _F1_NAME[] 			= "led";
const char _F1_DESCRIPTION[] 	= "Turn LED on or off";
const char _F1_HELPTEXT[] 		= "led <number>";

//DAC commands
static int _F2_Handler (void);
const char _F2_NAME[] 			= "cpu";
const char _F2_DESCRIPTION[] 	= "cpu commands";
const char _F2_HELPTEXT[] 		= "cpu <1> <2>";


//Get time from RTC
static int _F3_Handler (void);
const char _F3_NAME[]  			= "i2cscan";
const char _F3_DESCRIPTION[]  	= "Scan the I2C Bus";
const char _F3_HELPTEXT[]  		= "'i2cscan' has no parameters";


//Set time on RTC
static int _F4_Handler (void);
const char _F4_NAME[]  			= "uep";
const char _F4_DESCRIPTION[]  	= "USB EEPROM functions";
const char _F4_HELPTEXT[]  		= "nothing yet";


//OLED functions
static int _F5_Handler (void);
const char _F5_NAME[] 			= "prog";
const char _F5_DESCRIPTION[] 	= "USB EEPROM Programming";
const char _F5_HELPTEXT[]		= "prog <addr> <val>";

//Buzzer functions
static int _F6_Handler (void);
const char _F6_NAME[] 			= "beep";
const char _F6_DESCRIPTION[] 	= "Buzzer Functions";
const char _F6_HELPTEXT[]		= "beep <time>";

//EEPROM functions
static int _F7_Handler (void);
const char _F7_NAME[]			= "eeprom";
const char _F7_DESCRIPTION[]	= "EEPROM functions";
const char _F7_HELPTEXT[] 		= "eeprom <1> <2>";

//Timer functions
static int _F8_Handler (void);
const char _F8_NAME[]			= "timer";
const char _F8_DESCRIPTION[] 	= "Timer functions";
const char _F8_HELPTEXT[] 		= "timer <1>";

//List or update events
static int _F9_Handler (void);
const char _F9_NAME[] 			= "event";
const char _F9_DESCRIPTION[]	= "List or update events";
const char _F9_HELPTEXT[]		= "event <cmd> <data...>";

/*

//Get temperatures from the ADC
static int _F11_Handler (void);
const char _F11_NAME[] PROGMEM 			= "temp";
const char _F11_DESCRIPTION[] PROGMEM 	= "Get temperatures from the ADC";
const char _F11_HELPTEXT[] PROGMEM 		= "'temp' has no parameters";

//Scan the TWI bus for devices
static int _F12_Handler (void);
const char _F12_NAME[] PROGMEM 			= "twiscan";
const char _F12_DESCRIPTION[] PROGMEM 	= "Scan for TWI devices";
const char _F12_HELPTEXT[] PROGMEM 		= "'twiscan' has no parameters";

//Dataflash functions
static int _F13_Handler (void);
const char _F13_NAME[] PROGMEM 			= "mem";
const char _F13_DESCRIPTION[] PROGMEM 	= "dataflash functions";
const char _F13_HELPTEXT[] PROGMEM 		= "mem <1> <2> <3>";
*/

//Command list
const CommandListItem AppCommandList[] =
{
	{ _F1_NAME,		1,  2,	_F1_Handler,	_F1_DESCRIPTION,	_F1_HELPTEXT	},		//led
	{ _F2_NAME, 	0,  2,	_F2_Handler,	_F2_DESCRIPTION,	_F2_HELPTEXT	},		//cpu
	{ _F3_NAME, 	0,  0,	_F3_Handler,	_F3_DESCRIPTION,	_F3_HELPTEXT	},		//i2cscan
	{ _F4_NAME, 	2,  5,	_F4_Handler,	_F4_DESCRIPTION,	_F4_HELPTEXT	},		//time
	{ _F5_NAME, 	1,  2,	_F5_Handler,	_F5_DESCRIPTION,	_F5_HELPTEXT	},		//prog
	{ _F6_NAME, 	0,  1,	_F6_Handler,	_F6_DESCRIPTION,	_F6_HELPTEXT	},		//beep
	{ _F7_NAME, 	2,  2,	_F7_Handler,	_F7_DESCRIPTION,	_F7_HELPTEXT	},		//eeprom
	{ _F8_NAME,		1,  1,	_F8_Handler,	_F8_DESCRIPTION,	_F8_HELPTEXT	},		//timer
	{ _F9_NAME,		1,  8,	_F9_Handler,	_F9_DESCRIPTION,	_F9_HELPTEXT	},		//event

	/*
	{ _F10_NAME,	0,  0,	_F10_Handler,	_F10_DESCRIPTION,	_F10_HELPTEXT	},		//cal
	{ _F11_NAME,	0,  0,	_F11_Handler,	_F11_DESCRIPTION,	_F11_HELPTEXT	},		//temp
	{ _F12_NAME,	0,  0,	_F12_Handler,	_F12_DESCRIPTION,	_F12_HELPTEXT	},		//twiscan
	{ _F13_NAME,	1,  3,	_F13_Handler,	_F13_DESCRIPTION,	_F13_HELPTEXT	},		//twiscan
	*/
};

//Command functions

//LED control function
static int _F1_Handler (void)
{
	if(NumberOfArguments() == 2)
	{
		Board_LED_Set(argAsInt(1), argAsInt(2));
	}
	else
	{
		if(argAsInt(1) == 1)
		{
			//Turn on all leds
			Board_LED_Set(1, 1);
			Board_LED_Set(2, 1);
			Board_LED_Set(3, 1);
		}
		else
		{
			//Turn off all leds
			Board_LED_Set(1, 0);
			Board_LED_Set(2, 0);
			Board_LED_Set(3, 0);
		}

	}
	return 0;
}

//cpu
static int _F2_Handler (void)
{
	uint32_t tempVal[4];
	uint8_t resp;

	if (NumberOfArguments() == 0)
	{
		//Output some useful info
		printf("Main Clk: %u Hz\r\n", Chip_Clock_GetMainClockRate());
		printf("SSP0 Clk div: %u\r\n", Chip_Clock_GetSSP0ClockDiv());
		printf("Main Clk: %u Hz\r\n", SystemCoreClock);

		resp = ReadUID(tempVal);
		if(resp == IAP_CMD_SUCCESS)
		{
			printf("UID: 0x%08lX %08lX %08lX %08lX\r\n", tempVal[0], tempVal[1], tempVal[2], tempVal[3]);
		}

		resp = ReadPartID(tempVal);
		if(resp == IAP_CMD_SUCCESS)
		{
			printf("Part ID: 0x%08lX\r\n", tempVal[0]);
		}

		resp = ReadBootVersion(tempVal);
		if(resp == IAP_CMD_SUCCESS)
		{
			printf("Boot Code Version: %u.%u\r\n", (uint8_t)((tempVal[0]>>8)&0xFF), (uint8_t)(tempVal[0]&0xFF));
		}

		printf("----------------------------\r\n");
		printf("Task Name\tStack Usage\r\n");
		printf("----------------------------\r\n");
		printf("vTaskLed1\t%u/64\r\n", 64-uxTaskGetStackHighWaterMark(TaskList[0]));
		printf("vConsole\t%u/300\r\n", 300-uxTaskGetStackHighWaterMark(TaskList[1]));
		printf("vOLEDTask\t%u/200\r\n", 200-uxTaskGetStackHighWaterMark(TaskList[2]));
		printf("vTimer\t\t%u/150\r\n", 150-uxTaskGetStackHighWaterMark(TaskList[3]));
		printf("----------------------------\r\n");
		printf("Free Heap Space: %u\r\n", xPortGetFreeHeapSize());

		printf("Compile Time: %s, %s\r\n", __DATE__, __TIME__);
	}

	return 0;
}


//i2cscan
static int _F3_Handler (void)
{
	i2c_probe_slaves(I2C0);
	return 0;
}

//uep
static int _F4_Handler (void)
{
	uint8_t val;
	uint8_t val2;
	uint8_t val3;

	uint8_t USB_EEPROM_InitData[17] = {0x24, 0x04, 0x12, 0x25, 0xB3, 0x0B, 0x1B, 0x20, 0x02, 0x00, 0x00, 0x00, 0x01, 0x32, 0x01, 0x32, 0x32};
	int16_t i;
	uint8_t ReadByte;
	//uint8_t val4;

	val = argAsInt(2);


	switch(argAsInt(1))
	{
		case 1:
			App_USBEEPROM_Connect(val);
			if(val == 1)
			{
				printf("USB EEPROM connected to I2C bus.\r\n");
			}
			else
			{
				printf("USB EEPROM disconnected from I2C bus.\r\n");
			}

			break;

		case 2:
			App_USBEEPROM_WP(val);
			if(val == 1)
			{
				printf("USB EEPROM write protected.\r\n");
			}
			else
			{
				printf("USB EEPROM writable.\r\n");
			}
			break;

		case 3:
			val2 = argAsInt(3);
			val3 = argAsInt(4);
			EEPROM_24LV08B_WriteByte(val, val2, val3);
			printf("Write 0x%02X to block 0x%02X, address 0x%02X\r\n", val3, val, val2);
			break;

		case 4:
			val2 = argAsInt(3);
			EEPROM_24LV08B_ReadByte(val, val2, &val3);
			printf("Read 0x%02X from block 0x%02X, address 0x%02X\r\n", val3, val, val2);
			break;

		case 5:
			printf("Writing initial values to EEPROM...");
			for(i=0;i<17;i++)
			{
				EEPROM_24LV08B_WriteByte(0, i, USB_EEPROM_InitData[i]);
				EEPROM_24LV08B_WaitReady();
			}
			printf("Done.\r\n");

			printf("Verifying EEPROM...");
			for(i=0;i<17;i++)
			{
				EEPROM_24LV08B_ReadByte(0, i, &ReadByte);
				if(ReadByte != USB_EEPROM_InitData[i])
				{
					printf("Incorrect data at address 0x%02X\r\n", i);
				}
			}
			printf("Done.\r\n");
			break;

		case 6:
			printf("Writing initial values to EEPROM...");
			i = App_USBEEPROM_Init_Program();
			if(i == -1)
			{
				printf("Done.\r\n");
			}
			else
			{
				printf("Error %d.\r\n", i);
			}

		case 7:
			App_Analog_Power_Enable(val);
			if(val == 1)
			{
				printf("Analog Power On\r\n");
			}
			else
			{
				printf("Analog Power Off\r\n");
			}

			break;

		case 8:
			App_ADG804_Enable(val);
			if(val == 1)
			{
				printf("Mux Enabled (high)\r\n");
			}
			else
			{
				printf("Mux Disabled (low)\r\n");
			}

			break;

		case 9:
			App_ADG804_SetAddress(val);
			if((val >= 0) && (val < 4))
			{
				printf("Address set to %d\r\n", val);
			}
			else
			{
				printf("Invalid address\r\n");
			}
			break;




	}

	return 0;
}

//prog
static int _F5_Handler (void)
{
	uint8_t ReadVal;
	uint8_t AddressToModify = argAsInt(1);

	if (NumberOfArguments() == 2)
	{
		//Program a byte
		if(App_USBEEPROM_ProgramByte(AddressToModify, argAsInt(2)) == 0)
		{
			printf("Done\r\n");
		}
		else
		{
			printf("Error\r\n");
		}

	}
	else
	{
		//Read and display a byte
		ReadVal = App_USBEEPROM_ReadByte(AddressToModify);
		printf("Addresss: 0x%02X, Value: 0x%02X\r\n", AddressToModify, ReadVal);
	}

	return 0;
}



//Buzzer Functions
static int _F6_Handler (void)
{
	//double blarg = .5;
	uint8_t cmd;
	cmd = argAsInt(1);

	/*if(cmd == 1)
	{
		printf("Beep on\r\n");
		App_Buzzer_on();
	}
	else if(cmd == 0)
	{
		printf("Beep off\r\n");
		App_Buzzer_off();
	}
	else
	{
		//printf("sin(90) = %f\r\n", sin(90.0));
		//printf("sin(pi/2) = %f\r\n", sin(3.1415/2));
	}*/

	return 0;
}

//EEPROM Functions
static int _F7_Handler (void)
{
	uint8_t cmd;
	uint8_t val;
	uint8_t resp;
	uint32_t ver;

	int16_t LHS;
	uint16_t RHS;

	//uint8_t i;
	//TimerEvent TestEvent;

	unsigned int temp3[4];
	cmd = argAsInt(1);
	val = argAsInt(2);

	uint8_t DataArray[5];

	DataArray[0] = 0;
	DataArray[1] = 0;
	DataArray[2] = 0;
	DataArray[3] = 0;
	DataArray[4] = 0;

	switch(cmd)
	{
	case 0:
		ver = EELIB_getVersion();
		printf("Version: %ul\r\n", ver);
		break;

	case 1:
		printf("Reading 4 bytes from address %u\r\n", val);
		//resp = EEPROM_Read((uint8_t*)val, (uint8_t*)&ver, 4);
		resp = EEPROM_Read(val, &DataArray, 4);
		printf("Response is %u\r\n", resp);
		//printf("data: 0x%08lX\r\n", ver);
		printf("data[0]: 0x%02X\r\n", DataArray[0]);
		printf("data[1]: 0x%02X\r\n", DataArray[1]);
		printf("data[2]: 0x%02X\r\n", DataArray[2]);
		printf("data[3]: 0x%02X\r\n", DataArray[3]);
		break;

	case 2:
		DataArray[0] = 0xFA;
		DataArray[1] = 0xFF;
		DataArray[2] = 0x10;
		DataArray[3] = 0xFA;

		ver = 0xFAFF10FB;

		printf("writing 4 bytes starting at %u\r\n", val);
		resp = EEPROM_Write(val, &DataArray, 4);
		printf("Response is %u\r\n", resp);
		break;

	case 3:
		resp = ReadUID(temp3);
		if(resp == IAP_CMD_SUCCESS)
		{
			printf("UID: 0x%08lX %08lX %08lX %08lX\r\n", temp3[0], temp3[1], temp3[2], temp3[3]);
		}
		break;

	case 4:
		resp = ReadPartID(&ver);
		if(resp == IAP_CMD_SUCCESS)
		{
			printf("PID: 0x%08lX\r\n", ver);
		}
		break;

	case 5:
		resp = ReadBootVersion(&ver);
		if(resp == IAP_CMD_SUCCESS)
		{
			printf("BV: %u.%u\r\n", (uint8_t)((ver>>8)&0xFF), (uint8_t)(ver&0xFF));
		}
		break;

	case 6:
		printf("Calling reinvoke ISP...\r\n");
		RequestISP();
		//ReinvokeISP();
		break;

	/*case 7:
		printf("Writing to EEPROM...\r\n");
		TestEvent.EventType = 0xA2;
		TestEvent.EventOutputState = 0x01;
		TestEvent.EventTime[0] = 0x0A;
		TestEvent.EventTime[1] = 0xFF;
		TestEvent.EventTime[2] = 0xCA;
		resp = EEPROM_Write(0, &TestEvent, sizeof(TestEvent));

		//resp = EEPROM_Write((uint8_t*)0, (uint8_t*)&TestEvent, sizeof(TestEvent));
		printf("Response is %u\r\n", resp);
		break;

	case 8:
		TestEvent.EventType = 0x00;
		TestEvent.EventOutputState = 0x00;
		TestEvent.EventTime[0] = 0x20;
		TestEvent.EventTime[2] = 0x00;
		TestEvent.EventTime[3] = 0x00;

		printf("Reading from EEPROM...\r\n");
		resp = EEPROM_Read(0, &TestEvent, sizeof(TestEvent));
		//resp = EEPROM_Read((uint8_t*)0, (uint8_t*)&TestEvent, sizeof(TestEvent));
		printf("Response is %u\r\n", resp);

		printf("EventType: 0x%02X\r\n", TestEvent.EventType);
		printf("EventOutput: 0x%02X\r\n", TestEvent.EventOutputState);
		printf("EventTime[0]: 0x%02X\r\n", TestEvent.EventTime[0]);
		printf("EventTime[1]: 0x%02X\r\n", TestEvent.EventTime[1]);
		printf("EventTime[2]: 0x%02X\r\n", TestEvent.EventTime[2]);
		break;*/

		/*
	case 9:
		TimerReadEventsFromEEPROM();
		break;

	case 10:
		TimerReadSingleEventFromEEPROM(1, 3, &TestEvent);
		TimerReadSingleEventFromEEPROM(0, 4, &TestEvent);
		TimerReadSingleEventFromEEPROM(3, 1, &TestEvent);
		break;

	case 11:
		printf("Writing to EEPROM...\r\n");
		TestEvent.EventType = 0xA2;
		TestEvent.EventOutputState = 0x01;
		TestEvent.EventTime[0] = 0x0A;
		TestEvent.EventTime[1] = 0xFF;
		TestEvent.EventTime[2] = 0xCA;
		resp = TimerWriteSingleEventToEEPROM(1, 3, &TestEvent);
		printf("Response is %u\r\n", resp);
		break;

	case 12:
		TestEvent.EventType = 0;
		TestEvent.EventOutputState = 0;
		TestEvent.EventTime[0] = 0;
		TestEvent.EventTime[1] = 0;
		TestEvent.EventTime[2] = 0;
		printf("Reading from EEPROM...\r\n");
		resp = TimerReadSingleEventFromEEPROM(1, 3, &TestEvent);
		printf("Response is %u\r\n", resp);
		printf("EventType: 0x%02X\r\n", TestEvent.EventType);
		printf("EventOutput: 0x%02X\r\n", TestEvent.EventOutputState);
		printf("EventTime[0]: 0x%02X\r\n", TestEvent.EventTime[0]);
		printf("EventTime[1]: 0x%02X\r\n", TestEvent.EventTime[1]);
		printf("EventTime[2]: 0x%02X\r\n", TestEvent.EventTime[2]);
		break;

	case 13:
		TimerWriteEventsToEEPROM();
		break;

	case 14:
		GetLatitude(&LHS, &RHS);
		printf("LAT: %d.%04d\r\n", LHS, RHS);
		//printf("RHS: %d\r\n", RHS);

		GetLongitude(&LHS, &RHS);
		printf("LONG: %d.%04d\r\n", LHS, RHS);
		//printf("LHS: %d\r\n", LHS);
		//printf("RHS: %d\r\n", RHS);

		break;*/

	}

	return 0;
}

//Timer Functions
static int _F8_Handler (void)
{
	uint8_t cmd;
	cmd = argAsInt(1);
	//TimerEvent NewTimerEvent;

	/*

	struct tm CurrentTime;
	//TimeAndDate CurrentTime;


	DS3232M_GetTime(&CurrentTime);

	printf("Start Out: 0x%02X\r\n", TimerGetOutputState());

	switch (cmd)
	{
		case 1:
			//TimerTask();
			printf("Out: 0x%02X\r\n", TimerGetOutputState());
			break;

		case 2:
			NewTimerEvent.EventType = TIMER_EVENT_TYPE_TIMED;
			NewTimerEvent.EventTime[0] = 0xFF;	//Trigger on all days of week
			NewTimerEvent.EventTime[1] = CurrentTime.tm_hour;
			NewTimerEvent.EventTime[2] = CurrentTime.tm_min+1;
			NewTimerEvent.EventOutputState = 1;
			TimerSetEvent(1, 1, &NewTimerEvent);

			NewTimerEvent.EventTime[2] = CurrentTime.tm_min+2;
			NewTimerEvent.EventOutputState = 0;
			TimerSetEvent(1, 2, &NewTimerEvent);

			break;

		case 3:
			TimerGetEvent(1, 1, &NewTimerEvent);
			printf("type: %u\r\n", NewTimerEvent.EventType);
			printf("time[0]: %u\r\n", NewTimerEvent.EventTime[0]);
			printf("time[1]: %u\r\n", NewTimerEvent.EventTime[1]);
			printf("time[2]: %u\r\n", NewTimerEvent.EventTime[2]);
			printf("state: %u\r\n", NewTimerEvent.EventOutputState);
			break;


	}
*/
	return 0;
}

static int _F9_Handler (void)
{
	uint8_t cmd;
	uint8_t i,j;

	/*TimerEvent EventData;

	char LineChar[] = "-----------------------------------------------\r\n";

	//argAsChar(uint8_t argNum, char *ArgString);

	cmd = argAsInt(1);

	switch (cmd)
	{
		case 1:
			printf("List Events from RAM:\r\n");
			for(i=0; i<TIMER_OUTPUT_NUMBER; i++)
			{
				printf(LineChar);
				printf("Output %u\r\n", i+1);
				printf(LineChar);
				printf("Event\tType\tTime[2]\tTime[1]\tTime[0]\tOutput\r\n");
				printf(LineChar);

				for(j=0; j<TIMER_EVENT_NUMBER; j++)
				{
					TimerGetEvent(i, j, &EventData);
					printf("%u\t%u\t%u\t%u\t%u\t%u\r\n", j, EventData.EventType, EventData.EventTime[2], EventData.EventTime[1], EventData.EventTime[0], EventData.EventOutputState);
				}
				printf(LineChar);
			}
			break;

		case 2:
			printf("List Events from EEPROM:\r\n");
			for(i=0; i<TIMER_OUTPUT_NUMBER; i++)
			{
				printf(LineChar);
				printf("Output %u\r\n", i+1);
				printf(LineChar);
				printf("Event\tType\tTime[2]\tTime[1]\tTime[0]\tOutput\r\n");
				printf(LineChar);

				for(j=0; j<TIMER_EVENT_NUMBER; j++)
				{
					if(TimerReadSingleEventFromEEPROM(i, j, &EventData) != 0)
					{
						printf("Read Error\r\n");
						return 0;
					}
					printf("%u\t%u\t%u\t%u\t%u\t%u\r\n", j, EventData.EventType, EventData.EventTime[2], EventData.EventTime[1], EventData.EventTime[0], EventData.EventOutputState);
				}
				printf(LineChar);
			}
			break;

		case 3:
			printf("Reload Events from EEPROM...");
			if(TimerReadEventsFromEEPROM() == 0x00)
			{
				printf("Done\r\n");
			}
			else
			{
				printf("Read Error\r\n");
			}
			break;

		case 4:
			printf("Write RAM event list to EEPROM...");
			if(TimerWriteEventsToEEPROM() == 0x00)
			{
				printf("Done\r\n");
			}
			else
			{
				printf("Write Error\r\n");
			}
			break;

		case 5:
			i = argAsInt(2);
			j = argAsInt(3);
			printf("Clear output %u event %u from RAM..", i, j);
			TimerClearRamEvent(i, j);
			printf("Done\r\n");
			break;

		case 6:
			i = argAsInt(2);
			j = argAsInt(3);

			EventData.EventType = argAsInt(4);
			EventData.EventTime[0] = argAsInt(5);
			EventData.EventTime[1] = argAsInt(6);
			EventData.EventTime[2] = argAsInt(7);
			EventData.EventOutputState = argAsInt(8);

			//printf("Set: %u, %u, %u, %u, %u\r\n",EventData.EventType, EventData.EventTime[0], EventData.EventTime[1], EventData.EventTime[2], EventData.EventOutputState);

			printf("Setting output %u event %u in RAM..", i, j);
			TimerSetEvent(i, j, &EventData);
			printf("Done\r\n");

			break;


	}

*/





	return 0;
}

/** @} */
