#include "track.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"

// �ⲿ����ADC���ݻ�����
extern uint16_t adc_values[4];

// Ѳ�߲�������
#define THRESHOLD       2000    // �����������ֵ
#define BASE_SPEED      500     // �����ٶ�(0-1000)
#define ADJUST_SPEED    300     // �����ٶ�(0-1000)
#define SENSOR_LEFT     adc_values[0]  // ��ഫ����(PA4)
#define SENSOR_MID_LEFT adc_values[1]  // ���д�����(PA5)
#define SENSOR_MID_RIGHT adc_values[2] // ���д�����(PA6)
#define SENSOR_RIGHT    adc_values[3]  // �Ҳഫ����(PA7)

/**
  * @brief ��ʼ��Ѳ��ϵͳ
  * @param None
  * @retval None
  */
void Track_Init(void)
{
  // ����ADC DMA����
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 4);
  // ��ʼ��PWM���
}

/**
  * @brief ����С��Ѳ�߿���������
  * @param None
  * @retval None
  */
void Track_LineFollower(void)
{
  // ȫͣ״̬(���д�����δ��⵽����)
  if(SENSOR_LEFT < THRESHOLD && SENSOR_MID_LEFT < THRESHOLD &&
     SENSOR_MID_RIGHT < THRESHOLD && SENSOR_RIGHT < THRESHOLD)
  {
    MotorControl(0, 0, 0, 0, 0);  // ֹͣ
  }
  // ֱ����ʻ(�м�������������⵽����)
  else if(SENSOR_MID_LEFT > THRESHOLD && SENSOR_MID_RIGHT > THRESHOLD)
  {
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // ǰ��
  }
  // ��΢��ת(��ഫ������⵽����)
  else if(SENSOR_LEFT > THRESHOLD || SENSOR_MID_LEFT > THRESHOLD)
  {
    MotorControl(5, ADJUST_SPEED, BASE_SPEED, ADJUST_SPEED, BASE_SPEED);  // ����
  }
  // ��΢��ת(�Ҳഫ������⵽����)
  else if(SENSOR_RIGHT > THRESHOLD || SENSOR_MID_RIGHT > THRESHOLD)
  {
    MotorControl(6, BASE_SPEED, ADJUST_SPEED, BASE_SPEED, ADJUST_SPEED);  // ����
  }
  // �����ƫʱ��������
  else if(SENSOR_LEFT > THRESHOLD && SENSOR_MID_LEFT < THRESHOLD)
  {
    MotorControl(3, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // ����
  }
  // �����ƫʱ��������
  else if(SENSOR_RIGHT > THRESHOLD && SENSOR_MID_RIGHT < THRESHOLD)
  {
    MotorControl(4, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // ����
  }
}