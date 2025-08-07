/*
 * Presenter_DFPMini.h
 *
 *  Created on: Jul 21, 2025
 *      Author: kccistc
 */

#ifndef AP_PRESENTER_PRESENTER_DFPMINI_H_
#define AP_PRESENTER_PRESENTER_DFPMINI_H_

#include "cmsis_os.h"
#include "dfp_mini.h"
#include "Model_Controller_Presenter.h"

void Presenter_DFPMiniInit();
void Dfp_playbykey(char key);

#endif /* AP_PRESENTER_PRESENTER_DFPMINI_H_ */
