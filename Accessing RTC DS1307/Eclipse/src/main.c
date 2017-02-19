/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2017 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "i2c.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "diag/Trace.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
	/* USER CODE BEGIN 1 */
	char rmsg[64];

	uint8_t device_address = 0x68;
	uint8_t access_address[] = { 0 };

	volatile struct
	{
		uint8_t seconds, minutes, hours, dow, day, month, year;
	} rtc;

	char display[1];

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();

	/* USER CODE BEGIN 2 */
	rtc.seconds = 30;
	rtc.minutes = 59;
	rtc.hours = 19;
	rtc.dow = 2;
	rtc.day = 6;
	rtc.month = 2;
	rtc.year = 17;

	rmsg[0] = 0;
	rmsg[1] = (((rtc.seconds / 10) << 4) | (rtc.seconds - ((rtc.seconds / 10) * 10)));
	rmsg[2] = (((rtc.minutes / 10) << 4) | (rtc.minutes - ((rtc.minutes / 10) * 10)));
	rmsg[3] = (((rtc.hours / 10) << 4) | (rtc.hours - ((rtc.hours / 10) * 10)));
	rmsg[4] = rtc.dow;
	rmsg[5] = (((rtc.day / 10) << 4) | (rtc.day - ((rtc.day / 10) * 10)));
	rmsg[6] = (((rtc.month / 10) << 4) | (rtc.month - ((rtc.month / 10) * 10)));
	rmsg[7] = (((rtc.year / 10) << 4) | (rtc.year - ((rtc.year / 10) * 10)));

	if (HAL_I2C_IsDeviceReady(&hi2c1, (device_address << 1), 10, HAL_MAX_DELAY) != HAL_OK)
	{
		trace_printf("FALHA NO ACESSO A MEMORIA EEPROM");
		while (1) ;
	}

	HAL_I2C_Master_Transmit(&hi2c1, (device_address << 1), (uint8_t*)rmsg, 64, HAL_MAX_DELAY);
	if (HAL_I2C_IsDeviceReady(&hi2c1, (device_address << 1), 10, HAL_MAX_DELAY) != HAL_OK)
	{
		trace_printf("FALHA NO ACESSO A MEMORIA EEPROM");
		while (1) ;
	}

	// Set the memory vector at the position which wants to read.
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

		HAL_I2C_Master_Transmit(&hi2c1, (device_address << 1), access_address, 1, HAL_MAX_DELAY);

		HAL_I2C_Master_Receive(&hi2c1, (device_address << 1), (uint8_t*)rmsg, 64, HAL_MAX_DELAY);
		if (HAL_I2C_IsDeviceReady(&hi2c1, (device_address << 1), 10, HAL_MAX_DELAY) != HAL_OK)
		{
			trace_printf("FALHA NO ACESSO A MEMORIA EEPROM");
			while (1) ;
		}

		rtc.seconds = (((rmsg[0] & 0x70) >> 4) * 10) + (rmsg[0] & 0xF);
		rtc.minutes = (((rmsg[1] & 0x70) >> 4) * 10) + (rmsg[1] & 0xF);
		rtc.hours = (((rmsg[2] & 0x30) >> 4) * 10) + (rmsg[2] & 0xF);
		rtc.dow = rmsg[3];
		rtc.day = (((rmsg[4] & 0x30) >> 4) * 10) + (rmsg[4] & 0xF);
		rtc.month = (((rmsg[5] & 0x10) >> 4) * 10) + (rmsg[5] & 0xF);
		rtc.year = (((rmsg[6] & 0xF0) >> 4) * 10) + (rmsg[6] & 0xF);

		itoa(rtc.day, display, 10); trace_printf(display); trace_printf("/");
		itoa(rtc.month, display, 10); trace_printf(display); trace_printf("/");
		itoa(rtc.year, display, 10); trace_printf(display); trace_printf(" ");
		itoa(rtc.hours, display, 10); trace_printf(display); trace_printf(":");
		itoa(rtc.minutes, display, 10); trace_printf(display); trace_printf(":");
		itoa(rtc.seconds, display, 10); trace_printf(display); trace_printf(" ");
		itoa(rtc.dow, display, 10); trace_printf(display); trace_printf("\n");

	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while (1)
	{
	}
	/* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
