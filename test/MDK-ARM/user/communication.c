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
#include "communication.h"
#include "motor.h"
#include "adc.h"
#include "tim.h"
#include "track.h"
#include "usart.h"
#include "string.h"

/* ˽�б��� ----------------------------------------------------------------------------------------------------------*/

/* ˽�к������� ------------------------------------------------------------------------------------------------------*/
static void processServoBoardSignal(uint8_t *data, uint16_t length);
static void processRaspberryPiSignal(uint8_t *data, uint16_t length);
static void processAnotherMcuSignal(uint8_t *data, uint16_t length);

/* ����ʵ�� ----------------------------------------------------------------------------------------------------------*/
/**
  * @brief  ��ʼ��ȡ��ģ��
  * @param  None
  * @retval None
  */
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
void processUartCommands(uint8_t *_Rx, uint16_t length, UART_HandleTypeDef *huart)
{
    // ��������Ƿ���Ч
    if (_Rx == NULL || _Rx[0] == 0) {
        return;
    }
    
    // ��ӵ�����Ϣ����ӡ���յ�������
    USART1_SendString("Received data: ");
    USART1_SendString((const char*)_Rx);
    USART1_SendString("\n");
    
    // Ѱ������������
    uint16_t i = 0;
    while (i < UART_BUFFER_SIZE && _Rx[i] != '\n' && _Rx[i] != '\r' && _Rx[i] != 0) {
        i++;
    }
    
    // ����ҵ��˽�����ǻ������ݣ���������
    if (i > 0 && i < UART_BUFFER_SIZE) {
        // ���ݵĵ�һλ�����ź���Դ
        switch (_Rx[0]) {
            case '1': // ����������ź�
                processServoBoardSignal(_Rx + 1, i - 1);
                break;
            case '2': // ��ݮ���ź�
                processRaspberryPiSignal(_Rx + 1, i - 1);
                break;
            case '3': // ��һ����Ƭ���ź�
                processAnotherMcuSignal(_Rx + 1, i - 1);
                break;
            default: // δ֪�ź���Դ��Ҳ���Դ���
                USART1_SendString("Unknown signal source, trying to process anyway\n");
                processRaspberryPiSignal(_Rx, i); // ������Ϊ��ݮ���źŴ���
                break;
        }
    }
    
    // ��ջ�������׼��������һ������
    memset(_Rx, 0, UART_BUFFER_SIZE);
}

/**
  * @brief  ��������������ź�
  * @param  data: ���ݲ��֣�ȥ����һλ��ʶ����
  * @param  length: ���ݳ���
  * @retval None
  */
static void processServoBoardSignal(uint8_t *data, uint16_t length)
{
    // ʵ�ִ������������źŵ��߼�
    // ���磬����ָ����ƶ���˶�
    
    // ʾ���������յ���ָ�����
    USART1_SendString("Received servo board signal: ");
    USART1_SendString((const char*)data);
    USART1_SendString("\n");
    
    // ��������Ӿ���Ķ�������߼�
}

/**
  * @brief  ������ݮ�ɵ��ź�
  * @param  data: ���ݲ��֣�ȥ����һλ��ʶ����
  * @param  length: ���ݳ���
  * @retval None
  */
static void processRaspberryPiSignal(uint8_t *data, uint16_t length)
{
    // ��������Ƿ���Ч
    if (data == NULL || length < 1) {
        return;
    }
    
    // ʵ�ִ�����ݮ���źŵ��߼�
    // ���ݵڶ�λ�������ֲ�ͬ����
    switch (data[0]) {
        case '1': // �ƶ�����
            if (length >= 2) {
                // �����ƶ�������ٶ�
                char moveDirection = data[1];
                int speed = 300; // Ĭ���ٶ�
                
                // ������ٶȲ����������ٶ�ֵ
                if (length >= 3) {
                    // �򵥵��ٶȽ�����ʵ��Ӧ���п�����Ҫ�����ӵĽ����߼�
                    speed = ((data[2] - '0') * 100);
                    if (speed > 1000) speed = 1000; // ��������ٶ�
                    if (speed < 0) speed = 0; // ������С�ٶ�
                }
                
                // ���ݷ�������ƶ�
                switch (moveDirection) {
                    case '0': // ֹͣ
                        MotorControl(0, 0, 0, 0, 0);
                        USART1_SendString("Raspberry Pi: Stop\n");
                        break;
                    case '1': // ǰ��
                        MotorControl(1, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Forward\n");
                        break;
                    case '2': // ����
                        MotorControl(2, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Backward\n");
                        break;
                    case '3': // ����
                        MotorControl(3, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Move Left\n");
                        break;
                    case '4': // ����
                        MotorControl(4, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Move Right\n");
                        break;
                    case '5': // ����
                        MotorControl(5, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Rotate Left\n");
                        break;
                    case '6': // ����
                        MotorControl(6, speed, speed, speed, speed);
                        USART1_SendString("Raspberry Pi: Rotate Right\n");
                        break;
                    default:
                        USART1_SendString("Raspberry Pi: Unknown move direction\n");
                        break;
                }
            }
            break;
            
        case '2': // ���У׼
            if (length >= 2) {
                // ����У׼����
                char calibrateDirection = data[1];
                int ycalibrateSpeed = 200; // У׼�ٶȣ��������ٶ�����
							  int xcalibrateSpeed = 100;
                
                // ����У׼����ִ�в��У׼
                switch (calibrateDirection) {
                    case '1': // ��ǰУ׼
                        MotorControl(1, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Left\n");
                        // ���������ʱ��������У׼ִ��һ��ʱ��
                        break;
                    case '2': // ���У׼
                        MotorControl(2, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed, xcalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Right\n");
                        // ���������ʱ��������У׼ִ��һ��ʱ��
                        break;
										case '3': // ����У׼
                        MotorControl(3, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Left\n");
                        // ���������ʱ��������У׼ִ��һ��ʱ��
                        break;
                    case '4': // ����У׼
                        MotorControl(4, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed);
                        USART1_SendString("Raspberry Pi: Calibrate Right\n");
                        // ���������ʱ��������У׼ִ��һ��ʱ��
                        break;
										case '5': // У׼���
											  MotorControl(0, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed, ycalibrateSpeed);
										    USART2_SendString("FETCH\n");
                    default:
                        USART1_SendString("Raspberry Pi: Unknown calibration direction\n");
                        break;
                }
            }
            break;
            
        case '3': // Ѳ��ģʽ
            USART1_SendString("Raspberry Pi: Start Line Following\n");
            // ִ��Ѳ�߹���
            Track_LineFollower();
            break;
            
        default:
            // ���Խ��յ���ָ��
            USART1_SendString("Received Raspberry Pi signal: ");
            USART1_SendString((const char*)data);
            USART1_SendString("\n");
            break;
    }
}

/**
  * @brief  ������һ����Ƭ�����ź�
  * @param  data: ���ݲ��֣�ȥ����һλ��ʶ����
  * @param  length: ���ݳ���
  * @retval None
  */
static void processAnotherMcuSignal(uint8_t *data, uint16_t length)
{
    // ʵ�ִ�����һ����Ƭ���źŵ��߼�
    // ���磬����ָ�ִ����Ӧ����
    
    // ʾ���������յ���ָ�����
    USART1_SendString("Received another MCU signal: ");
    USART1_SendString((const char*)data);
    USART1_SendString("\n");
    
    // ��������Ӿ���ĵ�Ƭ��ͨ���߼�
}
