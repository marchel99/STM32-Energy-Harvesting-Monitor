#include "max.h"

#include "i2c.h"

#include <math.h>

void write_valrt_min(I2C_HandleTypeDef* hi2c, uint8_t valrt_min_value)
{
  uint8_t data[1];
  HAL_StatusTypeDef status;

  // Set the VALRT.MIN register value, assuming the address is 0x14
  data[0] = valrt_min_value;

  // Send the value to the VALRT.MIN register
  status = HAL_I2C_Mem_Write(hi2c, I2C_DEFAULT_ADDRESS << 1, 0x14, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

  // Check if the operation was successful
  if (status != HAL_OK)
  {
    // Error handling logic goes here
  }
}

void write_reset(I2C_HandleTypeDef* hi2c, uint8_t reset_value)
{
  uint8_t data[1];
  HAL_StatusTypeDef status;

  // Prepare the data to be written to the VRESET register
  data[0] = reset_value;

  // Transmit the reset value to the VRESET register
  status = HAL_I2C_Mem_Write(hi2c, I2C_DEFAULT_ADDRESS << 1, 0x18, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

  // Confirm if the write operation was successful
  if (status != HAL_OK)
  {
    // Error handling procedures
  }
}

HAL_StatusTypeDef read_reset(I2C_HandleTypeDef* hi2c, uint8_t* reset_value)
{
  HAL_StatusTypeDef status;
  uint8_t data[1] = {0};

  // Attempt to read the VRESET register value
  status = HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, 0x18, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

  // Verify that the read operation was successful
  if (status == HAL_OK)
  {
    // Assign the read value to the provided pointer
    *reset_value = data[0];
  }
  else
  {
    // Error handling as required
  }

  // Return the status of the operation
  return status;
}

uint8_t read_valrt_min(I2C_HandleTypeDef* hi2c)
{
  uint8_t valrt_min_value;
  uint8_t data[1];
  HAL_StatusTypeDef status;

  // Read the value from the VALRT.MIN register, assumed address is 0x14
  status = HAL_I2C_Mem_Read(hi2c, (I2C_DEFAULT_ADDRESS << 1) | 0x01, 0x14, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

  // Handle errors, return 0xFF as an error code
  if (status != HAL_OK)
  {
    return 0xFF;
  }

  // Assign the read value to the variable
  valrt_min_value = data[0];

  // Return the read value
  return valrt_min_value;
}

float read_voltage(I2C_HandleTypeDef* hi2c)
{
  uint8_t data[2];
  uint16_t raw_voltage;
  float voltage;

  // Perform a read operation for the voltage value
  HAL_StatusTypeDef status =
      HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_VCELL, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

  // Check for read success and compute the voltage
  if (status != HAL_OK)
  {
    return -1.0f; // Indicate error with a negative value
  }

  // Combine the two bytes into a raw voltage value
  raw_voltage = (data[0] << 8) | data[1];

  // Calculate the actual voltage using the LSB value
  voltage = raw_voltage * 78.125e-6; // 78.125 µV per LSB

  return voltage;
}

float read_current(I2C_HandleTypeDef* hi2c, float resistance)
{
  uint8_t data[2];
  uint16_t raw_voltage;
  float voltage, current;

  // Read the raw voltage related to the current measurement
  HAL_StatusTypeDef status =
      HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_VCELL, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

  // If read operation was successful, calculate current
  if (status != HAL_OK)
  {
    return -1.0f; // Indicate error with a negative value
  }

  // Calculate
