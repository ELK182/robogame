#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "tim.h"
#include "main.h"
void MotorControl(char direction,int left_front_MotorPWM, int right_front_MotorPWM,int left_behind_MotorPWM, int right_behind_MotorPWM);


#endif
