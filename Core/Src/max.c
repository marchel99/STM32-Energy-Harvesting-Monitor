#include "max.h"
#include "i2c.h"


float read_voltage(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[2];
    uint16_t raw_voltage;
    float voltage;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_VCELL, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return -1.0f; // Indicate error
    }

    raw_voltage = (data[0] << 8) | data[1];

    voltage = raw_voltage * 78.125e-6; // 78.125 µV per LSB

    return voltage;
}

#include <math.h> // Dołącz bibliotekę matematyczną dla nanf

float read_current(I2C_HandleTypeDef *hi2c, float resistance)
{
    uint8_t data[2];
    uint16_t raw_voltage;
    float voltage, current;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_VCELL, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        return -1.0f; // Indicate error
    }

    raw_voltage = (data[0] << 8) | data[1];
    voltage = raw_voltage * 78.125e-6; // 78.125 µV per LSB

    if (resistance > 0)
    {
        current = voltage / resistance;
        return current;
    }
    else
    {
        return nanf(""); // Return NaN to indicate error in resistance value
    }
}

float read_soc(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[2];
    float soc;

    // Odczyt 2 bajtów z rejestru SOC
    if (HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_SOC, I2C_MEMADD_SIZE_8BIT, data, sizeof(data), HAL_MAX_DELAY) == HAL_OK)
    {
        // Wartość SoC jest przechowywana jako 16-bitowa liczba, gdzie każdy bit odpowiada 1/256%
        soc = ((data[0] << 8) | data[1]) / 256.0f;
    }
    else
    {
        // W przypadku błędu zwróć -1.0f
        soc = -1.0f;
    }

    return soc;
}

uint8_t read_ic_version(I2C_HandleTypeDef *hi2c)
{
    uint8_t version;
    uint8_t reg = REGISTER_VERSION; // Adres rejestru VERSION to 0x08

    // Odczyt 1 bajtu z rejestru VERSION
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, I2C_DEFAULT_ADDRESS << 1, reg, I2C_MEMADD_SIZE_8BIT, &version, sizeof(version), HAL_MAX_DELAY);

    if (status != HAL_OK)
    {
        // W przypadku błędu zwróć 0xFF
        return 0xFF;
    }

    return version;
}
