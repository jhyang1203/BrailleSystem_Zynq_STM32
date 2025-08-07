/*
 * ButtonPushLock.h
 *
 *  Created on: Jul 23, 2025
 *      Author: kccistc
 */

#ifndef DRIVER_BUTTONPUSHLOCK_BUTTONPUSHLOCK_H_
#define DRIVER_BUTTONPUSHLOCK_BUTTONPUSHLOCK_H_
#include "stm32f4xx_hal.h"

typedef enum {ACT_RELEASED_LK, ACT_PUSHED_LK} buttonlock_state_t;

typedef struct {
	GPIO_TypeDef * GPIOx;
	uint16_t pinNum;
}ButtonLock_Handler_t;

void ButtonLock_Init(ButtonLock_Handler_t *hbtn, GPIO_TypeDef *GPIOx, uint16_t pinNum);
buttonlock_state_t ButtonLock_state(ButtonLock_Handler_t *hbtn);

#endif /* DRIVER_BUTTONPUSHLOCK_BUTTONPUSHLOCK_H_ */
