/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "motor.h"
#include "loop.h"
#include "track.h"
#include "communication.h"
extern int32_t system_running;
#define UART_BUFFER_SIZE 100 
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern uint8_t uart1_rx_buffer[UART_BUFFER_SIZE];
extern uint16_t uart1_rx_index ;
extern bool uart1_data_ready ;
extern bool uart1_command_ready ;
extern bool uart1_connected ;
extern uint8_t uart2_rx_buffer[UART_BUFFER_SIZE];
extern uint16_t uart2_rx_index ;
extern bool uart2_data_ready ;
extern bool uart2_command_ready ;
extern bool uart2_connected ;
// 锟斤拷锟経SART3锟斤拷全锟街憋拷锟斤拷
extern uint8_t uart3_rx_buffer[UART_BUFFER_SIZE];
extern uint16_t uart3_rx_index;
extern bool uart3_data_ready;
extern bool uart3_command_ready; 
extern bool uart3_connected;
// 系统锟斤拷锟斤拷状态锟斤拷锟?
extern uint8_t RxBuffer[10];
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM4)
  {
		
    // ÿ10msִ��һ�εĴ���
    // �������������Ҫ��ʱִ�е�����
  }
  /* USER CODE END Callback 0 */
}
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
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_TIM3_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_I2C1_Init();
    MX_TIM4_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    HAL_TIM_Base_Start_IT(&htim4);  // 启动TIM4定时器中断
    Track_Init();
    HAL_Delay(2000);
    /* USER CODE BEGIN 2 */
    // 初始化所有外设后，初始化通信
    USART1_InitCommunication();
    USART2_InitCommunication();
    USART3_InitCommunication();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
        // 处理接收到的数据
        USART1_ProcessReceivedData();
        USART2_ProcessReceivedData();
        USART3_ProcessReceivedData();
        
        // 短暂延时，避免CPU占用过高
        HAL_Delay(10);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief  UART Rx Event Callback
 * @param  huart: UART handle
 * @param  Size: 接收到的数据大小
 * @retval None
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1) {
        // 设置数据准备好标志
        uart1_data_ready = true;
        uart1_rx_index = Size;
        
        // 重新启动接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buffer, UART_BUFFER_SIZE - 1);
    }
    else if (huart->Instance == USART2) {
        // 设置数据准备好标志
        uart2_data_ready = true;
        uart2_rx_index = Size;
        
        // 重新启动接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buffer, UART_BUFFER_SIZE - 1);
    }
    else if (huart->Instance == USART3) {
        // 设置数据准备好标志
        uart3_data_ready = true;
        uart3_rx_index = Size;
        
        // 重新启动接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, uart3_rx_buffer, UART_BUFFER_SIZE - 1);
    }
}

/**
 * @brief  UART接收错误回调函数
 * @param  huart: UART handle
 * @retval None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        // 清除错误标志并重新启动接收
        __HAL_UART_CLEAR_OREFLAG(&huart1);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buffer, UART_BUFFER_SIZE - 1);
    }
    else if (huart->Instance == USART2) {
        // 清除错误标志并重新启动接收
        __HAL_UART_CLEAR_OREFLAG(&huart2);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buffer, UART_BUFFER_SIZE - 1);
    }
    else if (huart->Instance == USART3) {
        // 清除错误标志并重新启动接收
        __HAL_UART_CLEAR_OREFLAG(&huart3);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, uart3_rx_buffer, UART_BUFFER_SIZE - 1);
    }
}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
