/**
 * @file    Motor.c
 * @brief   ???????
 *
 * @author  qgt
 * @date    2025-7-2
 * @version v1.0
 */

/* ???????? ----------------------------------------------------------------------------------------------------------*/
#include "motor.h"
#include "gpio.h"
#include "tim.h"
/* ?????????? --------------------------------------------------------------------------------------------------------*/


/* ???????? ------------------------------------------------------------------------------------------------------------*/
/************************************************************************************************************************

 ***********************************************************************************************************************/

/************************************************************************************************************************
 * @brief   BDC??????????????
 ***********************************************************************************************************************/
/**
  *  @brief ??????????????????????
  *  @param None
  *  @retval None
  */
// ???????????
/**
  *  @brief 控制电机进行正转、反转、停止
  *  @param None
  *  @retval None
  */
// 各轮子分别正转
void Left_Front_Go()     // 左前轮正转——AIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
}
 
void Right_Front_Go()    // 右前轮正转——BIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
}
void Left_Behind_Go()    // 左后轮正转——CIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
}
void Right_Behind_Go()   // 右后轮正转——DIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);
}
 
//各轮子分别反转
void Left_Front_Back()   // 左前轮反转——AIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
}
 
void Right_Front_Back()  // 右前轮反转——BIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
}
void Left_Behind_Back()  // 左后轮反转——CIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
}
 
void Right_Behind_Back() // 右后轮反转——DIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET);
}
 
// 各轮子分别停止
void Left_Front_Stop()   // 左前轮停止——AIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
}
 
void Right_Front_Stop()  // 右前轮停止——BIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
}
void Left_Behind_Stop()  // 左后轮停止——CIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
}
void Right_Behind_Stop() // 右后轮停止——DIN
{
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET);
}
/**
  *  @brief 控制麦克纳姆小车的各种运动方式
  *  @param 运动方向，四个电机的PWM值
  *  @retval None
  */
void MotorControl(char direction,int left_front_MotorPWM, int right_front_MotorPWM,int left_behind_MotorPWM, int right_behind_MotorPWM)
{
  switch(direction)
  {
    case 0: // 停止
      Left_Front_Stop();
      Right_Front_Stop();
      Left_Behind_Stop();
      Right_Behind_Stop();
      left_front_MotorPWM = 0;
      right_front_MotorPWM = 0;
      left_behind_MotorPWM = 0;
      right_behind_MotorPWM = 0;break;
    case 1: // 前进
      Left_Front_Go();
      Right_Front_Go();
      Left_Behind_Go();
      Right_Behind_Go();break;
    case 2: // 后退
      Left_Front_Back();
      Right_Front_Back();
      Left_Behind_Back();
      Right_Behind_Back();break;
    case 3: // 左移
      Left_Front_Back();
      Right_Front_Go();
      Left_Behind_Go();
      Right_Behind_Back();break;
    case 4: // 右移
      Left_Front_Go();
      Right_Front_Back();
      Left_Behind_Back();
      Right_Behind_Go();break;
    case 5: // 左旋
      Left_Front_Back();
      Right_Front_Go();
      Left_Behind_Back();
      Right_Behind_Go();break;
    case 6: // 右旋
      Left_Front_Go();
      Right_Front_Back();
      Left_Behind_Go();
      Right_Behind_Back();break;
    case 7: // 左前方移动
      Left_Front_Stop();
      Right_Front_Go();
      Left_Behind_Go();
      Right_Behind_Stop();break;
    case 8: // 右前方移动
      Left_Front_Go();
      Right_Front_Stop();
      Left_Behind_Stop();
      Right_Behind_Go();break;
    case 9: // 左后方移动
      Left_Front_Back();
      Right_Front_Stop();
      Left_Behind_Stop();
      Right_Behind_Back();break;
    case 10: // 右后方移动
      Left_Front_Stop();
      Right_Front_Back();
      Left_Behind_Back();
      Right_Behind_Stop();break;
    case 11: // 绕前轴中点左旋
      Left_Front_Stop();
      Right_Front_Stop();
      Left_Behind_Back();
      Right_Behind_Go();break;
    case 12: // 绕前轴中点右旋
      Left_Front_Stop();
      Right_Front_Stop();
      Left_Behind_Go();
      Right_Behind_Back();break;
    case 13: // 绕后轴中点左旋
      Left_Front_Back();
      Right_Front_Go();
      Left_Behind_Stop();
      Right_Behind_Stop();break;
    case 14: // 绕后轴中点右旋
      Left_Front_Go();
      Right_Front_Back();
      Left_Behind_Stop();
      Right_Behind_Stop();break;
    default:break;
  }
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,left_front_MotorPWM);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,right_front_MotorPWM);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,left_behind_MotorPWM);
  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,right_behind_MotorPWM);
}
