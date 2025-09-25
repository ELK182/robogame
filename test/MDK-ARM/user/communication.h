#ifndef __QUBIAO_H
#define __QUBIAO_H
#include "stm32f4xx_hal.h"
extern uint16_t adc_values[5];
void processUartCommands(uint8_t *Rx, uint16_t length, UART_HandleTypeDef *huart);

#endif /* __QUBIAO_H */
