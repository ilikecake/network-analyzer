/*
 * 24LC08B.c
 *
 *  Created on: Jun 23, 2014
 *      Author: pat
 */

#include "main.h"


I2C_XFER_T EEPROM_24LV08B_I2C;

uint8_t EEPROM_24LV08B_WriteByte(uint8_t Block, uint8_t Address, uint8_t Data)
{
	uint8_t SendData[2];

	SendData[0] = Address;
	SendData[1] = Data;
	EEPROM_24LV08B_I2C.slaveAddr = (EEPROM_24LV08B_BLOCK0_ADDRESS + Block);
	EEPROM_24LV08B_I2C.txSz = 2;
	EEPROM_24LV08B_I2C.rxSz = 0;
	EEPROM_24LV08B_I2C.txBuff = SendData;
	EEPROM_24LV08B_I2C.rxBuff = NULL;
	return (Chip_I2C_MasterTransfer(DEFAULT_I2C, &EEPROM_24LV08B_I2C));
}

uint8_t EEPROM_24LV08B_ReadByte(uint8_t Block, uint8_t Address, uint8_t *Data)
{
	uint8_t SendData;
	SendData = Address;

	EEPROM_24LV08B_I2C.slaveAddr = (EEPROM_24LV08B_BLOCK0_ADDRESS + Block);
	EEPROM_24LV08B_I2C.txBuff = &SendData;
	EEPROM_24LV08B_I2C.txSz = 1;
	EEPROM_24LV08B_I2C.rxBuff = Data;
	EEPROM_24LV08B_I2C.rxSz = 1;
	return (Chip_I2C_MasterTransfer(DEFAULT_I2C, &EEPROM_24LV08B_I2C));
}

uint8_t EEPROM_24LV08B_WaitReady(void)
{
	uint8_t ch[2];
	uint16_t Timeout = 0;

	while ((Chip_I2C_MasterRead(DEFAULT_I2C, EEPROM_24LV08B_BLOCK0_ADDRESS, ch, 1) == 0) && (Timeout < 50000))
	{
		Timeout++;
	}
	return 0;
}
