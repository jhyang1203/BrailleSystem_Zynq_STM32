/*
 * Listener.h
 *
 *  Created on: Jul 3, 2025
 *      Author: kccistc
 */

#ifndef AP_LISTENER_LISTENER_H_
#define AP_LISTENER_LISTENER_H_

#include <stdio.h>
#include "string.h"
#include "Listener_Switch.h"
#include "Listener_CNN.h"
#include "Model_Listener_Controller.h"
#include "Button.h"
#include "ButtonPushLock.h"
#include "tim.h"
#include "dfp_mini.h"

void Listener_Init();
void Listener_Execute();
void ussart();

extern uint8_t mode;
extern uint8_t rf_state;
extern volatile uint8_t tx_done_flag;


typedef enum {S_CNN_MODE, S_RF_MODE} eModestate_t;
typedef enum {S_CNN_STOP, S_CNN_START} eCnnstate_t;
typedef enum {S_RX_MODE, S_TX_MODE} eRfstate_t;

#endif /* AP_LISTENER_LISTENER_H_ */
