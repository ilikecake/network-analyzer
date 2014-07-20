/*
 * hardware.h
 *
 *  Created on: Dec 13, 2013
 *      Author: pat
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_


//Pick more statuses later...
//Status should include error states and also normal program states
#define APP_STATUS_INIT			0x00
#define APP_STATUS_OK			0x01
#define APP_STATUS_OSC_STOPPED	0x02


//TODO: Add a return to defaults function that sets all the EEPROM setup values to their default states.

void App_SSP_Init (void);

void App_HWInit(void);

void App_USBEEPROM_Connect(uint8_t toConnect);
void App_USBEEPROM_WP(uint8_t WPState);

void App_USBHUB_Reset(uint8_t ToReset);

uint8_t App_USBEEPROM_ProgramByte(uint8_t AddressToProgram, uint8_t ByteToWrite);
uint8_t App_USBEEPROM_ReadByte(uint8_t AddressToRead);


uint8_t App_USBEEPROM_Program(uint8_t StartAddress, uint8_t BytesToWrite, uint8_t *DataArray);
int16_t App_USBEEPROM_Init_Program(void);

void App_SetStatus(uint8_t Status);
uint8_t App_GetStatus(void);

//TODO: Add a function to enable and disable the interrupts
void App_Button_Init(void);
void App_HandleButtonPress(void);

void App_EnableButtons(void);
void App_DisableButtons(void);



void App_Buzzer_Init(void);
void App_Buzzer_on (void);
void App_Buzzer_off (void);

void App_InitializeFromEEPROM(void);



void App_Die(uint8_t ErrorCode);

#endif /* HARDWARE_H_ */
