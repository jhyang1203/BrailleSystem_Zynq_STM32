/*
 * Model_Controller_Presenter.h
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */

#ifndef AP_MODEL_MODEL_CONTROLLER_PRESENTER_H_
#define AP_MODEL_MODEL_CONTROLLER_PRESENTER_H_

#include <stdint.h>
#include "cmsis_os.h"

typedef struct {
    uint8_t pattern;
    char character;
} BrailleChar_t;

extern osMailQId RFTx_brailleCharMailBox;
extern osMailQId RFRx_brailleCharMailBox;
extern osMailQId CNN_brailleCharMailBox;  // 메시지 큐 핸들

void Model_Controller_Presenter_Init();

#endif /* AP_MODEL_MODEL_CONTROLLER_PRESENTER_H_ */
