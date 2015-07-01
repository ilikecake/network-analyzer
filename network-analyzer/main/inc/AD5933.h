/*
 * AD5933.h
 *
 *  Created on: Jun 15, 2015
 *      Author: pat
 */

#ifndef AD5933_H_
#define AD5933_H_


#define AD5933_ADDRESS								0x0D

#define AD5933_REG_CONTROL_HIGH						0x80
#define AD5933_REG_CONTROL_LOW						0x81
#define AD5933_REG_START_FREQ_HIGH					0x82
#define AD5933_REG_START_FREQ_MID					0x83
#define AD5933_REG_START_FREQ_LOW					0x84
#define AD5933_REG_FREQ_INC_HIGH					0x85
#define AD5933_REG_FREQ_INC_MID						0x86
#define AD5933_REG_FREQ_INC_LOW						0x87
#define AD5933_REG_INC_NUMBER_HIGH					0x88
#define AD5933_REG_INC_NUMBER_LOW					0x89
#define AD5933_REG_SETTLING_TIME_CYCLES_HIGH		0x8A
#define AD5933_REG_SETTLING_TIME_CYCLES_LOW			0x8B
#define AD5933_REG_STATUS							0x8F
#define AD5933_REG_TEMP_HIGH						0x92
#define AD5933_REG_TEMP_LOW							0x93
#define AD5933_REG_REAL_HIGH						0x94
#define AD5933_REG_REAL_LOW							0x95
#define AD5933_REG_IMAG_HIGH						0x96
#define AD5933_REG_IMAG_LOW							0x97




//Control register values
//Upper byte
#define AD5933_CONTROL_INIT_WITH_START_FREQ		0x10
#define AD5933_CONTROL_START_FREQ_SWEEP			0x20
#define AD5933_CONTROL_INCREMENT_FREQ			0x30
#define AD5933_CONTROL_REPEAT_FREQ				0x40
#define AD5933_CONTROL_MEASURE_TEMP				0x90
#define AD5933_CONTROL_POWER_DOWN				0xA0
#define AD5933_CONTROL_STANDBY					0xB0

#define AD5933_CONTROL_OUTPUT_2V				0x00
#define AD5933_CONTROL_OUTPUT_200MV				0x02
#define AD5933_CONTROL_OUTPUT_400MV				0x04
#define AD5933_CONTROL_OUTPUT_1V				0x06

#define AD5933_CONTROL_PGA_5X					0x00
#define AD5933_CONTROL_PGA_1X					0x01

//Lower byte
#define AD5933_CONTROL_RESET					0x10

#define AD5933_CONTROL_CLK_EXT					0x08
#define AD5933_CONTROL_CLK_INT					0x00



//done
uint8_t AD5933_ReadReg(uint8_t RegToRead, uint8_t *RegVal);
uint8_t AD5933_WriteReg(uint8_t RegToWrite, uint8_t RegValToWrite);
uint8_t AD5933_SetMode(uint8_t Mode);
uint8_t AD5933_GetMode(uint8_t *Mode);
uint8_t AD5933_SetGain(uint8_t GainToSet);
uint8_t AD5933_SetOutputVoltage(uint8_t OutputVoltage);

uint8_t AD5933_SetClockSource(uint8_t ClockSource);

uint32_t AD5933_CalcFrequency(double FreqInHz, double MCLKInkHz);

uint8_t AD5933_GetStatus(uint8_t *StatusByte);

uint8_t AD5933_SetStartFreq(uint32_t StartFreq);
uint8_t AD5933_SetFreqIncrement(uint32_t FreqIncrement);
uint8_t AD5933_SetNumberOfIncrements(uint32_t Increments);
//write these


uint8_t AD5933_SetupSweep(uint16_t StartFrequency, uint16_t FrequencyIncrement, uint16_t NumberOfIncrements);


#endif /* AD5933_H_ */
