/**
  ******************************************************************************
  * @file    main_loop.h
  * @brief   Main loop function declaration
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_LOOP_H__
#define __MAIN_LOOP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void MainLoop_Process(void);
void MainLoop_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_LOOP_H__ */
