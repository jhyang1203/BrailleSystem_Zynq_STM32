/*
 * Presenter.c
 *
 *  Created on: Jul 3, 2025
 *      Author: kccistc
 */


#include "Presenter.h"
#include <stdio.h>

void Presenter_Init()
{
	Presenter_MotorInit();
	Presenter_DFPMiniInit();
}

void Presenter_Excute()
{
	if (mode == S_CNN_MODE) Presenter_MotorExcute();

	else if (mode == S_RF_MODE) {
		Presenter_RF_RxExcute();
		Presenter_RF_TxExcute();
	}
}
