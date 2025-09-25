#include "track.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"

// �ⲿ����ADC���ݻ�����
uint16_t adc_values[5];
// ����������״̬���鶨��
// ����״̬����
uint8_t sensor_states[5] = {0};  // 0:���,1:����,2:����,3:�Ҳ�;4:���
// Ѳ�߲�������
#define THRESHOLD       2000    // �����������ֵ
#define BASE_SPEED      300     // �����ٶ�(0-1000)
#define ADJUST_SPEED    300     // �����ٶ�(0-1000)
#define TURN_DURATION   2900     // ֱ�ǹ������ʱ��(ms)
#define TURN_SPEED      400     // �����ٶ�
#define TURN_DELAY      1200    // ת���ӳ�ʱ��(ms) <-- ����
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
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 5);
}

/**
  * @brief ����С��Ѳ�߿���������
  * @param None
  * @retval None
  */
static void UpdateSensorStates(void)
{
  // ����4��������,������ֵ����״̬(1=��⵽����,0=δ���)
  sensor_states[0] = (SENSOR_LEFT > THRESHOLD) ? 1 : 0;
  sensor_states[1] = (SENSOR_MID_LEFT > THRESHOLD) ? 1 : 0;
  sensor_states[2] = (SENSOR_MID_RIGHT > THRESHOLD) ? 1 : 0;
  sensor_states[3] = (SENSOR_RIGHT > THRESHOLD) ? 1 : 0;
	sensor_states[4] = (adc_values[4]>THRESHOLD) ? 1:0;
}
// ... existing code ...
void Track_LineFollower(void)
{ static uint8_t state = 0; // 0:����, 1:�ȴ�ת��, 2:ִ��ת��
	 static uint32_t delay_start_time = 0;
  static uint32_t turn_start_time = 0;
	static uint8_t turn_time = 0;
	UpdateSensorStates();
  // ȫͣ״̬(���д�����δ��⵽����)
	// ������ֱ�ǹ���״̬
 switch(state) {
    case 0: // ����Ѳ��״̬
			if(sensor_states[0] == 0 && sensor_states[1] == 0 &&
     sensor_states[2] == 0 && sensor_states[3] == 0)
  {
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED); //ǰ��
  }
  // ֱ����ʻ(�м�������������⵽����)
  else if(sensor_states[1] == 1 && sensor_states[2] == 1)
	{
    
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // ǰ��
  }
	else if(sensor_states[1] == 1 && sensor_states[2] == 1&&sensor_states[0] == 1 && sensor_states[3] == 1)
	{
    
    MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // ǰ��
  }
  // ��΢��ת(�������д�������⵽����)
  else if((sensor_states[0] == 1 || sensor_states[1] == 1) && 
          sensor_states[2] == 0 && sensor_states[3] == 0)
  {
    MotorControl(5, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // ����
		HAL_Delay(10);
  }
  // ��΢��ת(�Ҳ�����д�������⵽����)
  else if((sensor_states[2] == 1 || sensor_states[3] == 1) && 
          sensor_states[0] == 0 && sensor_states[1] == 0)
  {
    MotorControl(6, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);  // ����
		HAL_Delay(10);
  }
  // �����ƫ(������ഫ������⵽����)
  else if(sensor_states[0] == 1 && sensor_states[1] == 0)
  {
    MotorControl(3, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // ����
  }
  // �����ƫ(�����Ҳഫ������⵽����)
  else if(sensor_states[3] == 1 && sensor_states[2] == 0)
  {
    MotorControl(4, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED, ADJUST_SPEED);  // ����
  }
      // �����ֱ�ǹ�������
      if (sensor_states[3] == 1 && sensor_states[2] == 1 && 
          sensor_states[1] == 1 && sensor_states[0] == 0 &&turn_time == 2) {
				turn_time = 1;
        state = 1; // ����ȴ�״̬
        delay_start_time = HAL_GetTick();
        MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED); // ����ǰ��
      }
      break;
    
    case 1: // �ȴ�ת��״̬
      // �ȴ�TURN_DELAY(2000ms)��ִ��ת��
      if (HAL_GetTick() - delay_start_time >= TURN_DELAY) {
        state = 2; // ����ת��״̬
        turn_start_time = HAL_GetTick();
        MotorControl(6, TURN_SPEED, TURN_SPEED, TURN_SPEED, TURN_SPEED); // ��������
      }
      break;
    
    case 2: // ִ��ת��״̬
      // ����ת��TURN_DURATION(3000ms)
      if (HAL_GetTick() - turn_start_time >= TURN_DURATION) {
        state = 0; // �ָ�����״̬
      }
      break;
  }
} 
// ... existing code ...
