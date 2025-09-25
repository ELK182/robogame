/**
  ******************************************************************************
  * @file    main_loop.c
  * @brief   Main loop function implementation
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

/* Includes ------------------------------------------------------------------*/
#include "loop.h"
#include "usart.h"
#include "motor.h"
#include "track.h"
#include "communication.h"

/* Private variables ---------------------------------------------------------*/
extern int32_t system_running;
extern int32_t dart_count;
/**
  * @brief  初始化主循环相关组件
  * @retval None


  * @brief  主循环处理函数
  * @retval None
  */
void MainLoop_Process(void)
{
  
}
