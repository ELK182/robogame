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
#include "communication.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"
#include "track.h"
#include "usart.h"
#include "string.h"

/* 私有变量 ----------------------------------------------------------------------------------------------------------*/

/* 私有函数声明 ------------------------------------------------------------------------------------------------------*/
static void processServoBoardSignal(uint8_t *data, uint16_t length);
static void processRaspberryPiSignal(uint8_t *data, uint16_t length);
static void processAnotherMcuSignal(uint8_t *data, uint16_t length);

/* 函数实现 ----------------------------------------------------------------------------------------------------------*/
/**
  * @brief  初始化取镖模块
  * @param  None
  * @retval None
  */
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
void processUartCommands(uint8_t *_Rx, uint16_t length, UART_HandleTypeDef *huart)
{
    // 检查数据是否有效
    if (_Rx == NULL || _Rx[0] == 0) {
        return;
    }
    
    // 添加调试信息，打印接收到的数据
    USART1_SendString("Received data: ");
    USART1_SendString((const char*)_Rx);
    USART1_SendString("\n");
    
    // 寻找命令结束标记
    uint16_t i = 0;
    while (i < UART_BUFFER_SIZE && _Rx[i] != '\n' && _Rx[i] != '\r' && _Rx[i] != 0) {
        i++;
    }
    
    // 如果找到了结束标记或有数据，处理命令
    if (i > 0 && i < UART_BUFFER_SIZE) {
        // 数据的第一位区分信号来源
        switch (_Rx[0]) {
            case '1': // 舵机开发板信号
                processServoBoardSignal(_Rx + 1, i - 1);
                break;
            case '2': // 树莓派信号
                processRaspberryPiSignal(_Rx + 1, i - 1);
                break;
            case '3': // 另一个单片机信号
                processAnotherMcuSignal(_Rx + 1, i - 1);
                break;
            default: // 未知信号来源，也尝试处理
                USART1_SendString("Unknown signal source, trying to process anyway\n");
                processRaspberryPiSignal(_Rx, i); // 尝试作为树莓派信号处理
                break;
        }
    }
    
    // 清空缓冲区，准备接收下一条命令
    memset(_Rx, 0, UART_BUFFER_SIZE);
}

/**
  * @brief  处理舵机开发板的信号
  * @param  data: 数据部分（去掉第一位标识符）
  * @param  length: 数据长度
  * @retval None
  */
static void processServoBoardSignal(uint8_t *data, uint16_t length)
{
    // 实现处理舵机开发板信号的逻辑
    // 例如，解析指令并控制舵机运动
    
    // 示例：将接收到的指令回显
    USART1_SendString("Received servo board signal: ");
    USART1_SendString((const char*)data);
    USART1_SendString("\n");
    
    // 在这里添加具体的舵机控制逻辑
}

/**
  * @brief  处理树莓派的信号
  * @param  data: 数据部分（去掉第一位标识符）
  * @param  length: 数据长度
  * @retval None
  */
static void processRaspberryPiSignal(uint8_t *data, uint16_t length)
{
    // 检查数据是否有效
    if (data == NULL || length < 1) {
        return;
    }
    
    // 实现处理树莓派信号的逻辑
    // 根据第二位数字区分不同功能
    switch (data[0]) {
        case '1': // 移动控制
            if (length >= 2) {
                // 解析移动方向和速度
                char moveDirection = data[1];
                int speed = 300; // 默认速度
                
                // 如果有速度参数，解析速度值
                if (length >= 3) {
                    // 简单的速度解析，实际应用中可能需要更复杂的解析逻辑
                    speed = ((data[2] - '0') * 100);
                    if (speed > 1000) speed = 1000; // 限制最大速度
                    if (speed < 0) speed = 0; // 限制最小速度
                }
                
                // 根据方向控制移动
                switch (moveDirection) {
                    case '0': // 停止
                        MotorControl(0, 0, 0, 0, 0);
                        USART1_SendString("Raspberry Pi: Stop\n");
                        break;
                    case '1': // 前进
                        MotorControl(1, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Forward\n");
                        break;
                    case '2': // 后退
                        MotorControl(2, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Backward\n");
                        break;
                    case '3': // 左移
                        MotorControl(3, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Move Left\n");
                        break;
                    case '4': // 右移
                        MotorControl(4, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Move Right\n");
                        break;
                    case '5': // 左旋
                        MotorControl(5, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Rotate Left\n");
                        break;
                    case '6': // 右旋
                        MotorControl(6, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Rotate Right\n");
                        break;
                    default:
                        USART1_SendString("Raspberry Pi: Unknown move direction\n");
                        break;
                }
            }
            break;
            
        case '2': // 侧边校准
            if (length >= 2) {
                // 解析校准方向
                char calibrateDirection = data[1];
                int ycalibrateSpeed = 200; // 校准速度（比正常速度慢）
							  int xcalibrateSpeed = 100;
                
                // 根据校准方向执行侧边校准
                switch (calibrateDirection) {
                    case '1': // 向前校准
                        MotorControl(1, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Left\n");
                        // 可以添加延时函数，让校准执行一段时间
                        break;
                    case '2': // 向后校准
                        MotorControl(2, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Right\n");
                        // 可以添加延时函数，让校准执行一段时间
                        break;
										case '3': // 向左校准
                        MotorControl(3, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Left\n");
                        // 可以添加延时函数，让校准执行一段时间
                        break;
                    case '4': // 向右校准
                        MotorControl(4, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Right\n");
                        // 可以添加延时函数，让校准执行一段时间
                        break;
										case '5': // 校准完成
											  MotorControl(0, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed);
										    USART2_SendString("FETCH\n");
                    default:
                        USART1_SendString("Raspberry Pi: Unknown calibration direction\n");
                        break;
                }
            }
            break;
            
        case '3': // 巡线模式
            USART1_SendString("Raspberry Pi: Start Line Following\n");
            // 执行巡线功能
            Track_LineFollower();
            break;
            
        default:
            // 回显接收到的指令
            USART1_SendString("Received Raspberry Pi signal: ");
            USART1_SendString((const char*)data);
            USART1_SendString("\n");
            break;
    }
}

/**
  * @brief  处理另一个单片机的信号
  * @param  data: 数据部分（去掉第一位标识符）
  * @param  length: 数据长度
  * @retval None
  */
static void processAnotherMcuSignal(uint8_t *data, uint16_t length)
{
    // 实现处理另一个单片机信号的逻辑
    // 例如，解析指令并执行相应操作
    
    // 示例：将接收到的指令回显
    USART1_SendString("Received another MCU signal: ");
    USART1_SendString((const char*)data);
    USART1_SendString("\n");
    
    // 在这里添加具体的单片机通信逻辑
}
