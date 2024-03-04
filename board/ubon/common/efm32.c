// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 * Copyright 2020-2022 NXP
 * Copyright 2021 Microsoft Corporation
 */

#include <common.h>
#include <i2c.h>
#include "efm32.h"

#define I2C_BUS 	2
#define EFM32_ADDR 	0x0C

void set_efm32_watchdog(unsigned long sec)
{
    u8 ubSentBuf[8] = { 0 } ;
    u8 ubRecvBuf[8] = { 0 } ;
    //unsigned int iBus = i2c_get_bus_num() ;
    //int i ;
    
    ubSentBuf[0] = 5;
    ubSentBuf[1] = 0x8E;
    ubSentBuf[2] = (u8)(sec >> 8 & 0xFF);
    ubSentBuf[3] = (u8)(sec >> 0 & 0xFF);
    ubSentBuf[4] = 0;

	ubSentBuf[4] = ubSentBuf[0] + ubSentBuf[1] + ubSentBuf[2] + ubSentBuf[3];

#ifdef CONFIG_DM_I2C
	struct udevice *dev;
	if(i2c_get_chip_for_busnum(I2C_BUS ,EFM32_ADDR,0 , &dev))
	{
		printf("EFM32: device not found\n");
		return;
	}
	dm_i2c_write(dev, 0, (u8 *)ubSentBuf, 5);
	dm_i2c_read( dev, 0, (u8 *)ubRecvBuf, 3);
	printf("EFM32: [%02X][%02X][%02X] \n",ubRecvBuf[0],ubRecvBuf[1],ubRecvBuf[2]);
#else
	u8 iBus = i2c_get_bus_num();
	i2c_set_bus_num(I2C_BUS);
	i2c_write(EFM32_ADDR, 0x00 , 0 , (u8 *)ubSentBuf , 5);
	i2c_read( EFM32_ADDR, 0x00 , 0 , ubRecvBuf , 3);
	printf("EFM32: [%02X][%02X][%02X] \n",ubRecvBuf[0],ubRecvBuf[1],ubRecvBuf[2]);
	/* reset the i2c bus */
	i2c_set_bus_num(iBus);
#endif
}

