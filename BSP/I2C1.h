#ifndef _I2C1_H_
#define _I2C1_H_

#include "stm32f0xx.h"

void I2C1_Init(void);
uint16_t IIC_WriteData(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t *buf, uint16_t len);
uint16_t IIC_ReadData(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t *buf, uint16_t len);

#endif
