/*
 * Motor.h
 *
 *  Created on: Jun 26, 2025
 *      Author: kccistc
 */

#ifndef DRIVER_BUZZER_BUZZER_H_
#define DRIVER_BUZZER_BUZZER_H_

#include "stm32f4xx_hal.h"
#include "tim.h"

void Motor_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void Motor_Start();
void Motor_StartAll();
void Motor_Stop();
void Motor_SetAngle(uint32_t degree);
void Motor_DisplayBits(uint8_t bits6);
void Motor_DisplayBraille(char data);
void Motor_ResetAllDots();

#endif /* DRIVER_BUZZER_BUZZER_H_ */
