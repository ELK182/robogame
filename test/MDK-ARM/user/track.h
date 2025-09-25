#ifndef __TRACK_H
#define __TRACK_H

#include "stm32f4xx_hal.h"
extern uint16_t adc_values[5];
void Track_Init(void);
void Track_LineFollower(void);

#endif /* __TRACK_H */
