#ifndef __QUBIAO_H
#define __QUBIAO_H
#include "stm32f4xx_hal.h"
extern uint16_t adc_values[5];

void qubiao_Init(void);
void qubiao(void);
static void processUartCommands(void);

#endif /* __QUBIAO_H */
