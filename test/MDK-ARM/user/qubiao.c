/**
  ******************************************************************************
  * @file    qubiao.c
  * @brief   ����λ��ͨ�ſ��Ƶ���˶�
  *
  * @author  qgt
  * @date    2025-7-2
  * @version v1.0
  ******************************************************************************
  */

/* ������Ҫ��ͷ�ļ� ------------------------------------------------------------------------------------------------*/
#include "qubiao.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"
#include "track.h"
#include "usart.h"
#include "string.h"

/* ˽�б��� ----------------------------------------------------------------------------------------------------------*/
static const uint16_t BASE_SPEED = 50;  // �����ٶ�ֵ

/* ˽�к������� ------------------------------------------------------------------------------------------------------*/
static void processUartCommands(void);

/* ����ʵ�� ----------------------------------------------------------------------------------------------------------*/
/**
  * @brief  ��ʼ��ȡ��ģ��
  * @param  None
  * @retval None
  */
void qubiao_Init(void)
{
  // ��ʼ���������
  MotorControl(0, 0, 0, 0, 0);  // ��ʼ״̬Ϊֹͣ
  // ��ʼ��ADC��������Ҫ��ģ��
  // ...
}

/**
  * @brief  ȡ��ģ��������
  * @param  None
  * @retval None
  */
void qubiao(void)
{
  
  // ������λ�����͵�ָ��
}

/**
  * @brief  ���´�����״̬
  * @param  None
  * @retval None
  */
/**
  * @brief  ������λ�����͵�ָ��
  * @param  None
  * @retval None
  */
static void processUartCommands(void)
{
  if (USART1_HasCommand())
  {
    const char* command = USART1_GetCommand();
    
    // ����X�����ָ��
    if (strcmp(command, "XOK\n") == 0)
    {
      // �յ�XTRUEָ�ֹͣ
      MotorControl(0, 0, 0, 0, 0);
      USART1_SendString("ACK: STOP\n");
    }
    else if (strcmp(command, "XGO\n") == 0)
    {
      // �յ�XGOָ���ǰ��
      MotorControl(1, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: FORWARD\n");
    }
    else if (strcmp(command, "XBACK\n") == 0)
    {
      // �յ�XBACKָ������
      MotorControl(2, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: BACKWARD\n");
    }
    // ����Y�����ָ��
    else if (strcmp(command, "YOK\n") == 0)
    {
      // �յ�YTRUEָ�ֹͣ
      MotorControl(0, 0, 0, 0, 0);
      USART1_SendString("ACK: STOP\n");
    }
    else if (strcmp(command, "YLEFT\n") == 0)
    {
      // �յ�YGOָ�������
      MotorControl(3, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: LEFT\n");
    }
    else if (strcmp(command, "YRIGHT\n") == 0)
    {
      // �յ�YBACKָ�������
      MotorControl(4, BASE_SPEED, BASE_SPEED, BASE_SPEED, BASE_SPEED);
      USART1_SendString("ACK: RIGHT\n");
    }
		else if (strcmp(command,"TRUE\n") == 0)
				USART2_SendString("FETCH\n");
    else
    {
      // �յ�δָ֪��
      USART1_SendString("ERROR: UNKNOWN COMMAND\n");
    }
    
    // ����Ѵ��������
    USART1_ClearCommand();
  }
}
