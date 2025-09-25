/**
  ******************************************************************************
  * @file    qubiao.c
  * @brief   与上位机通信控制电机运动
  *
  * @author  qgt
  * @date    2025-7-2
  * @version v1.0
  ******************************************************************************
  */

/* 包含必要的头文件 ------------------------------------------------------------------------------------------------*/
#include "qubiao.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"
#include "track.h"
#include "usart.h"
#include "string.h"

/* 私有变量 ----------------------------------------------------------------------------------------------------------*/
static const uint16_t BASE_SPEED = 50;  // 基础速度值

/* 私有函数声明 ------------------------------------------------------------------------------------------------------*/
static void processUartCommands(void);

/* 函数实现 ----------------------------------------------------------------------------------------------------------*/
/**
  * @brief  初始化取镖模块
  * @param  None
  * @retval None
  */
void qubiao_Init(void)
{
  // 初始化电机控制
  MotorControl(0, 0, 0, 0, 0);  // 初始状态为停止
  // 初始化ADC和其他必要的模块
  // ...
}

/**
  * @brief  取镖模块主函数
  * @param  None
  * @retval None
  */
void qubiao(void)
{
  
  // 处理上位机发送的指令
}

/**
  * @brief  更新传感器状态
  * @param  None
  * @retval None
  */
/**
  * @brief  处理上位机发送的指令
  * @param  None
  * @retval None
  */
static void processUartCommands(void)
{
  if (USART1_HasCommand())
  {
    const char* command = USART1_GetCommand();
    
    // 处理X轴相关指令
    if (strcmp(command, "XOK\n") == 0)
    {
      // 收到XTRUE指令，停止
      MotorControl(0, 0, 0, 0, 0);
      USART1_SendString("ACK: STOP\n");
    }
    else if (strcmp(command, "XGO\n") == 0)
    {
      // 收到XGO指令，向前进
      MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: FORWARD\n");
    }
    else if (strcmp(command, "XBACK\n") == 0)
    {
      // 收到XBACK指令，向后退
      MotorControl(2, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: BACKWARD\n");
    }
    // 处理Y轴相关指令
    else if (strcmp(command, "YOK\n") == 0)
    {
      // 收到YTRUE指令，停止
      MotorControl(0, 0, 0, 0, 0);
      USART1_SendString("ACK: STOP\n");
    }
    else if (strcmp(command, "YLEFT\n") == 0)
    {
      // 收到YGO指令，向左移
      MotorControl(3, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: LEFT\n");
    }
    else if (strcmp(command, "YRIGHT\n") == 0)
    {
      // 收到YBACK指令，向右移
      MotorControl(4, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: RIGHT\n");
    }
		else if (strcmp(command,"TRUE\n") == 0)
				USART2_SendString("FETCH\n");
    else
    {
      // 收到未知指令
      USART1_SendString("ERROR: UNKNOWN COMMAND\n");
    }
    
    // 清除已处理的命令
    USART1_ClearCommand();
  }
}
