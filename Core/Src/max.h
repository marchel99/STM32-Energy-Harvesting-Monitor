#pragma once

#include "stm32l4xx_hal.h"

#include <stdint.h>

#define I2C_DEFAULT_ADDRESS 0x36
// Define the default I2C slave address.

#define REGISTER_VCELL 0x02
// The VCELL register stores the measured cell voltage. Access this register to obtain the battery's current voltage.

#define REGISTER_SOC 0x04
// State of Charge (SOC): The SOC register contains the battery's charge percentage as determined by the chip's algorithm, reflecting the
// charge level relative to its full capacity.

#define REGISTER_MODE 0x06
// Hibernation State: The MODE register indicates whether the chip is in hibernation mode, with a specific bit designated to represent this
// state.

#define REGISTER_VERSION 0x08
// Register Version: This register stores the version of the IC, providing information about the chip's model and revision.

#define VALRT_MIN 0x14
// Minimum Voltage Alert Register: This register holds the threshold value below which a voltage alert will be triggered.

#define VRESET 0x18
// Reset Register: The VRESET register controls the conditions under which the chip will reset itself.

// Function declarations with detailed descriptions:

// Reads the battery voltage and returns it as a floating-point value.
float read_voltage(I2C_HandleTypeDef* hi2c);

// Reads the current flowing through a resistor of known resistance and returns the calculated current as a floating-point value.
float read_current(I2C_HandleTypeDef* hi2c, float resistance);

// Reads the State of Charge (SOC) percentage from the battery and returns it as a floating-point value.
float read_soc(I2C_HandleTypeDef* hi2c);

// Reads the IC version from the chip and returns it as an unsigned 8-bit integer.
uint8_t read_ic_version(I2C_HandleTypeDef* hi2c);

// Writes the minimum voltage alert value to the VALRT.MIN register.
void write_valrt_min(I2C_HandleTypeDef* hi2c, uint8_t valrt_min_value);

// Writes a value to the reset register to initiate a reset condition.
void write_reset(I2C_HandleTypeDef* hi2c, uint8_t reset_value);

// Reads the minimum voltage alert value from the VALRT.MIN register and returns it as an unsigned 8-bit integer.
uint8_t read_valrt_min(I2C_HandleTypeDef* hi2c);

// Reads the current value of the reset register and stores it in the provided variable.
HAL_StatusTypeDef read_reset(I2C_HandleTypeDef* hi2c, uint8_t* reset_value);
