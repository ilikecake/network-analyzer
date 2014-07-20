/*
 * i2c.c
 *
 *  Created on: Nov 13, 2013
 *      Author: pat
 */


/*
 * @brief I2C example.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "main.h"

static int mode_poll;	/* Poll/Interrupt mode flag */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void Init_I2C_PinMux(void)
{
	Chip_SYSCTL_PeriphReset(RESET_I2C0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 4, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 5, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
}

/* State machine handler for I2C0 and I2C1 */
static void i2c_state_handling(I2C_ID_T id)
{
	if (Chip_I2C_IsMasterActive(id)) {
		Chip_I2C_MasterStateHandler(id);
	}
	else {
		Chip_I2C_SlaveStateHandler(id);
	}
}

/* Set I2C mode to polling/interrupt */
/* polling = 1 for polling mode, 0 for interrupt mode */
static void i2c_set_mode(I2C_ID_T id, int polling)
{
	if (!polling) {
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(I2C0_IRQn);
	}
	else {
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(I2C0_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}

/* Initialize the I2C bus */
void i2c_app_init(I2C_ID_T id, int speed)
{
	Init_I2C_PinMux();

	/* Initialize I2C */
	Chip_I2C_Init(id);
	Chip_I2C_SetClockRate(id, speed);

	/* Set default mode to interrupt */
	i2c_set_mode(id, I2C_MODE_POLLING);
}

/* Function that probes all available slaves connected to an I2C bus */
void i2c_probe_slaves(I2C_ID_T i2c)
{
	int i;
	uint8_t ch[2];

	DEBUGOUT("Probing available I2C devices...\r\n");
	DEBUGOUT("\r\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
	DEBUGOUT("\r\n====================================================");
	for (i = 0; i <= 0x7F; i++) {
		if (!(i & 0x0F)) {
			DEBUGOUT("\r\n%02X  ", i >> 4);
		}
		if ((i <= 7) || (i > 0x78)) {
			DEBUGOUT("   ");
			continue;
		}
		if (Chip_I2C_MasterRead(i2c, i, ch, 1) > 0) {
			DEBUGOUT(" %02X", i);
		}
		else {
			DEBUGOUT(" --");
		}
	}
	DEBUGOUT("\r\n");
}

/**
 * @brief	I2C Interrupt Handler
 * @return	None
 */
void I2C_IRQHandler(void)
{
	i2c_state_handling(I2C0);
}
