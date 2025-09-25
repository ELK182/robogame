#include "track.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"

// 外部声明ADC数据缓冲区
uint16_t adc_values[5];
// 新增传感器状态数组定义
// 新增状态变量
uint8_t sensor_states[5] = {0};  // 0:左侧,1:左中,2:右中,3:右侧;4:侧边
// 巡线参数配置
#define THRESHOLD       2000    // 传感器检测阈值
#define BASE_SPEED      300     // 基础速度(0-1000)
#define ADJUST_SPEED    300     // 调整速度(0-1000)
#define TURN_DURATION   2900     // 直角拐弯持续时间(ms)
#define TURN_SPEED      400     // 拐弯速度
#define TURN_DELAY      1200    // 转弯延迟时间(ms) <-- 新增
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
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 5);
}

/**
  * @brief 麦轮小车巡线控制主函数
  * @param None
  * @retval None
  */
static void UpdateSensorStates(void)
{
  // 遍历4个传感器,根据阈值更新状态(1=检测到黑线,0=未检测)
  sensor_states[0] = (SENSOR_LEFT > THRESHOLD) ? 1 : 0;
  sensor_states[1] = (SENSOR_MID_LEFT > THRESHOLD) ? 1 : 0;
  sensor_states[2] = (SENSOR_MID_RIGHT > THRESHOLD) ? 1 : 0;
  sensor_states[3] = (SENSOR_RIGHT > THRESHOLD) ? 1 : 0;
	sensor_states[4] = (adc_values[4]>THRESHOLD) ? 1:0;
}
// ... existing code ...
void Track_LineFollower(void)
{ static uint8_t state = 0; // 0:正常, 1:等待转弯, 2:执行转弯
	 static uint32_t delay_start_time = 0;
  static uint32_t turn_start_time = 0;
	static uint8_t turn_time = 0;
	UpdateSensorStates();
  // 全停状态(所有传感器未检测到黑线)
	// 处理右直角拐弯状态
 switch(state) {
    case 0: // 正常巡线状态
			if(sensor_states[0] == 0 && sensor_states[1] == 0 &&
     sensor_states[2] == 0 && sensor_states[3] == 0)
  {
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED); //前进
  }
  // 直线行驶(中间两个传感器检测到黑线)
  else if(sensor_states[1] == 1 && sensor_states[2] == 1)
	{
    
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // 前进
  }
	else if(sensor_states[1] == 1 && sensor_states[2] == 1&&sensor_states[0] == 1 && sensor_states[3] == 1)
	{
    
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // 前进
  }
  // 轻微左转(左侧或左中传感器检测到黑线)
  else if((sensor_states[0] == 1 || sensor_states[1] == 1) && 
          sensor_states[2] == 0 && sensor_states[3] == 0)
  {
    MotorControl(5, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // 左旋
		HAL_Delay(10);
  }
  // 轻微右转(右侧或右中传感器检测到黑线)
  else if((sensor_states[2] == 1 || sensor_states[3] == 1) && 
          sensor_states[0] == 0 && sensor_states[1] == 0)
  {
    MotorControl(6, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // 右旋
		HAL_Delay(10);
  }
  // 大幅左偏(仅最左侧传感器检测到黑线)
  else if(sensor_states[0] == 1 && sensor_states[1] == 0)
  {
    MotorControl(3, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // 左移
  }
  // 大幅右偏(仅最右侧传感器检测到黑线)
  else if(sensor_states[3] == 1 && sensor_states[2] == 0)
  {
    MotorControl(4, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // 右移
  }
      // 检测右直角拐弯条件
      if (sensor_states[3] == 1 && sensor_states[2] == 1 && 
          sensor_states[1] == 1 && sensor_states[0] == 0 &&turn_time == 2) {
				turn_time = 1;
        state = 1; // 进入等待状态
        delay_start_time = HAL_GetTick();
        MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED); // 保持前进
      }
      break;
    
    case 1: // 等待转弯状态
      // 等待TURN_DELAY(2000ms)后执行转弯
      if (HAL_GetTick() - delay_start_time >= TURN_DELAY) {
        state = 2; // 进入转弯状态
        turn_start_time = HAL_GetTick();
        MotorControl(6, TURN_SPEED, TURN_SPEED, TURN_SPEED, TURN_SPEED); // 启动右旋
      }
      break;
    
    case 2: // 执行转弯状态
      // 持续转弯TURN_DURATION(3000ms)
      if (HAL_GetTick() - turn_start_time >= TURN_DURATION) {
        state = 0; // 恢复正常状态
      }
      break;
  }
} 
// ... existing code ...
