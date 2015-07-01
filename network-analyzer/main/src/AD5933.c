/*
 * AD5933.c
 *
 *  Created on: Jun 15, 2015
 *      Author: pat
 */

#include "main.h"


uint8_t AD5933_ReadReg(uint8_t RegToRead, uint8_t *RegVal)
{
	uint8_t SendData;
	I2C_XFER_T AD5933_Xfer;
	SendData = RegToRead;

	AD5933_Xfer.slaveAddr = AD5933_ADDRESS;
	AD5933_Xfer.txBuff = &SendData;
	AD5933_Xfer.txSz = 1;
	AD5933_Xfer.rxBuff = RegVal;
	AD5933_Xfer.rxSz = 1;
	return (Chip_I2C_MasterTransfer(DEFAULT_I2C, &AD5933_Xfer));
}


uint8_t AD5933_WriteReg(uint8_t RegToWrite, uint8_t RegValToWrite)
{
	uint8_t SendData[2];
	I2C_XFER_T AD5933_Xfer;

	SendData[0] = RegToWrite;
	SendData[1] = RegValToWrite;
	AD5933_Xfer.slaveAddr = AD5933_ADDRESS;
	AD5933_Xfer.txSz = 2;
	AD5933_Xfer.rxSz = 0;
	AD5933_Xfer.txBuff = SendData;
	AD5933_Xfer.rxBuff = NULL;
	return (Chip_I2C_MasterTransfer(DEFAULT_I2C, &AD5933_Xfer));
}

uint8_t AD5933_SetMode(uint8_t Mode)
{
	uint8_t TempBit;
	uint8_t ret;

	//TODO: Check if mode is valid
	ret = AD5933_ReadReg(AD5933_REG_CONTROL_HIGH, &TempBit);
	if(ret != I2C_STATUS_DONE) return ret;

	TempBit = TempBit & (0x0F);
	return(AD5933_WriteReg(AD5933_REG_CONTROL_HIGH, (TempBit | Mode)));
}

uint8_t AD5933_GetMode(uint8_t *Mode)
{
	uint8_t ret;
	ret = AD5933_ReadReg(AD5933_REG_CONTROL_HIGH, Mode);
	*Mode &= 0xF0;
	return ret;
}

uint8_t AD5933_SetGain(uint8_t GainToSet)
{
	uint8_t TempBit;
	uint8_t ret;

	if((GainToSet == AD5933_CONTROL_PGA_5X) || (GainToSet == AD5933_CONTROL_PGA_1X))
	{
		ret = AD5933_ReadReg(AD5933_REG_CONTROL_HIGH, &TempBit);
		if(ret != I2C_STATUS_DONE) return ret;
		TempBit = TempBit & (0xFE);
		return(AD5933_WriteReg(AD5933_REG_CONTROL_HIGH, (TempBit | GainToSet)));
	}
	else
	{
		return 0xFF;
	}
}

uint8_t AD5933_SetOutputVoltage(uint8_t OutputVoltage)
{
	uint8_t TempBit;
	uint8_t ret;

	if((OutputVoltage == AD5933_CONTROL_OUTPUT_2V) || (OutputVoltage == AD5933_CONTROL_OUTPUT_200MV) || (OutputVoltage == AD5933_CONTROL_OUTPUT_400MV) || (OutputVoltage == AD5933_CONTROL_OUTPUT_1V))
	{
		ret = AD5933_ReadReg(AD5933_REG_CONTROL_HIGH, &TempBit);
		if(ret != I2C_STATUS_DONE) return ret;		//TODO: make this into a compiler define?
		TempBit = TempBit & (0xF9);
		return(AD5933_WriteReg(AD5933_REG_CONTROL_HIGH, (TempBit | OutputVoltage)));
	}
	else
	{
		return 0xFF;
	}

}

uint8_t AD5933_SetClockSource(uint8_t ClockSource)
{
	uint8_t TempBit;
	uint8_t ret;

	if((ClockSource == AD5933_CONTROL_CLK_EXT) || (ClockSource == AD5933_CONTROL_CLK_INT))
	{
		ret = AD5933_ReadReg(AD5933_REG_CONTROL_LOW, &TempBit);
		if(ret != I2C_STATUS_DONE) return ret;
		TempBit = TempBit & (0xF7);
		return(AD5933_WriteReg(AD5933_REG_CONTROL_LOW, (TempBit | ClockSource)));
	}
	else
	{
		return 0xFF;
	}
}

uint32_t AD5933_CalcFrequency(double FreqInHz, double MCLKInkHz)
{
	double FloatOutput;
	FloatOutput = (FreqInHz/(MCLKInkHz*1000/4.0))*0x8000000;
	return (uint32_t)FloatOutput;
}

uint8_t AD5933_GetStatus(uint8_t *StatusByte)
{
	return(AD5933_ReadReg(AD5933_REG_STATUS, StatusByte));
}

uint8_t AD5933_SetStartFreq(uint32_t StartFreq)
{
	uint8_t ret;

	//TODO: Implement the block write functions to make this easier to do
	ret = AD5933_WriteReg(AD5933_REG_START_FREQ_HIGH, ((StartFreq&0x00FF0000) >> 16) );
	if(ret != I2C_STATUS_DONE) return ret;

	ret = AD5933_WriteReg(AD5933_REG_START_FREQ_MID, ((StartFreq&0x0000FF00) >> 8) );
	if(ret != I2C_STATUS_DONE) return ret;

	ret = AD5933_WriteReg(AD5933_REG_START_FREQ_LOW, (StartFreq&0x000000FF) );

	return ret;
}

uint8_t AD5933_SetFreqIncrement(uint32_t FreqIncrement)
{
	uint8_t ret;

	//TODO: Implement the block write functions to make this easier to do
	ret = AD5933_WriteReg(AD5933_REG_FREQ_INC_HIGH, ((FreqIncrement&0x00FF0000) >> 16) );
	if(ret != I2C_STATUS_DONE) return ret;

	ret = AD5933_WriteReg(AD5933_REG_FREQ_INC_MID, ((FreqIncrement&0x0000FF00) >> 8) );
	if(ret != I2C_STATUS_DONE) return ret;

	ret = AD5933_WriteReg(AD5933_REG_FREQ_INC_LOW, (FreqIncrement&0x000000FF) );

	return ret;
}

uint8_t AD5933_SetNumberOfIncrements(uint32_t Increments)
{
	uint8_t ret;

	ret = AD5933_WriteReg(AD5933_REG_INC_NUMBER_HIGH, ((Increments&0x00000100) >> 8) );
	if(ret != I2C_STATUS_DONE) return ret;

	ret = AD5933_WriteReg(AD5933_REG_INC_NUMBER_LOW, (Increments&0x000000FF) );

	return ret;
}
