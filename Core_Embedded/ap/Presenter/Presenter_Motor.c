/*
 * Presenter_Motor.c
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */



#include "Presenter_Motor.h"


//#include "Model_Braille.h"

void Presenter_MotorInit()
{
	Motor_Init(&htim5, TIM_CHANNEL_1);
	Motor_Init(&htim5, TIM_CHANNEL_2);
	Motor_Init(&htim3, TIM_CHANNEL_1);
	Motor_Init(&htim3, TIM_CHANNEL_2);
	Motor_Init(&htim3, TIM_CHANNEL_3);
	Motor_Init(&htim3, TIM_CHANNEL_4);

}

void Presenter_MotorExcute()
{
	osEvent evt = osMailGet(CNN_brailleCharMailBox, 0);

	if (evt.status == osEventMail) {
		BrailleChar_t *p = evt.value.p;
		uint8_t bits6 = p->pattern;
		char character = p->character;

		Motor_DisplayBits(bits6);

		LCD_Presenter_RX (character);

		Dfp_playbykey(character);

		osMailFree(CNN_brailleCharMailBox, p);

	}

}
