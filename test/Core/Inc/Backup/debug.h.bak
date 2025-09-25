#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/
typedef enum {
  DEBUG_LEVEL_NONE = 0,
  DEBUG_LEVEL_ERROR,
  DEBUG_LEVEL_WARNING,
  DEBUG_LEVEL_INFO,
  DEBUG_LEVEL_VERBOSE
} DebugLevel;

/* Exported constants --------------------------------------------------------*/
#define DEBUG_BUFFER_SIZE 256

/* Exported macro ------------------------------------------------------------*/
// 条件编译宏，可全局控制调试输出
#define DEBUG_ENABLED 1

#if DEBUG_ENABLED
  #define DEBUG_PRINT(level, ...) do { \
    if (debug_get_level() >= level) { \
      debug_printf(__VA_ARGS__); \
    } \
  } while(0)
#else
  #define DEBUG_PRINT(level, ...) ((void)0)
#endif

// 便捷宏
#define DEBUG_ERROR(...)   DEBUG_PRINT(DEBUG_LEVEL_ERROR, __VA_ARGS__)
#define DEBUG_WARNING(...) DEBUG_PRINT(DEBUG_LEVEL_WARNING, __VA_ARGS__)
#define DEBUG_INFO(...)    DEBUG_PRINT(DEBUG_LEVEL_INFO, __VA_ARGS__)
#define DEBUG_VERBOSE(...) DEBUG_PRINT(DEBUG_LEVEL_VERBOSE, __VA_ARGS__)

/* Exported functions prototypes ---------------------------------------------*/
void Debug_Init(UART_HandleTypeDef *huart);
void Debug_ProcessCommand(char *command);
void debug_printf(const char *fmt, ...);
void debug_set_level(DebugLevel level);
DebugLevel debug_get_level(void);
void debug_send_data(uint8_t *data, uint16_t size);
void debug_receive_callback(void);

// 特定于项目的调试函数
void Debug_SendSensorData(void);
void Debug_SendMotorPWM(uint16_t pwm1, uint16_t pwm2, uint16_t pwm3, uint16_t pwm4);
void Debug_SendTrackStatus(uint8_t *track_values);

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H */