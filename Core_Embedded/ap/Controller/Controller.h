/*
 * Controller.h
 *
 *  Created on: Jul 3, 2025
 *      Author: kccistc
 */

#ifndef AP_CONTROLLER_CONTROLLER_H_
#define AP_CONTROLLER_CONTROLLER_H_

#include "stm32f4xx_hal.h"
#include "Model_Listener_Controller.h"
#include "Model_Controller_Presenter.h"
#include "Switch.h"
#include "Controller_CNN.h"
#include "Listener.h"

extern UART_HandleTypeDef huart2; // huart2 전역 핸들 참조

void Controller_Init();
void Controller_Execute();

#endif /* AP_CONTROLLER_CONTROLLER_H_ */
