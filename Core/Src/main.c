/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"
#include "atk_qmi8658.h"
#include "sys.h"
#include "delay.h"
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"

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


int fputc(int ch, FILE *f)
{
 
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

float accel[3]; 
float gyro[3];  
float temp;     

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
delay_init(72);  
printf("Starting QMI8658A test...\r\n"); 

while(atk_qmi8658_init() != 0) 
{
    printf("QMI8658A Check Failed!\r\n"); 
}
printf("QMI8658A Ready!\r\n"); 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		/* USER CODE BEGIN WHILE */
while (1)
{
    temp = atk_qmi8658_get_temperature(); 

    // ------------------- ??????? -------------------
    // ???? 12 ??????!
    uint8_t raw_buf[12];
    
    // ? 0x35 ??,??????? 12 ??????
    atk_qmi8568_read_nbytes(0x35, raw_buf, 12); 

    // 1. ??????? (?????)
    int16_t raw_acc_x = (raw_buf[1] << 8) | raw_buf[0];
    int16_t raw_acc_y = (raw_buf[3] << 8) | raw_buf[2];
    int16_t raw_acc_z = (raw_buf[5] << 8) | raw_buf[4];

    // 2. ??????? (?? 6 ??????)
    int16_t raw_gyr_x = (raw_buf[7] << 8) | raw_buf[6];
    int16_t raw_gyr_y = (raw_buf[9] << 8) | raw_buf[8];
    int16_t raw_gyr_z = (raw_buf[11] << 8) | raw_buf[10];

    // 3. ?????:±16g ??,???? 2048 LSB/g
    float acc_x = (float)raw_acc_x / 2048.0f;
    float acc_y = (float)raw_acc_y / 2048.0f;
    float acc_z = (float)raw_acc_z / 2048.0f;

    // 4. ?????:??????? ±2048 dps (?/?) ??
    // 16???????? 32768? 32768 / 2048 = 16.0 
    // ?????? 16.0 LSB/dps
    float gyr_x = (float)raw_gyr_x / 16.0f;
    float gyr_y = (float)raw_gyr_y / 16.0f;
    float gyr_z = (float)raw_gyr_z / 16.0f;
    // -------------------------------------------------------------

    // 5. ???????
    printf("Acc(g): X=%.2f, Y=%.2f, Z=%.2f | Gyr(dps): X=%.1f, Y=%.1f, Z=%.1f\r\n", 
            acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z);

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
    HAL_Delay(500);  // ??????? 500ms,????????
}
/* USER CODE END WHILE */
	
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
