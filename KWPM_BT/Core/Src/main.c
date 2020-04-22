/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//AKCELEROMETR
#define ACC_USTAWIENIA 0x57
#define ACC_ADRES (0x19 << 1)
#define ACC_CTRL_REG1_A 0x20
#define ACC_CTRL_REG4_A 0x23
#define ACC_ZCZYTANIE_POCZATEK 0x28
#define ACC_MULTI_READ 0x80
#define ACC_WSZYSTKIE_OSIE_ZCZYTANIE (ACC_MULTI_READ | ACC_ZCZYTANIE_POCZATEK)
#define ACC_SET_4G 0x10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Komunikacja
int8_t Wiadomosc[200];
int16_t Rozmiar;

uint8_t Dane[6]; // Tablica przechowujaca wszystkie bajty zczytane z akcelerometru
int16_t X = 0; // Zczytane dane z osi X w formie zlaczonych w całość bajtów starszych i młodszych
int16_t Y = 0; // Zczytane dane z osi Y w formie zlaczonych w całość bajtów starszych i młodszych
int16_t Z = 0; // Zczytane dane z osi Z w formie zlaczonych w całość bajtów starszych i młodszych

float X_g = 0; // Zawiera przyspieszenie w osi X w jednostce g - przyspieszenia ziemskiego
float Y_g = 0; // Zawiera przyspieszenie w osi Y w jednostce g - przyspieszenia ziemskiego
float Z_g = 0; // Zawiera przyspieszenie w osi Z w jednostce g - przyspieszenia ziemskiego

float X_mem = 0; // Poprzednia wartość
float Y_mem = 0; // Poprzednia wartość
float Z_mem = 0; // Poprzednia wartość

float X_roznica = 0; // Przechowywuje roznice
float Y_roznica = 0; // Przechowywuje roznice
float Z_roznica = 0; // Przechowywuje roznice

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

volatile int flaga = 0;
//printf przeciazenie

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef* htim)
{
	if(htim == &htim11){
		flaga = 1;
	}
}

void Setup_UART_BT(UART_HandleTypeDef * UART){

	HAL_Delay(1000);
	HAL_GPIO_WritePin(KEY_GPIO_Port, KEY_Pin, GPIO_PIN_SET);
	HAL_Delay(100); //Przywrocenie ustawien fabrycznych
	/*HAL_UART_Transmit(UART, (uint8_t*) "AT+ORGL\r\n", strlen("AT+ORGL\r\n"), 100);
	HAL_Delay(100);
	HAL_UART_Transmit(UART, (uint8_t*) "AT+RMAAD\r\n", strlen("AT+RMAAD\r\n"), 100);
	HAL_Delay(100); //Wyzerowanie sparowanych urzadzen
	HAL_UART_Transmit(UART, (uint8_t*) "AT+NAME=BT_STM\r\n", strlen("AT+NAME=BT_STM\r\n"), 100);
	HAL_Delay(100); //Zmiana nazwy na  BT_STM
	HAL_UART_Transmit(UART, (uint8_t*) "AT+ROLE=0\r\n", strlen("AT+ROLE=0\r\n"), 100);
	HAL_Delay(100); //Ustawienie roli urzadzenia w tryb slave*/


	HAL_UART_Transmit(UART, (uint8_t*) "AT+UART=115200,0,0\r\n", strlen("AT+UART=115200,0,0\r\n"), 100);
	HAL_Delay(100);


	HAL_GPIO_WritePin(KEY_GPIO_Port, KEY_Pin, GPIO_PIN_RESET);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM11_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  Setup_UART_BT(&huart2);
  uint8_t USTAWIENIA = ACC_USTAWIENIA;
  uint8_t RESOLUTION = ACC_SET_4G;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //AKCELEROMETR - aktywacja, 100Hz, oś XYZ
   HAL_I2C_Mem_Write(&hi2c1, ACC_ADRES, ACC_CTRL_REG1_A, 1, &USTAWIENIA, 1, 100);
   //AKCELEROMETR - zmiana zakresu pomiarowego z +-2g na +-4g
   HAL_I2C_Mem_Write(&hi2c1, ACC_ADRES, ACC_CTRL_REG4_A, 1, &RESOLUTION, 1, 100);

   //TIM11 - 66Hz
   HAL_TIM_Base_Start_IT(&htim11);

  while (1)
  {
	   if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {

		   if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {

				   if (flaga == 1){
					   HAL_I2C_Mem_Read(&hi2c1, ACC_ADRES,


					   ACC_WSZYSTKIE_OSIE_ZCZYTANIE, 1, Dane, 6, 100);
					   X = ((Dane[1] << 8) | Dane[0]);
					   Y = ((Dane[3] << 8) | Dane[2]);
					   Z = ((Dane[5] << 8) | Dane[4]);

					   X_g = ((float) X * 4.0) / (float) INT16_MAX;
					   Y_g = ((float) Y * 4.0) / (float) INT16_MAX;
					   Z_g = ((float) Z * 4.0) / (float) INT16_MAX;

					   X_roznica = fabs(X_mem - X_g);
					   Y_roznica = fabs(Y_mem - Y_g);
					   Z_roznica = fabs(Z_mem - Z_g);

					   if(X_roznica > 0.1)HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
					   if(Y_roznica > 0.1)HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
					   if(Z_roznica > 0.1)HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);

					   Rozmiar = sprintf((char *)Wiadomosc, "X:%f Y:%f Z:%f\r\n", X_g,Y_g,Z_g);

					   HAL_UART_Transmit(&huart2, (uint8_t*) Wiadomosc,  Rozmiar, 100);

					   X_mem = X_g;
					   Y_mem = Y_g;
					   Z_mem = Z_g;


					   flaga = 0;
				   }
		   	   }

		   }
	   HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
