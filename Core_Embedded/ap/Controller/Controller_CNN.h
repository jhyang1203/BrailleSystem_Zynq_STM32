/*
 * Controller_CNN.h
 *
 *  Created on: Jul 18, 2025
 *      Author: kccistc
 */

#ifndef AP_CONTROLLER_CONTROLLER_CNN_H_
#define AP_CONTROLLER_CONTROLLER_CNN_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "Model_Listener_Controller.h"
#include "Model_Controller_Presenter.h"
#include "BrailleTable.h"
#include <string.h>

extern UART_HandleTypeDef huart2; // huart2 전역 핸들 참조

extern osMessageQId CNN_brailleMsgBox;
extern osMailQId CNN_brailleCharMailBox;


void CNN_Execute();

#endif /* AP_CONTROLLER_CONTROLLER_CNN_H_ */
