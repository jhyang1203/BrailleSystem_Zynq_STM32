/*
 * Button.c
 *
 *  Created on: Jun 19, 2025
 *      Author: rhoblack
 */

#include "Button.h"

enum {PUSHED=0, RELEASED};

void Button_Init(Button_Handler_t *hbtn, GPIO_TypeDef * GPIOx, uint16_t pinNum)
{
	hbtn->GPIOx = GPIOx;
	hbtn->pinNum = pinNum;
	hbtn->prevState = RELEASED;
}

button_state_t Button_GetState(Button_Handler_t *hbtn)
{
	uint32_t curState;
	curState = HAL_GPIO_ReadPin(hbtn->GPIOx, hbtn->pinNum); // normal state is high.

	if ((hbtn->prevState == RELEASED) && (curState == PUSHED)) { // button released -> pushed
		HAL_Delay(2); // debounce
		hbtn->prevState = PUSHED;
		return ACT_PUSHED;
	}
	else if ((hbtn->prevState == PUSHED) && (curState == RELEASED)) { // button pushed -> released
		HAL_Delay(2); // debounce
		hbtn->prevState = RELEASED;
		return ACT_RELEASED;
	}
	return NO_ACT;
}


