/*
 * Switch.h
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */

#ifndef AP_CONTROLLER_SWITCH_H_
#define AP_CONTROLLER_SWITCH_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "Model_Listener_Controller.h"
#include "Model_Controller_Presenter.h"
#include <string.h>

extern UART_HandleTypeDef huart2; // huart2 전역 핸들 참조

void Switch_TxExecute();
void Switch_RxExecute();

#endif /* AP_CONTROLLER_SWITCH_H_ */
