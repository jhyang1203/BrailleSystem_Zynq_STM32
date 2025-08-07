/*
 * Presenter_LCD.c
 *
 *  Created on: Jul 22, 2025
 *      Author: kccistc
 */

#include "Presenter_LCD.h"
#include <stdbool.h>

static bool lcd_initialized = false;

void LCD_InitOnce(){
	if(!lcd_initialized) {
		ILI9341_FillScreen(ILI9341_WHITE);
		lcd_initialized = true;
	}
}

void LCD_Presenter_TX (char alphabet){

		//ILI9341_FillScreen(ILI9341_WHITE);
		char str[2] = {alphabet, '\0'};
		ILI9341_WriteString(0, 0, str, Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
}


void LCD_Presenter_RX (char character){

		//ILI9341_FillScreen(ILI9341_WHITE);
		char str[2] = {character, '\0'};
		ILI9341_WriteString(0, 0, str, Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
}




//FontDef Font_7x10 = {7,10,Font7x10};
//FontDef Font_11x18 = {11,18,Font11x18};
//FontDef Font_16x26 = {16,26,Font16x26};
