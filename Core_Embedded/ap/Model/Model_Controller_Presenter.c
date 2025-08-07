/*
 * Model_Controller_Presenter.c
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */

#include "Model_Controller_Presenter.h"
#include "BrailleTable.h"

osMailQId RFTx_brailleCharMailBox;
osMailQDef(RFTx_brailleCharMailQ, 8, BrailleChar_t);

osMailQId RFRx_brailleCharMailBox;
osMailQDef(RFRx_brailleCharMailQ, 8, BrailleChar_t);

osMailQId CNN_brailleCharMailBox;
osMailQDef(CNN_brailleCharMailQ, 8, BrailleAscii_t);

void Model_Controller_Presenter_Init(){
	RFTx_brailleCharMailBox = osMailCreate(osMailQ(RFTx_brailleCharMailQ), NULL);
	RFRx_brailleCharMailBox = osMailCreate(osMailQ(RFRx_brailleCharMailQ), NULL);
	CNN_brailleCharMailBox = osMailCreate(osMailQ(CNN_brailleCharMailQ), NULL);
}
