/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "lcd.h"
// #include "logo.c"

#include "font6x9.h"
#include "hagl.h"
#include "max.h"
#include "rgb565.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t test_image[64 * 64];

int __io_putchar(int ch)
{
  if (ch == '\n')
  {
    uint8_t ch2 = '\r';
    HAL_UART_Transmit(&huart2, &ch2, 1, HAL_MAX_DELAY);
  }

  HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
  return 1;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef * hrtc, RTC_TimeTypeDef * sTime, uint32_t Format);
  HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef * hrtc, RTC_DateTypeDef * sDate, uint32_t Format);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

   */

  lcd_init();

  /* // RECTANGLE
  for (int i = 0; i < 8; i++) {
    hagl_draw_rounded_rectangle(2+i, 2+i, 158-i, 126-i, 8-i, rgb565(0, 0,
  i*16));
  }

   */

  wchar_t buffer[32]; // Bufor na ciąg znaków, musi być wystarczająco duży

  // Konwersja int na ciąg znaków
  // swprintf(buffer, 32, L"Pomiar prądu: %.2f mA", your_variable);

  // hagl_put_text(L"Godzina: ", 15, 20, WHITE, font6x9);
  //-l_+p , -g|+d

  // Teraz możesz wywołać funkcję hagl_put_text z tym buforem
  hagl_put_text(buffer, 15, 40, WHITE, font6x9);

  wchar_t time_buffer[32]; //  Buffer for storing time information

  // write_valrt_min(&hi2c1, 00000000);  // 0x14 alert register

  write_reset(&hi2c1, 00000001); // 0x18 reset register

  while (1)
  {
    // Define time and date structure variables
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    // Retrieve current time from the RTC
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    // Retrieve current date from the RTC (date may not be used here but is
    // retrieved for completeness)
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    // Format and assign the current time to the time buffer
    swprintf(time_buffer, sizeof(time_buffer), L"Time: %02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);

    // Display the time on the screen at the specified coordinates and font
    hagl_put_text(time_buffer, 15, 20, WHITE, font6x9);

    // Read the battery voltage
    float battery_voltage = read_voltage(&hi2c1);
    wchar_t voltage_buffer[32]; // Buffer for voltage display

    // If the battery voltage is positive, format and display it
    if (battery_voltage > 0)
    {
      // Format and display the battery voltage
      swprintf(voltage_buffer, sizeof(voltage_buffer), L"Voltage: %.4f V", battery_voltage);
      // Display the voltage on the screen at the specified coordinates and font
      hagl_put_text(voltage_buffer, 15, 50, WHITE,
                    font6x9); // Adjust text position as needed
    }
    else
    {
      // Display an error message if voltage reading fails
      hagl_put_text(L"Voltage reading error!", 15, 50, WHITE, font6x9);
    }

    // Read the battery state of charge (SoC)
    float battery_soc = read_soc(&hi2c1);
    wchar_t soc_buffer[32]; // Buffer for SoC display

    // If the SoC reading is valid, format and display it
    if (battery_soc >= 0)
    {
      // Format and display the battery's state of charge
      swprintf(soc_buffer, sizeof(soc_buffer), L"Charge: %.2f%%", battery_soc);
      hagl_put_text(soc_buffer, 15, 40, WHITE,
                    font6x9); // Adjust text position as needed
    }
    else
    {
      // Display an error message if SoC reading fails
      hagl_put_text(L"SoC reading error!", 15, 40, WHITE, font6x9);
    }

    uint8_t ic_version = read_ic_version(&hi2c1);
    wchar_t version_buffer[32]; // Buffer for IC version information

    // Check if the IC version read is valid
    if (ic_version != 0xFF)
    {
      // Format and display the IC version
      swprintf(version_buffer, sizeof(version_buffer), L"IC Version: 0x%02X", ic_version);
      hagl_put_text(version_buffer, 15, 80, WHITE,
                    font6x9); // Adjust text position as necessary
    }
    else
    {
      // Display error message if IC version reading fails
      hagl_put_text(L"IC version read error!", 15, 80, WHITE, font6x9);
    }

    float resistance = 00.0f; // Shunt value: 0 Ohm, indicating no shunt present
    wchar_t current_buffer[64]; // Buffer for current measurement
    wchar_t shunt_buffer[32]; // Buffer for shunt description

    // Verify the presence of a shunt
    if (resistance > 0.0f)
    {
      // Format the shunt description
      if (resistance < 1000.0f)
      {
        swprintf(shunt_buffer, sizeof(shunt_buffer) / sizeof(wchar_t), L"Shunt: %.0f Ω", resistance);
      }
      else
      {
        swprintf(shunt_buffer, sizeof(shunt_buffer) / sizeof(wchar_t), L"Shunt: %.1f kΩ", resistance / 1000.0f);
      }
    }
    else
    {
      // Display "NONE" if no shunt is present
      swprintf(shunt_buffer, sizeof(shunt_buffer) / sizeof(wchar_t), L"Shunt: NONE");
    }

    float battery_current = read_current(&hi2c1, resistance);

    // Check the current value and format the result
    if (battery_current != 0 && resistance > 0.0f)
    {
      // Display in mA if the current is less than 1 A
      if (battery_current < 1.0)
      {
        swprintf(current_buffer, sizeof(current_buffer) / sizeof(wchar_t), L"Current: %.2f mA", battery_current * 1000);
      }
      // Otherwise, display in A
      else
      {
        swprintf(current_buffer, sizeof(current_buffer) / sizeof(wchar_t), L"Current: %.2f A", battery_current);
      }
    }
    else
    {
      // Display "NO SHUNT" if shunt value is 0 or current is 0
      swprintf(current_buffer, sizeof(current_buffer) / sizeof(wchar_t), L"Current: NO SHUNT");
    }

    // Display current and shunt information on the screen
    hagl_put_text(current_buffer, 15, 60, WHITE, font6x9);
    hagl_put_text(shunt_buffer, 15, 70, WHITE, font6x9);

    uint8_t vreset_value;
    HAL_StatusTypeDef status;

    // Attempt to read the VRESET register value
    status = read_reset(&hi2c1, &vreset_value);

    wchar_t vreset_buffer[32]; // Buffer for the VRESET register value

    // Check if the operation was successful
    if (status == HAL_OK)
    {
      // Format and display the VRESET register value
      swprintf(vreset_buffer, sizeof(vreset_buffer) / sizeof(vreset_buffer[0]), L"VRESET: %u", vreset_value);
      hagl_put_text(vreset_buffer, 15, 80, WHITE,
                    font6x9); // Adjust text position as necessary
    }
    else
    { // Display error message if VRESET register read fails
      hagl_put_text(L"VRESET register read error!", 15, 80, WHITE, font6x9);
    }
    uint8_t valrt_min = read_valrt_min(&hi2c1);
    wchar_t valrt_min_buffer[32]; // Buffer for the VALRT.MIN value
                                  // Verify that the read value is not an error code
    if (valrt_min != 0xFF)
    {
      // Format and display the VALRT.MIN value
      swprintf(valrt_min_buffer, sizeof(valrt_min_buffer) / sizeof(valrt_min_buffer[0]), L"VALRT.MIN: %u", valrt_min);
      hagl_put_text(valrt_min_buffer, 15, 90, WHITE,
                    font6x9); // Adjust text position as necessary
    }
    else
    {
      // Display error message if VALRT.MIN register read fails
      hagl_put_text(L"VALRT.MIN register read error!", 15, 90, WHITE, font6x9);
    }

    printf("Czas: %02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);

    printf(" | Voltage: %.8f\n", battery_voltage);

    printf(" | Voltage: %.8f\n", battery_voltage);

    lcd_copy();
    HAL_Delay(1000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */