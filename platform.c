/**
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#include "platform.h"
#include <stdio.h>

uint8_t RdByte(
		VL53L8CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_value)
{
	uint8_t status = 255;
	unsigned char buffer[2];
	buffer[0] = RegisterAdress >> 8;
	buffer[1] = RegisterAdress & 0xFF;

	if (write(p_platform->fd, buffer, 2) != 2)
	{
		perror("Failed to write to the i2c bus.");
		return status;
	}

	if (read(p_platform->fd, p_value, 1) != 1)
	{
		perror("Failed to read from the i2c bus.");
		return status;
	}

	//readVal = wiringPiI2CReadReg8(p_platform->fd, (int)RegisterAdress);
	//if (readVal == (int)(*p_value))
	//	status = 0;

	return 0;
}

uint8_t WrByte(
		VL53L8CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t value)
{
	uint8_t status = 255;

	unsigned char buffer[3];
	buffer[0] = RegisterAdress >> 8;
	buffer[1] = RegisterAdress & 0xFF;
	buffer[2] = value;

	if (write(p_platform->fd, buffer, 3) != 3)
	{
		perror("Failed to write to the I2C bus");
		return status;
	}


//	if (wiringPiI2CWriteReg8(p_platform->fd, (int)RegisterAdress, value) != -1)
//		status = 0;

	return 0;
}

uint8_t WrMulti(
		VL53L8CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
	uint8_t status = 255;

	if (size > MAX_I2C_BUFFER_SIZE)
	{
		uint32_t remaining_bytes = size;
		uint32_t offset = 0;
		uint8_t buffer[MAX_I2C_BUFFER_SIZE];
		int chunk_size;

		while (remaining_bytes > 0)
		{
			chunk_size = remaining_bytes > MAX_I2C_BUFFER_SIZE - 2 ? MAX_I2C_BUFFER_SIZE - 2 : remaining_bytes;

			buffer[0] = (RegisterAdress >> 8) & 0xFF;
			buffer[1] = RegisterAdress & 0xFF;

			for (int i = 0; i < chunk_size; ++i)
				buffer[2 + i] = p_values[offset + i];
		
			if (write(p_platform->fd, buffer, chunk_size + 2) != chunk_size + 2)
			{
				perror("Failed to write to the i2c bus");
				return status;
			}

			remaining_bytes -= chunk_size;
			offset += chunk_size;
			RegisterAdress += chunk_size;
		}

		
	/*	unsigned char buffer[2];
		buffer[0] = RegisterAdress >> 8;
		buffer[1] = RegisterAdress & 0xFF;

		if (write(p_platform->fd, buffer, 2) != 2)
		{
			perror("Failed to write register address to the i2c bus.");
			return status;
		}
		
		uint32_t remaining_bytes = size;
		uint32_t offset = 0;
		int chunk_size;

		printf("size parameter: %d\n", size);

		while (remaining_bytes > 0)
		{
			chunk_size = remaining_bytes > 8192 ? 8192 : remaining_bytes;
			printf("chunk_size: %d, remaining_bytes: %d\n", chunk_size, remaining_bytes);

			if (write(p_platform->fd, p_values + offset, chunk_size) != chunk_size)
			{
				perror("Failed to write to the i2c bus.");
				return status;
			}
			remaining_bytes -= chunk_size;
			offset += chunk_size;
		}*/
	}
	else
	{
	// BELOW IS THE OK METHOD, BUT THE MAX I2C WRITE SIZE IS LIMITED TO 8192
		unsigned char buffer[size + 2];
		buffer[0] = RegisterAdress >> 8;
		buffer[1] = RegisterAdress & 0xFF;
		memcpy(buffer + 2, p_values, size);

		int wrsize = write(p_platform->fd, buffer, size + 2); 
		if (wrsize != size + 2)
		{
			perror("Failed to write to the i2c bus.");
			return status;
		}
	}
	
	return 0;
}

uint8_t RdMulti(
		VL53L8CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
	uint8_t status = 255;

	unsigned char buffer[2];
	buffer[0] = RegisterAdress >> 8;
	buffer[1] = RegisterAdress & 0xFF;

	if (write(p_platform->fd, buffer, 2) != 2)
	{
		perror("Failed to write to the i2c bus.");
		return status;
	}

	if (read(p_platform->fd, p_values, size) != size)
	{
		perror("Failed to read from the i2c bus.");
		return status;
	}

	/* Need to be implemented by customer. This function returns 0 if OK */
	
	return 0;
}

uint8_t Reset_Sensor(
		VL53L8CX_Platform *p_platform)
{
	uint8_t status = 0;
	
	/* (Optional) Need to be implemented by customer. This function returns 0 if OK */
	
	/* Set pin LPN to LOW */
	/* Set pin AVDD to LOW */
	/* Set pin VDDIO  to LOW */
	/* Set pin CORE_1V8 to LOW */
	WaitMs(p_platform, 100);

	/* Set pin LPN to HIGH */
	/* Set pin AVDD to HIGH */
	/* Set pin VDDIO to HIGH */
	/* Set pin CORE_1V8 to HIGH */
	WaitMs(p_platform, 100);

	return status;
}

void SwapBuffer(
		uint8_t 		*buffer,
		uint16_t 	 	 size)
{
	uint32_t i, tmp;
	
	/* Example of possible implementation using <string.h> */
	for(i = 0; i < size; i = i + 4) 
	{
		tmp = (
		  buffer[i]<<24)
		|(buffer[i+1]<<16)
		|(buffer[i+2]<<8)
		|(buffer[i+3]);
		
		memcpy(&(buffer[i]), &tmp, 4);
	}
}	

uint8_t WaitMs(
		VL53L8CX_Platform *p_platform,
		uint32_t TimeMs)
{
//	uint8_t status = 255;

	usleep(TimeMs * 1000);

	return 0;
}
