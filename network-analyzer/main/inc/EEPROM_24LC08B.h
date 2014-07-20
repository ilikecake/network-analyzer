/*
 * 24LC08B.h
 *
 *  Created on: Jun 23, 2014
 *      Author: pat
 */




#ifndef EEPROM_24LC08B_H_
#define EEPROM_24LC08B_H_

#include "stdint.h"

#define EEPROM_24LV08B_BLOCK0_ADDRESS		0x50
#define EEPROM_24LV08B_BLOCK1_ADDRESS		0x51
#define EEPROM_24LV08B_BLOCK2_ADDRESS		0x52
#define EEPROM_24LV08B_BLOCK3_ADDRESS		0x53

uint8_t EEPROM_24LV08B_WriteByte(uint8_t Block, uint8_t Address, uint8_t Data);
uint8_t EEPROM_24LV08B_ReadByte(uint8_t Block, uint8_t Address, uint8_t* Data);
uint8_t EEPROM_24LV08B_WaitReady(void);

#endif /* EEPROM_24LC08B_H_ */
