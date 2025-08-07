/*
 * delay.c
 *
 *  Created on: Jun 27, 2025
 *      Author: kccistc
 */

#include "delay.h"

void delay_01ms(uint32_t time)
{
	uint32_t start;
	__HAL_TIM_SET_COUNTER(&htim4, 0);

	start = __HAL_TIM_GET_COUNTER(&htim4);

	while ((__HAL_TIM_GET_COUNTER(&htim4) - start) < time);

	//HAL_TIM_Base_Stop(&htim5);
}

