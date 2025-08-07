/*
 * Model_Listener_Controller.h
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */

#ifndef AP_MODEL_MODEL_LISTENER_CONTROLLER_H_
#define AP_MODEL_MODEL_LISTENER_CONTROLLER_H_

#include <stdint.h>
#include "cmsis_os.h"

extern osMessageQId RFTx_brailleMsgBox;
extern osMessageQId RFRx_brailleMsgBox;
extern osMessageQId CNN_brailleMsgBox;  // 메시지 큐 핸들

void Model_Listener_Controller_Init();

#endif /* AP_MODEL_MODEL_LISTENER_CONTROLLER_H_ */
