/*
 * lib_24c32.c
 *
 *  Created on: 30 de jan de 2017
 *      Author: Elivelton
 */


#include "lib_24c32.h"

/*
 * @brief	Write some data at a specific address in EEPROM 24C32.
 * @note	There is a field to check if the user wants to wait for the EEPROM memory finishes write data.
 *
 * @param	*hi2c: which hardware device will be used.
 * @param	device_address: 7 bits address of the device.
 * @param	memory_address: first 16 bits address to start to write.
 * @param	*data_write: data to write.
 * @param	config: check if want wait writes process.
 *
 * @retval	I2C default status.
 */
HAL_StatusTypeDef eeprom_24c32_write(I2C_HandleTypeDef *hi2c,
		const uint16_t device_address,
		const uint16_t memory_address,
		const char *data_write,
		EEPROM_ConfigTypeDef config)
{
	HAL_StatusTypeDef return_value = 0;

	// Allocate on memory all bytes required (2 bytes of address + n bytes of the message) to send.
	uint8_t *data = (uint8_t*)malloc(sizeof(uint8_t)*(strlen(data_write)+3));

	// Split the address in 2 bytes.
	data[0] = (uint8_t)(memory_address >> 8);
	data[1] = (uint8_t)memory_address;

	// Merge address with the message.
	memcpy(data+2, (uint8_t*)data_write, strlen(data_write)+1);

	// Transmit the message.
	return_value = HAL_I2C_Master_Transmit(hi2c, (device_address << 1), data, (strlen(data_write)+3),
			HAL_MAX_DELAY);

	// Check if is needed to wait for the write routine is completed.
	if (config == EEPROM_CONFIG_WAIT)
		while(HAL_I2C_Master_Transmit(hi2c, (device_address << 1), 0, 0, HAL_MAX_DELAY) != HAL_OK);

	// Unallocated memory
	free(data);

	return return_value;
}

/*
 * @brief	Read a specify bytes start from an address at EEPROM 24C32.
 * @note	Return bytes reads at pointer *data_read.
 *
 * @param	*hi2c: which hardware device will be used.
 * @param	device_address: 7 bits address of the device.
 * @param	memory_address: first 16 bits address to start to read.
 * @param	*data_read: pointer to store the data read.
 * @param	length: quantity of bytes to read.
 *
 * @retval	I2C default status.
 */
HAL_StatusTypeDef eeprom_24c32_read(I2C_HandleTypeDef *hi2c,
		const uint16_t device_address,
		const uint16_t memory_address,
		char *data_read,
		const uint16_t length)
{
	uint8_t return_value;
	uint8_t access_address[2];

	// Split the address in 2 bytes.
	access_address[0] = (uint8_t)(memory_address >> 8);
	access_address[1] = (uint8_t)memory_address;

	// Set the memory vector at the position which wants to read.
	return_value = HAL_I2C_Master_Transmit(hi2c, (device_address << 1), access_address, 2, HAL_MAX_DELAY);

	// Check if communication is made.
	if (return_value != HAL_OK)
		return return_value;

	// Request the data to memory.
	return_value = HAL_I2C_Master_Receive(hi2c, (device_address << 1), (uint8_t *)data_read, length,
			HAL_MAX_DELAY);

	return return_value;
}
