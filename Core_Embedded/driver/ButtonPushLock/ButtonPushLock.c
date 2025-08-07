/*
 * ButtonPushLock.c
 *
 *  Created on: Jul 23, 2025
 *      Author: kccistc
 */

#include "ButtonPushLock.h"

void ButtonLock_Init(ButtonLock_Handler_t *hbtn, GPIO_TypeDef *GPIOx, uint16_t pinNum)
{
	hbtn->GPIOx = GPIOx;
	hbtn->pinNum = pinNum;
}

buttonlock_state_t ButtonLock_state(ButtonLock_Handler_t *hbtn)
{
	uint32_t curState;
	curState = HAL_GPIO_ReadPin(hbtn->GPIOx, hbtn->pinNum);

	if (curState) {
		HAL_Delay(2);
		return ACT_PUSHED_LK;
	}
	else {
		HAL_Delay(2);
		return ACT_RELEASED_LK;
	}
}


