/*
 * Presenter_LCD.h
 *
 *  Created on: Jul 22, 2025
 *      Author: kccistc
 */

#ifndef AP_PRESENTER_PRESENTER_LCD_H_
#define AP_PRESENTER_PRESENTER_LCD_H_

#include "ili9341.h"

void LCD_InitOnce();

void LCD_Presenter_TX (char alphabet);
void LCD_Presenter_RX (char character);
#endif /* AP_PRESENTER_PRESENTER_LCD_H_ */
