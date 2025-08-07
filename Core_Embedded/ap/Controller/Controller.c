/*
 * Controller.c
 *
 *  Created on: Jul 3, 2025
 *      Author: kccistc
 */

#include "Controller.h"
#include <string.h>


void Controller_Init()
{
	Model_Controller_Presenter_Init();
}

void Controller_Execute()
{
	if (mode == S_CNN_MODE) CNN_Execute();

	else if (mode == S_RF_MODE) {
		Switch_RxExecute();
		Switch_TxExecute();
	}
}

