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
#include "matrix.h"
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

//ZYROSKOP
#define ZYR_REG1_A 0x20
#define ZYR_USTAWIENIA 0x0F
#define ZYR_MULTIREAD 0x40
#define ZYR_READ 0x80
#define ZYR_ZCZYTANIE_POCZATEK 0x28
#define ZYR_WSZYSTKIE_OSIE_ZCZYTANIE (ZYR_READ | ZYR_MULTIREAD | ZYR_ZCZYTANIE_POCZATEK)



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Komunikacja
int8_t Wiadomosc[200];
uint8_t Odbior;
int16_t Rozmiar;

uint8_t Dane[6]; // Tablica przechowujaca wszystkie bajty zczytane z akcelerometru lub zyroskopu
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

float X_kat = 0; // Zawiera wartosc przyspieszenia katowego w osi X w jednostce dps - degrees per second
float Y_kat = 0; // Zawiera wartosc przyspieszenia katowego w osi Y w jednostce dps - degrees per second
float Z_kat = 0; // Zawiera wartosc przyspieszenia katowego w osi Z w jednostce dps - degrees per second

float roll = 0; //kat roll dla filtru komplementarnego
float pitch = 0; //kat pitch dla filtru komplementarnego

int Button_B_mem = 0; //zmienne przechowujace stan przycisku B1
int Button_B = 0;

int Polaczenie = 0; //nawiazana lacznosc z komputerem




//KALMAN x
float x_post[2];
float A[4], B[2], C[2];
float std_dev_v, std_dev_w;
float V[4], W[1];
float P_pri[4], P_post[4];
float x_pri[2];
float eps[1], S[1], K[2];
float u[1], y[1];
float acc_x, acc_y, acc_z;

float Ax[2], Bu[2];
float AP[4], AT[4], APAT[4];
float Cx[1];
float CP[2], CPCT[1];
float PCT[2], S1[1];
float Keps[2];
float KS[2], KSKT[2];

//KALMAN y
float y_post[2];
float A2[4], B2[2], C2[2];
float std_dev_v2, std_dev_w2;
float V2[4], W2[1];
float P_pri2[4], P_post2[4];
float y_pri[2];
float eps2[1], S2[1], K2[2];
float u2[1];
float y2[1];

float Ax2[2], Bu2[2];
float AP2[4], AT2[4], APAT2[4];
float Cx2[1];
float CP2[2], CPCT2[1];
float PCT2[2], S12[1];
float Keps2[2];
float KS2[2], KSKT2[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

volatile int flaga = 0;
volatile int flaga2 = 0;
//printf przeciazenie

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef* htim)
{
	if(htim == &htim11){
		flaga = 1;
	}

	if(htim == &htim10){
		flaga2 = 1;
	}
}

void Setup_UART_BT(UART_HandleTypeDef * UART){

	HAL_Delay(1000);
	HAL_GPIO_WritePin(KEY_GPIO_Port, KEY_Pin, GPIO_PIN_SET);
	HAL_Delay(100);                       //Przywrocenie ustawien fabrycznych
	HAL_UART_Transmit(UART, (uint8_t*) "AT+ORGL\r\n", strlen("AT+ORGL\r\n"), 100);
	HAL_Delay(100);                        //Wyzerowanie sparowanych urzadzen
	HAL_UART_Transmit(UART, (uint8_t*) "AT+RMAAD\r\n", strlen("AT+RMAAD\r\n"), 100);
	HAL_Delay(100);                               //Zmiana nazwy na  BT_STM
	HAL_UART_Transmit(UART, (uint8_t*) "AT+NAME=BT_STM\r\n", strlen("AT+NAME=BT_STM\r\n"), 100);
	HAL_Delay(100);                        //Ustawienie roli urzadzenia w tryb slave
	HAL_UART_Transmit(UART, (uint8_t*) "AT+ROLE=0\r\n", strlen("AT+ROLE=0\r\n"), 100);
	HAL_Delay(100);                    //Ustawienie predkosci, ilosci bitow stop, parzystosci
	HAL_UART_Transmit(UART, (uint8_t*) "AT+UART=115200,0,0\r\n", strlen("AT+UART=115200,0,0\r\n"), 100);
	HAL_Delay(100);
	HAL_GPIO_WritePin(KEY_GPIO_Port, KEY_Pin, GPIO_PIN_RESET);

}

void Setup_L3GD20(SPI_HandleTypeDef* spi){

	uint8_t tmp;

	//Rozpoczecie komunikacji spi z zyroskopem
	HAL_GPIO_WritePin(Zyro_SS_GPIO_Port, Zyro_SS_Pin, GPIO_PIN_RESET);

	tmp = ZYR_REG1_A;
	HAL_SPI_Transmit(spi, &tmp, 1, 20);
	tmp = ZYR_USTAWIENIA;
	HAL_SPI_Transmit(spi, &tmp, 1, 20);

	HAL_GPIO_WritePin(Zyro_SS_GPIO_Port, Zyro_SS_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void L3GD20_MultiRead(SPI_HandleTypeDef* spi){

	uint8_t tmp;

	//Rozpoczecie komunikacji spi z zyroskopem
	HAL_GPIO_WritePin(Zyro_SS_GPIO_Port, Zyro_SS_Pin, GPIO_PIN_RESET);

	//zczytanie wartosci wskazan 3ech osi naraz (6 bajtow = 3x(mlodszy i starszy bajt))
	tmp = ZYR_WSZYSTKIE_OSIE_ZCZYTANIE;
	HAL_SPI_Transmit(spi, &tmp, 1, 100);//adres wraz z multireadem
	HAL_SPI_Receive(spi, Dane, 6, 100);

	HAL_GPIO_WritePin(Zyro_SS_GPIO_Port, Zyro_SS_Pin, GPIO_PIN_SET);
}

/////////////////////////////////////////////////////////////
void Filtr_komplementarny(float xg, float yg, float zg, float xa, float ya, float za, float przyrost){

	float dt = 0.02;//50 Hz = 0.02s
	float pitchAcc; //zmienna przechowujaca wskazanie akcelerometru dla danego kata
	float rollAcc;  //zmienna przechowujaca wskazanie akcelerometru dla danego kata

	roll+= xa * dt;
	pitch+= ya * dt;

	//obliczenie sumy przyspieszen w celu wyeliminowania drgan

	if (przyrost > 0.4){

        //Obliczenia dla katu roll
        rollAcc = atan2f(xg, zg) * 180 / M_PI;
        roll = roll * 0.98 + rollAcc * 0.02;

		//Obliczenia dla katu pitch
        pitchAcc = atan2f(yg, zg) * 180 / M_PI;
        pitch = pitch * 0.98 + pitchAcc * 0.02;

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void KalmanInit(float xg, float zg, float za){
	float dt;


	dt = 0.02;

	A[0] = 1;
	A[1] = -dt;
	A[2] = 0;
	A[3] = 1;

	B[0] = dt;
	B[1] = 0;

	C[0] = 1;
	C[1] = 0;

	std_dev_v = 1;
	std_dev_w = 2;
	V[0] = std_dev_v * std_dev_v * dt;
	V[1] = 0;
	V[2] = 0;
	V[3] = std_dev_v * std_dev_v * dt;
	W[0] = std_dev_w * std_dev_w;

	/* Wartosci poczatkowe filtru */
	P_post[0] = 1;
	P_post[1] = 0;
	P_post[2] = 0;
	P_post[3] = 1;

	acc_x = xg;
	acc_z = zg;
	x_post[0] = atan2f(acc_x,acc_z) * 180 / M_PI;
	x_post[1] = 0;

	HAL_Delay(150);
}

void KalmanInitY(float yg, float zg, float za){
	float dt;


	dt = 0.02;

	A2[0] = 1;
	A2[1] = -dt;
	A2[2] = 0;
	A2[3] = 1;

	B2[0] = dt;
	B2[1] = 0;

	C2[0] = 1;
	C2[1] = 0;

	std_dev_v2 = 1;
	std_dev_w2 = 2;
	V2[0] = std_dev_v2 * std_dev_v2 * dt;
	V2[1] = 0;
	V2[2] = 0;
	V2[3] = std_dev_v2 * std_dev_v2 * dt;
	W2[0] = std_dev_w2 * std_dev_w2;

	/* Wartosci poczatkowe filtru */
	P_post2[0] = 1;
	P_post2[1] = 0;
	P_post2[2] = 0;
	P_post2[3] = 1;

	acc_y = yg;
	acc_z = zg;
	y_post[0] = atan2f(acc_y,acc_z) * 180 / M_PI;
	y_post[1] = 0;

	HAL_Delay(150);
}

void filtrKalmana(float xg, float zg, float za){

		// I
		u[0] = za * 250 / 32768;
		matrix_2x2_mul_2x1(A, x_post, Ax);
		mx2x1_tim_mx1x1(B, u, Bu);
		matrix_2x1_add_2x1(Ax, Bu, x_pri);

		// II
		matrix_2x2_mul_2x2(A, P_post, AP);
		matrix_2x2_trans(A, AT);
		matrix_2x2_mul_2x2(AP, AT, APAT);
		matrix_2x2_add_2x2(APAT, V, P_pri);

		// III
		acc_x = xg;
		acc_z = zg;
		y[0] = atan2f(acc_x,acc_z ) * 180 / M_PI;
		mx1x2_tim_mx2x1(C, x_pri, Cx);
		eps[0] = y[0] - Cx[0];

		// IV
		mx1x2_tim_mx2x2(C, P_pri, CP);
		mx1x2_tim_mx2x1(C, C, CPCT);
		S[0] = CPCT[0] + W[0];

		// V
		matrix_2x2_mul_2x1(P_pri, C, PCT);
		S1[0] = 1 / S[0];
		mx2x1_tim_mx1x1(PCT, S1, K);

		// VI
		mx2x1_tim_mx1x1(K, eps, Keps);
		matrix_2x1_add_2x1(x_pri, Keps, x_post);

		// VII
		mx2x1_tim_mx1x1(K, S, KS);
		matrix_2x1_mul_1x2(KS, K, KSKT);
		matrix_2x2_sub_2x2(P_pri, KSKT, P_post);


}

void filtrKalmanaY(float yg, float zg, float za){

		// I
		u2[0] = za * 250 / 32768;
		matrix_2x2_mul_2x1(A2, y_post, Ax2);
		mx2x1_tim_mx1x1(B2, u2, Bu2);
		matrix_2x1_add_2x1(Ax2, Bu2, y_pri);

		// II
		matrix_2x2_mul_2x2(A2, P_post2, AP2);
		matrix_2x2_trans(A2, AT2);
		matrix_2x2_mul_2x2(AP2, AT2, APAT2);
		matrix_2x2_add_2x2(APAT2, V2, P_pri2);

		// III
		acc_y = yg;
		acc_z = zg;
		y2[0] = atan2f(acc_y,acc_z ) * 180 / M_PI;
		mx1x2_tim_mx2x1(C2, y_pri, Cx2);
		eps2[0] = y2[0] - Cx2[0];

		// IV
		mx1x2_tim_mx2x2(C2, P_pri2, CP2);
		mx1x2_tim_mx2x1(C2, C2, CPCT2);
		S2[0] = CPCT2[0] + W2[0];

		// V
		matrix_2x2_mul_2x1(P_pri2, C2, PCT2);
		S12[0] = 1 / S2[0];
		mx2x1_tim_mx1x1(PCT2, S12, K2);

		// VI
		mx2x1_tim_mx1x1(K2, eps2, Keps2);
		matrix_2x1_add_2x1(y_pri, Keps2, y_post);

		// VII
		mx2x1_tim_mx1x1(K2, S2, KS2);
		matrix_2x1_mul_1x2(KS2, K2, KSKT2);
		matrix_2x2_sub_2x2(P_pri2, KSKT2, P_post2);


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
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

  //Zablokowanie komunikacji z zyroskopem
  HAL_GPIO_WritePin(Zyro_SS_GPIO_Port, Zyro_SS_Pin, GPIO_PIN_SET);


  //Wywolanie inicjalizacji ustawien zyroskopu
  //Setup_UART_BT(&huart2);

  //AKCELEROMETR - zmienne pomocnicze do komunikacji z akcelerometrem
  uint8_t USTAWIENIA = ACC_USTAWIENIA;
  uint8_t RESOLUTION = ACC_SET_4G;

  //AKCELEROMETR - aktywacja, 100Hz, oś XYZ
  HAL_I2C_Mem_Write(&hi2c1, ACC_ADRES, ACC_CTRL_REG1_A, 1, &USTAWIENIA, 1, 100);

  //AKCELEROMETR - zmiana zakresu pomiarowego z +-2g na +-4g
  HAL_I2C_Mem_Write(&hi2c1, ACC_ADRES, ACC_CTRL_REG4_A, 1, &RESOLUTION, 1, 100);

  //ZYROSKOP - setup
  Setup_L3GD20(&hspi1);

  //Kalman init
  KalmanInit(X_g,Z_g,Z_kat);

  KalmanInitY(Y_g,Z_g,Z_kat);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //TIM11 - 50Hz TIM10 - 10Hz
  HAL_TIM_Base_Start_IT(&htim11);
  HAL_TIM_Base_Start_IT(&htim10);

  while (1)
  {
	  if(Polaczenie == 1){

		  Button_B_mem = Button_B;
		  Button_B = 0;
		   if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {

			   if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {

				   Button_B = 1;

					   if (flaga == 1){



						   //Zczytanie kolejnych 6ściu bajtów wskazań z kolejno osi x, y, z (3x(mlodszy + starszy bajt))
						   HAL_I2C_Mem_Read(&hi2c1, ACC_ADRES, ACC_WSZYSTKIE_OSIE_ZCZYTANIE, 1, Dane, 6, 100);
						   X = ((Dane[1] << 8) | Dane[0]);
						   Y = ((Dane[3] << 8) | Dane[2]);
						   Z = ((Dane[5] << 8) | Dane[4]);

						   //Przeksztalcenie w faktyczna jednostke wskazan
						   X_g = ((float) X * 4.0) / (float) INT16_MAX;
						   Y_g = ((float) Y * 4.0) / (float) INT16_MAX;
						   Z_g = ((float) Z * 4.0) / (float) INT16_MAX;

						   X_roznica = fabs(X_mem - X_g);
						   Y_roznica = fabs(Y_mem - Y_g);
						   Z_roznica = fabs(Z_mem - Z_g);


						   L3GD20_MultiRead(&hspi1);

						   X_kat = ((float)((int16_t)((Dane[1] << 8) | Dane[0])) * 250.0)/(float) INT16_MAX;
						   Y_kat = ((float)((int16_t)((Dane[3] << 8) | Dane[2])) * 250.0)/(float) INT16_MAX;
						   Z_kat = ((float)((int16_t)((Dane[5] << 8) | Dane[4])) * 250.0)/(float) INT16_MAX;

						   if(X_roznica > 0.15)HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
						   if(Y_roznica > 0.15)HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
						   if(Z_roznica > 0.15)HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);

						   filtrKalmana(X_g,Z_g,Z_kat);
						   filtrKalmanaY(Y_g,Z_g,Z_kat);

						   Filtr_komplementarny(X_g,Y_g,Z_g, X_kat,Y_kat,Z_kat,X_roznica+Y_roznica+Z_roznica);

						   Rozmiar = sprintf((char *)Wiadomosc, "Xg: %f Yg: %f Zg: %f Xa: %f Ya: %f Za: %f Rkom: %f Pkom: %f Xpost: %f Ypost: %f B: %d\n", X_g,Y_g, Z_g, X_kat,Y_kat,Z_kat,roll,pitch,x_post[0],y_post[0],Button_B);

						   HAL_UART_Transmit(&huart2, (uint8_t*) Wiadomosc,  Rozmiar, 100);

						   HAL_Delay(5);

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

		   if(Button_B_mem == Button_B && Button_B_mem == 0 && flaga == 1){

			   flaga = 0;
			   Rozmiar = sprintf((char *)Wiadomosc, "Xg: %f Yg: %f Zg: %f Xa: %f Ya: %f Za: %f Rkom: %f Pkom: %f Xpost: %f Ypost: %f B: %d\n", X_g,Y_g, Z_g, X_kat,Y_kat,Z_kat,roll,pitch,x_post[0],y_post[0],Button_B);

			   HAL_UART_Transmit(&huart2, (uint8_t*) Wiadomosc,  Rozmiar, 100);

			   HAL_Delay(5);
		   }
	  }
	  if(flaga2 == 1){

		   HAL_UART_Receive(&huart2, &Odbior, 1,10);

		   if(Odbior == '1'){

			   Polaczenie = 1;

		   }

		   if(Odbior == '0'){

			   Polaczenie = 0;

		   }
		   flaga2 = 0;

	  }

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
