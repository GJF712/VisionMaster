#ifndef _I2C1_H_
#define _I2C1_H_

#include "stm32f0xx.h"
#include <stdbool.h>

#define BatteryVoltage 16800

void I2C1_Init(void);
void I2C_CheckBusy(void);
uint16_t IIC_WriteData(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t *buf, uint16_t len);
uint16_t IIC_ReadData(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t *buf, uint16_t len);
void TUSB422_BoostTo20V(bool *Event_flag);
void BQ25703A_Init(void);
void TUSB422_BoostTo20VInit(void);

#endif
