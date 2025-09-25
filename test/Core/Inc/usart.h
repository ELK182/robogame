/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdbool.h>
#include "motor.h"
#include "communication.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
#define UART_BUFFER_SIZE 100  // ���󻺳�������߿ɿ���
#define HEARTBEAT_INTERVAL_MS 1000  // ���������ͼ��

// ͨ�������
#define COMMAND_STOP       "STOP\n"
#define COMMAND_START      "START\n"
#define COMMAND_STATUS     "STATUS\n"

// ��Ӧ��Ϣ����
#define RESPONSE_ACK       "ACK\n"
#define RESPONSE_READY     "READY\n"
#define RESPONSE_ERROR     "ERROR\n"
#define HEARTBEAT_MESSAGE  "STM32_ALIVE\n"

// ͨ��״̬������
#define UART_STATE_IDLE       0
#define UART_STATE_RECEIVING  1
#define UART_STATE_PROCESSING 2
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
/* USER CODE BEGIN Prototypes */
// USART通信相关
void USART1_InitCommunication(void);
void USART2_InitCommunication(void);
void USART3_InitCommunication(void);
void USART1_ProcessReceivedData(void);
void USART2_ProcessReceivedData(void);
void USART3_ProcessReceivedData(void);
void USART1_SendString(const char* str);
void USART2_SendString(const char* str);
void USART3_SendString(const char* str);

// 命令检查函数
bool USART1_HasCommand(void);
const char* USART1_GetCommand(void);
void USART1_ClearCommand(void);
bool USART2_HasCommand(void);
const char* USART2_GetCommand(void);
void USART2_ClearCommand(void);
bool USART3_HasCommand(void);
const char* USART3_GetCommand(void);
void USART3_ClearCommand(void);

// 状态查询函数
bool USART1_IsConnected(void);
void USART1_ResetConnectionStatus(void);
bool USART2_IsConnected(void);
void USART2_ResetConnectionStatus(void);
bool USART3_IsConnected(void);
void USART3_ResetConnectionStatus(void);

// 系统状态查询函数
int32_t USART1_IsSystemRunning(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
