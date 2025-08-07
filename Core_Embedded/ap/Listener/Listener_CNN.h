/*
 * Listener_CNN.h
 *
 *  Created on: Jul 18, 2025
 *      Author: kccistc
 */

#ifndef AP_LISTENER_LISTENER_CNN_H_
#define AP_LISTENER_LISTENER_CNN_H_

#include "stm32f4xx_hal.h"     // GPIO_TypeDef, HAL_XXX, GPIO_PIN_XXX
#include "cmsis_os.h"         // osMessageQId, osMessagePut
#include <stdint.h>           // uint8_t, uint32_t
#include "BrailleTable.h"
#include "usart.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;

extern osMessageQId CNN_brailleMsgBox;

void Listener_CNNStop();
void Listener_CNNExecute();
uint8_t GetBraillePatternFromChar(char c);
char UART_ReceiveChar();

#endif /* AP_LISTENER_LISTENER_CNN_H_ */
