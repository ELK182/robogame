#include "track.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"

// 外部声明ADC数据缓冲区
extern uint16_t adc_values[4];

// 巡线参数配置
#define THRESHOLD       2000    // 传感器检测阈值
#define BASE_SPEED      500     // 基础速度(0-1000)
#define ADJUST_SPEED    300     // 调整速度(0-1000)
#define SENSOR_LEFT     adc_values[0]  // 左侧传感器(PA4)
#define SENSOR_MID_LEFT adc_values[1]  // 左中传感器(PA5)
#define SENSOR_MID_RIGHT adc_values[2] // 右中传感器(PA6)
#define SENSOR_RIGHT    adc_values[3]  // 右侧传感器(PA7)

/**
  * @brief 初始化巡线系统
  * @param None
  * @retval None
  */
void Track_Init(void)
{
  // 启动ADC DMA传输
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 4);
  // 初始化PWM输出
}

/**
  * @brief 麦轮小车巡线控制主函数
  * @param None
  * @retval None
  */
void Track_LineFollower(void)
{
  // 全停状态(所有传感器未检测到黑线)
  if(SENSOR_LEFT < THRESHOLD && SENSOR_MID_LEFT < THRESHOLD &&
     SENSOR_MID_RIGHT < THRESHOLD && SENSOR_RIGHT < THRESHOLD)
  {
    MotorControl(0, 0, 0, 0, 0);  // 停止
  }
  // 直线行驶(中间两个传感器检测到黑线)
  else if(SENSOR_MID_LEFT > THRESHOLD && SENSOR_MID_RIGHT > THRESHOLD)
  {
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // 前进
  }
  // 轻微左转(左侧传感器检测到黑线)
  else if(SENSOR_LEFT > THRESHOLD || SENSOR_MID_LEFT > THRESHOLD)
  {
    MotorControl(5, ADJUST_SPEED, BASE_SPEED, ADJUST_SPEED, BASE_SPEED);  // 左旋
  }
  // 轻微右转(右侧传感器检测到黑线)
  else if(SENSOR_RIGHT > THRESHOLD || SENSOR_MID_RIGHT > THRESHOLD)
  {
    MotorControl(6, BASE_SPEED, ADJUST_SPEED, BASE_SPEED, ADJUST_SPEED);  // 右旋
  }
  // 大幅左偏时横向修正
  else if(SENSOR_LEFT > THRESHOLD && SENSOR_MID_LEFT < THRESHOLD)
  {
    MotorControl(3, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // 左移
  }
  // 大幅右偏时横向修正
  else if(SENSOR_RIGHT > THRESHOLD && SENSOR_MID_RIGHT < THRESHOLD)
  {
    MotorControl(4, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // 右移
  }
}