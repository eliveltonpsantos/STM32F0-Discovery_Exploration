/*
 * lib_24c32.h
 *
 *  Created on: 30 de jan de 2017
 *      Author: Elivelton
 */

#ifndef LIB_24C32_H_
#define LIB_24C32_H_

#include "stdlib.h"
#include "string.h"
#include "stm32f0xx_hal.h"
#include "main.h"

typedef enum
{
	EEPROM_CONFIG_WAIT			= 0x00U,	// binary: 0000 0000
	EEPROM_CONFIG_NOWAIT		= 0x01U		// binary: 0000 0001
} EEPROM_ConfigTypeDef;

HAL_StatusTypeDef eeprom_24c32_write(I2C_HandleTypeDef *hi2c, const uint16_t device_address,
		const uint16_t memory_address, const char *data_write, EEPROM_ConfigTypeDef config);
HAL_StatusTypeDef eeprom_24c32_read(I2C_HandleTypeDef *hi2c, const uint16_t device_address,
		const uint16_t memory_address, char *data_read, const uint16_t length);

#endif /* LIB_24C32_H_ */
