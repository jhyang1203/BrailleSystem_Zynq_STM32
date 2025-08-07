/*
 * RF_Transfer.h
 *
 *  Created on: Jul 17, 2025
 *      Author: kccistc
 */

#ifndef DRIVER_RF_RF_TRANSFER_H_
#define DRIVER_RF_RF_TRANSFER_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "BrailleTable.h"
#include "nrf24l01p.h"
#include "Model_Controller_Presenter.h"
#include "Presenter_LCD.h"
#include "Presenter_DFPMini.h"
#include "Motor.h"

extern UART_HandleTypeDef huart2; // huart2 전역 핸들 참조

extern osMailQId RFTx_brailleCharMailBox;
extern osMailQId RFRx_brailleCharMailBox;

void Presenter_RF_TxExcute();
void Presenter_RF_RxExcute();
void RF_Transfer(uint8_t bits6);

#endif /* DRIVER_RF_RF_TRANSFER_H_ */
