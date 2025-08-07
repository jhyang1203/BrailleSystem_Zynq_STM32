/*
 * Listener_Switch.c
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */

#include "Listener_Switch.h"

/*
typedef struct {
	GPIO_TypeDef *GPIOx;
	uint32_t pinNum;
}Switch_TypeDef;

Switch_TypeDef Switch[6] = {
		{GPIOC, GPIO_PIN_8}, 	// switch 1
		{GPIOC, GPIO_PIN_9}, 	// switch 2
		{GPIOC, GPIO_PIN_5},  	// switch 3
		{GPIOA, GPIO_PIN_12},  	// switch 4
		{GPIOA, GPIO_PIN_11}, 	// switch 5
		{GPIOB, GPIO_PIN_12}, 	// switch 6
};
*/

ButtonLock_Handler_t Button_Braille[6] ={
		{GPIOC, GPIO_PIN_8}, 	// switch 1
		{GPIOC, GPIO_PIN_9}, 	// switch 2
		{GPIOC, GPIO_PIN_5},  	// switch 3
		{GPIOA, GPIO_PIN_12},  	// switch 4
		{GPIOA, GPIO_PIN_11}, 	// switch 5
		{GPIOB, GPIO_PIN_12}, 	// switch 6
};

void Listener_Switch_RxExecute()
{


}

void Listener_Switch_TxExecute()
{
    uint8_t pattern = GetBraillePattern();
    osMessagePut(RFTx_brailleMsgBox, pattern, 0);  // pattern 값을 메시지로 전송
}

uint8_t GetBraillePattern()
{
	uint8_t pattern = 0;

//	if (HAL_GPIO_ReadPin(Button_Braille[0].GPIOx, Button_Braille[0].pinNum) == GPIO_PIN_SET) pattern |= (1 << 0);  // ●1 ← Switch 1
//	if (HAL_GPIO_ReadPin(Button_Braille[1].GPIOx, Button_Braille[1].pinNum) == GPIO_PIN_SET) pattern |= (1 << 1);  // ●2 ← Switch 2
//	if (HAL_GPIO_ReadPin(Button_Braille[2].GPIOx, Button_Braille[2].pinNum) == GPIO_PIN_SET) pattern |= (1 << 2);  // ●3 ← Switch 3
//	if (HAL_GPIO_ReadPin(Button_Braille[3].GPIOx, Button_Braille[3].pinNum) == GPIO_PIN_SET) pattern |= (1 << 3); // ●4 ← Switch 4
//	if (HAL_GPIO_ReadPin(Button_Braille[4].GPIOx, Button_Braille[4].pinNum) == GPIO_PIN_SET) pattern |= (1 << 4); // ●5 ← Switch 5
//	if (HAL_GPIO_ReadPin(Button_Braille[5].GPIOx, Button_Braille[5].pinNum) == GPIO_PIN_SET) pattern |= (1 << 5); // ●6 ← Switch 6

	if (ButtonLock_state(&Button_Braille[0]) == ACT_PUSHED_LK) pattern |= (1 << 0);
	if (ButtonLock_state(&Button_Braille[1]) == ACT_PUSHED_LK) pattern |= (1 << 1);
	if (ButtonLock_state(&Button_Braille[2]) == ACT_PUSHED_LK) pattern |= (1 << 2);
	if (ButtonLock_state(&Button_Braille[3]) == ACT_PUSHED_LK) pattern |= (1 << 3);
	if (ButtonLock_state(&Button_Braille[4]) == ACT_PUSHED_LK) pattern |= (1 << 4);
	if (ButtonLock_state(&Button_Braille[5]) == ACT_PUSHED_LK) pattern |= (1 << 5);

	return pattern;
}


