/*
 * Motor.c
 *
 *  Created on: Jun 26, 2025
 *      Author: kccistc
 */
#include "Motor.h"

TIM_HandleTypeDef *hmotorTim;
uint32_t motorTimChannel;

#define arr 	2000-1

// dot 번호
// 1 2
// 3 4
// 5 6


static TIM_HandleTypeDef* motorTim[6] = {
		&htim5, &htim3, &htim5, &htim3, &htim3, &htim3
};

static uint32_t motorCh[6] = {
		TIM_CHANNEL_1, TIM_CHANNEL_1, TIM_CHANNEL_2,
		TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4
};

void Motor_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
	hmotorTim = htim;
	motorTimChannel = channel;
}

void Motor_SetAngle(uint32_t degree)
{
	if (degree > 180) degree = 180;

	float pulse_ms = 1.0f + ((float)degree / 90.0f);  // 0도=1.0ms, 90도=2.0ms, 180도=3.0ms
	uint32_t ccr = (uint32_t)(pulse_ms * 100.0f);

	__HAL_TIM_SET_AUTORELOAD(hmotorTim, arr);
	__HAL_TIM_SET_COMPARE(hmotorTim, motorTimChannel, ccr);
}

void Motor_Start()
{
	HAL_TIM_PWM_Start(hmotorTim, motorTimChannel);
}

void Motor_Stop()
{
	HAL_TIM_PWM_Stop(hmotorTim, motorTimChannel);
}

void Motor_DisplayBits(uint8_t bits6)
{

	for (uint8_t dot = 0; dot < 6; dot++) {
		Motor_Init(motorTim[dot], motorCh[dot]);
		if (bits6 & (1 << dot)) {
			Motor_SetAngle(90);
		} else {
			Motor_SetAngle(0);
		}
		Motor_Start();
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		HAL_TIM_Base_Start_IT(&htim2);
	}

}

void Motor_ResetAllDots()
{
	for (uint8_t dot = 0; dot < 6; dot++) {
		Motor_Init(motorTim[dot], motorCh[dot]);
		Motor_SetAngle(0);
	}
	HAL_TIM_Base_Stop_IT(&htim2);  // 1회용이면 정지
}
